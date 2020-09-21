#include "PhysicsMultithreader.h"
#include "PhysicsEngine.h"
#include "PhysicsController.h"
#include "ChunkController.h"
#include "Entity.h"

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <algorithm>
#include <map>
#include <atomic>
#include <utility>

using std::thread;
using std::vector;
using std::queue;
using std::sort;
using std::cout;
using std::cerr;
using std::endl;
using std::mutex;
using std::map;
using std::unique_lock;
using std::condition_variable;
using std::atomic;
using std::pair;

static vector<thread> ThreadPool;
static mutex JobMutex;

enum PhysicsJobType {
    PhysicsJob_Partition, PhysicsJob_Collisions
} JobType;

static queue<pair<int, int>> JobQueue;
static condition_variable JobCondition;

static mutex BatchMutex;
static unique_lock<mutex> BatchLock(BatchMutex, std::defer_lock);
static condition_variable BatchCondition;

static mutex ReturnQueueMutex;
static queue<CollisionManifold> ReturnQueue;

ChunkController   *ChunkController;
PhysicsController *PhysicsController;

bool FinishedBatch = false;
bool TerminatePool = false;

static mutex ThreadExecutionMutex;
int ThreadExecutionCount;

void blockForJob() {
    while (true) {
        pair<int, int> chunk;
        {
            unique_lock<mutex> lock(JobMutex);
            JobCondition.wait(lock, []{ return !JobQueue.empty() || TerminatePool; });
            if (TerminatePool) {
                JobCondition.notify_one();
                return;
            }

            chunk = JobQueue.front();
            JobQueue.pop();
        }
        JobCondition.notify_one();

        int x = chunk.first;
        int y = chunk.second;

        switch (JobType) {
          /* PARTITIONING ENTITIES INTO CHUNKS */
          case PhysicsJob_Partition:
            ChunkController->updatePartitionForChunk(x, y);
            break;

          /* COLLISION CHECKS WITHIN CHUNKS */
          case PhysicsJob_Collisions:
            ChunkContainer &chunk = ChunkController->getChunk(x, y);
            vector<CollisionManifold> collisions = PhysicsController->runCollisionChecks(chunk);
            if (collisions.size() != 0) {
                unique_lock<mutex> lock(ReturnQueueMutex);
                for (const CollisionManifold &ret : collisions) {
                    ReturnQueue.push(ret);
                }
            }
            break;
        }

        {
            unique_lock<mutex> lock(ThreadExecutionMutex);
            ThreadExecutionCount -= 1;
            if (ThreadExecutionCount == 0) {
                // Yield control back to the main thread; this batch is finished.
                BatchLock.unlock();
                FinishedBatch = true;
                BatchCondition.notify_all();
            }
        }
    }
}

static int NumThreads = thread::hardware_concurrency();

void PhysicsMultithreader::init() {
    if (NumThreads == 0) {
        // TODO: Handle this case
        exit(1);
    }

    for (int i = 0; i < NumThreads; i++) {
        ThreadPool.push_back(thread(blockForJob));
    }
}

void PhysicsMultithreader::shutdown() {
    {
        unique_lock<mutex> lock(ThreadExecutionMutex);
        TerminatePool = true;
    }

    JobCondition.notify_all();
    for (int i = 0; i < NumThreads; i++) {
        ThreadPool[i].join();
    }
}

void execute_jobs() {
    BatchLock.lock();
    {
        unique_lock<mutex> job_lock(JobMutex);
        unique_lock<mutex> thread_lock(ThreadExecutionMutex);

        int width = ChunkController->width();
        int height = ChunkController->height();
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                JobQueue.push({x,y});
            }
        }
        ThreadExecutionCount = width * height;
        FinishedBatch = false;
    }
    JobCondition.notify_one();

    // Wait for job to complete
    unique_lock<mutex> lock(JobMutex);
    BatchCondition.wait(lock, []{ return FinishedBatch; });
}

void PhysicsMultithreader::run_partitioning(class PhysicsController *pc, class ChunkController *cc) {
    PhysicsController = pc;
    ChunkController = cc;

    JobType = PhysicsJob_Partition;
    execute_jobs();
}

map<Entity*, vector<CollisionManifold>>
      PhysicsMultithreader::run_collision_checks(class PhysicsController *pc, class ChunkController *cc) {
    PhysicsController = pc;
    ChunkController = cc;

    // Check for collisions by chunk
    JobType = PhysicsJob_Collisions;
    execute_jobs();

    map<Entity*, vector<CollisionManifold>> results;
    while (ReturnQueue.size() != 0) {
        CollisionManifold ret = ReturnQueue.front();
        ReturnQueue.pop();
        results[ret.e1].push_back(ret);
    }

    return results;
}
