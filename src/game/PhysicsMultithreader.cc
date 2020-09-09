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

using std::thread;
using std::vector;
using std::queue;
using std::cout;
using std::cerr;
using std::endl;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using PhysicsMultithreader::JobReturn;

static vector<thread> ThreadPool;

static mutex QueueMutex;
static queue<Entity*> Queue;
static condition_variable QueueCondition;

static mutex BatchMutex;
static unique_lock<mutex> BatchLock(BatchMutex, std::defer_lock);
static condition_variable BatchCondition;
static mutex ReturnQueueMutex;
static queue<JobReturn> ReturnQueue;

GameController *MainGameController;

bool finished_batch = false;
bool terminate_pool = false;

void blockForJob() {
    while (true) {
        Entity *entity;
        bool finished = false;
        {
            unique_lock<mutex> lock(QueueMutex);
            QueueCondition.wait(lock, []{ return !Queue.empty() || terminate_pool; });
            entity = Queue.front();
            Queue.pop();

            if (Queue.size() == 0) finished = true;
        }
        
        vector<JobReturn> collisions = MainGameController->runCollisionChecks(entity);
        if (collisions.size() != 0) {
            unique_lock<mutex> lock(ReturnQueueMutex);
            for (const JobReturn &ret : collisions) {
                ReturnQueue.push(ret);
            }
        }

        if (finished) {
            // Yield control back to the main thread; this batch is finished.
            BatchLock.unlock();
            finished_batch = true;
            BatchCondition.notify_all();
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
        unique_lock<mutex> lock(QueueMutex);
        for (Entity *entity : entities) {
            Queue.push(entity);
        }
    }
    QueueCondition.notify_one();

    unique_lock<mutex> batch_lock(BatchMutex);
    BatchCondition.wait(batch_lock, []{ return finished_batch; });

    while (ReturnQueue.size() != 0) {
        JobReturn ret = ReturnQueue.front();
        ReturnQueue.pop();

        Vector2D pv;
        if (ret.type == 0) {
            if (PhysicsEngine::checkCollision(ret.e1, ret.e2, &pv)) {
                ret.e1->handleCollision(ret.e2, pv);
            }
        } else if (ret.type == 1) {
            if (PhysicsEngine::checkHitboxCollision(ret.e1, ret.e2, &pv)) {
                ret.e1->handleHit(ret.e2, pv);
            }
        }
    }
}
