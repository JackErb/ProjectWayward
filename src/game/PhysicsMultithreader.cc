#include "PhysicsMultithreader.h"
#include "PhysicsEngine.h"
#include "GameController.h"
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
using PhysicsMultithreader::JobReturn;
using std::atomic;

static vector<thread> ThreadPool;

static mutex JobMutex;
static queue<Entity*> JobQueue;
static condition_variable JobCondition;

static mutex BatchMutex;
static unique_lock<mutex> BatchLock(BatchMutex, std::defer_lock);
static condition_variable BatchCondition;

static mutex ReturnQueueMutex;
static queue<JobReturn> ReturnQueue;

GameController *MainGameController;

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
            vector<JobReturn> collisions = MainGameController->runCollisionChecks(entity);
            if (collisions.size() != 0) {
                unique_lock<mutex> lock(ReturnQueueMutex);
                for (const JobReturn &ret : collisions) {
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

void PhysicsMultithreader::init() {
    int NumThreads = thread::hardware_concurrency();
    if (NumThreads == 0) {
        // TODO: Handle this case
        exit(1);
    }

    for (int i = 0; i < NumThreads; i++) {
        ThreadPool.push_back(thread(blockForJob));
    }
}

void PhysicsMultithreader::runJobs(vector<Entity*> entities, GameController *gc) {
    BatchLock.lock();
    MainGameController = gc;
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

    map<Entity*, vector<JobReturn>> results;
    while (ReturnQueue.size() != 0) {
        JobReturn ret = ReturnQueue.front();
        ReturnQueue.pop();

        results[ret.e1].push_back(ret);
    }

    // Apply collisions


    // First apply horizontal pvs
    Vector2D pv;
    for (auto it = results.begin(); it != results.end(); it++) {
        vector<JobReturn> collisions = it->second;
        for (JobReturn ret : collisions) {
            if (ret.type == 0 && ret.pv.y == 0) {
                // Collision pushing in horizontal direction
                if (PhysicsEngine::checkCollision(ret.e1, ret.e2, &pv)) {
                    ret.e1->handleCollision(ret.e2, pv);
                }
            }
        }
    }

    // Now apply the rest of the pvs (if still colliding)
    for (auto it = results.begin(); it != results.end(); it++) {
        vector<JobReturn> collisions = it->second;
        for (JobReturn ret : collisions) {
            if (ret.type == 0) {
                // Collision pushing in horizontal direction
                if (PhysicsEngine::checkCollision(ret.e1, ret.e2, &pv)) {
                    ret.e1->handleCollision(ret.e2, pv);
                }
            } else {
                if (PhysicsEngine::checkHitboxCollision(ret.e1, ret.e2, &pv)) {
                    ret.e2->handleHit(ret.e1, pv);
                }
            }
        }
    }
}
