#ifndef SIMOPTICON_THREADSAFEQUEUE_H
#define SIMOPTICON_THREADSAFEQUEUE_H


#include <queue>
#include <mutex>

using namespace std;

template<class Key>
class ThreadsafeQueue {
private:
    queue<Key> safeQueue;
    mutex queueLock;
    size_t startSize = 0;

public:
    void push(Key val);

    pair<Key, bool> pop();

    [[nodiscard]] size_t getStartSize();

    [[nodiscard]] size_t getSize();

};

#include "ThreadsafeQueue.tpp"

#endif //SIMOPTICON_THREADSAFEQUEUE_H
