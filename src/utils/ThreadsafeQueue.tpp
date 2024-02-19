#include "ThreadsafeQueue.h"

/**
 * @file
 * In this file, the implementation of the ThreadSafeQueue class is defined.
 * @author Per Natzschka
 */

template<class Key>
void ThreadsafeQueue<Key>::push(Key val) {
    queueLock.lock();
    safeQueue.push(val);
    startSize = safeQueue.size();
    queueLock.unlock();
}

template<class Key>
std::pair<Key, bool> ThreadsafeQueue<Key>::pop() {
    queueLock.lock();
    if (safeQueue.empty()) {
        queueLock.unlock();
        return make_pair(Key{}, false);
    }
    auto val = safeQueue.front();
    safeQueue.pop();
    queueLock.unlock();
    return make_pair(val, true);
}

template<class Key>
size_t ThreadsafeQueue<Key>::getStartSize() {
    queueLock.lock();
    size_t res = startSize;
    queueLock.unlock();
    return res;
}

template<class Key>
size_t ThreadsafeQueue<Key>::getSize() {
    queueLock.lock();
    size_t res = safeQueue.size();
    queueLock.unlock();
    return res;
}
