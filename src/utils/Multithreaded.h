#ifndef SIMOPTICON_MULTITHREADED_H
#define SIMOPTICON_MULTITHREADED_H


#include <mutex>
#include <queue>
#include <map>
#include <functional>
#include <set>

using namespace std;

template<class Key, class T, class Compare = less<Key>, class Allocator = allocator<pair<const Key, T>>>
class Multithreaded {
private:
    const unsigned int NR_THREADS;

    struct ThreadSafeQueue {
        mutex queueLock;
        queue<Key> taskQueue;

        void push(Key val) {
            queueLock.lock();
            taskQueue.push(val);
            queueLock.unlock();
        }

        pair<Key, bool> pop() {
            queueLock.lock();
            if (taskQueue.empty()) {
                queueLock.unlock();
                return make_pair(Key{}, false);
            }
            auto val = taskQueue.front();
            taskQueue.pop();
            queueLock.unlock();
            return make_pair(val, true);
        }
    };

    ThreadSafeQueue queue;

    virtual T work(Key arg) = 0;

protected:
    virtual map<Key, T, Compare, Allocator> runMultithreadedFunctions(set<Key, Compare> runs);

    virtual map<Key, T, Compare, Allocator> multithreadFunction();

public:
    explicit Multithreaded(unsigned int threads);

};

#include "Multithreaded.tpp"

#endif //SIMOPTICON_MULTITHREADED_H
