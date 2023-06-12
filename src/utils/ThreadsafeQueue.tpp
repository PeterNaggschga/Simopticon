#include "ThreadsafeQueue.h"

template<class T>
void ThreadsafeQueue<T>::push(T val) {
    queueLock.lock();
    safeQueue.push(val);
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
