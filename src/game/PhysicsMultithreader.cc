#include "PhysicsMultithreader.h"
#include "PhysicsEngine.h"
#include "PhysicsController.h"
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

static vector<thread> ThreadPool;

static mutex JobMutex;
static queue<Entity*> JobQueue;
static condition_variable JobCondition;

static mutex BatchMutex;
static unique_lock<mutex> BatchLock(BatchMutex, std::defer_lock);
static condition_variable BatchCondition;

static mutex ReturnQueueMutex;
static queue<CollisionManifold> ReturnQueue;

PhysicsController *PhysicsController;

bool finished_batch = false;
bool terminate_pool = false;

atomic<int> thread_execution_count;

void blockForJob() {
    const int jobs_len = 100;
    int job_idx;
    Entity *jobs[jobs_len];
    while (true) {
        {
            unique_lock<mutex> lock(JobMutex);
            JobCondition.wait(lock, []{ return !JobQueue.empty() || terminate_pool; });
            if (terminate_pool) return;

            job_idx = 0;
            while (!JobQueue.empty() && job_idx < jobs_len) {
                jobs[job_idx] = JobQueue.front();
                JobQueue.pop();
                job_idx++;
            }
        }
        JobCondition.notify_one();
        
        for (int i = 0; i < job_idx; i++) {
            Entity *entity = jobs[i];
            vector<CollisionManifold> collisions = PhysicsController->runCollisionChecks(entity);
            if (collisions.size() != 0) {
                unique_lock<mutex> lock(ReturnQueueMutex);
                for (const CollisionManifold &ret : collisions) {
                    ReturnQueue.push(ret);
                }
            }
        }

        {
            unique_lock<mutex> lock(ReturnQueueMutex);
            thread_execution_count -= job_idx;
            if (thread_execution_count == 0) {
                // Yield control back to the main thread; this batch is finished.
                BatchLock.unlock();
                finished_batch = true;
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
    terminate_pool = true;
    JobCondition.notify_all();
    for (int i = 0; i < NumThreads; i++) {
        ThreadPool[i].join();
    }
}

map<Entity*, vector<CollisionManifold>> PhysicsMultithreader::run(vector<Entity*> entities, class PhysicsController *pc) {
    BatchLock.lock();
    PhysicsController = pc;
    {
        unique_lock<mutex> lock(JobMutex);
        for (Entity *entity : entities) {
            JobQueue.push(entity);
        }
        thread_execution_count = entities.size();
    }
    JobCondition.notify_one();

    unique_lock<mutex> batch_lock(BatchMutex);
    BatchCondition.wait(batch_lock, []{ return finished_batch; });

    map<Entity*, vector<CollisionManifold>> results;
    while (ReturnQueue.size() != 0) {
        CollisionManifold ret = ReturnQueue.front();
        ReturnQueue.pop();

        results[ret.e1].push_back(ret);
    }

    return results;
}
