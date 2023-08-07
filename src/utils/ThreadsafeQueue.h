#ifndef SIMOPTICON_THREADSAFEQUEUE_H
#define SIMOPTICON_THREADSAFEQUEUE_H

/**
 * @file
 * In this file, the header of the ThreadSafeQueue class is defined.
 */

#include <queue>
#include <mutex>

using namespace std;

/**
 * A container class of a queue that is safe for concurrent access of different threads.
 * @tparam Key: Type of elements in the contained queue.
 * @ingroup utils
 */
template<class Key>
class ThreadsafeQueue {
private:
    /**
     * The actual queue data structure.
     */
    queue<Key> safeQueue;
    /**
     * Threadlock to avoid damage to #safeQueue on concurrent access.
     */
    mutex queueLock;
    /**
     * Number of elements in queue when #push was called the last time.
     * Can be used for progress information.
     */
    size_t startSize = 0;

public:
    /**
     * Adds the given value to #safeQueue.
     * @param val: Values to be added to queue.
     */
    void push(Key val);

    /**
     * Returns the first element of the queue. If the queue is empty, the second entry of the returned pair is false.
     * @return A pair containing an element of type Key and a boolean determining if access was successful.
     */
    pair<Key, bool> pop();

    /**
     * Returns the value of #startSize.
     * @return A number representing the number of tasks, when #push was called last.
     */
    [[nodiscard]] size_t getStartSize();

    /**
     * Returns current size of the underlying queue structure.
     * @return A number representing the size of the queue.
     */
    [[nodiscard]] size_t getSize();

};

#include "ThreadsafeQueue.tpp"

#endif //SIMOPTICON_THREADSAFEQUEUE_H
