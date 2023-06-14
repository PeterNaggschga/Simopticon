#include "ThreadsafeQueue.h"

template<class T>
void ThreadsafeQueue<T>::push(T val) {
    queueLock.lock();
    safeQueue.push(val);
    startSize = safeQueue.size();
    queueLock.unlock();
}

template<class T>
pair<T, bool> ThreadsafeQueue<T>::pop() {
    queueLock.lock();
    if (safeQueue.empty()) {
        queueLock.unlock();
        return make_pair(T{}, false);
    }
    auto val = safeQueue.front();
    safeQueue.pop();
    queueLock.unlock();
    return make_pair(val, true);
}

template<class T>
size_t ThreadsafeQueue<T>::getStartSize() {
    queueLock.lock();
    size_t res = startSize;
    queueLock.unlock();
    return res;
}

template<class T>
size_t ThreadsafeQueue<T>::getSize() {
    queueLock.lock();
    size_t res = safeQueue.size();
    queueLock.unlock();
    return res;
}
