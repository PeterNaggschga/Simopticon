#ifndef SIMOPTICON_MULTITHREADED_H
#define SIMOPTICON_MULTITHREADED_H


#include <mutex>
#include <queue>
#include <map>
#include <functional>
#include <set>
#include "ThreadsafeQueue.h"

using namespace std;

/**
 * A class implementing concurrent execution of the same function for different arguments.
 * The function must be implemented through #work and execution follows the ThreadPool design pattern.
 * @tparam Key: Argument type of the concurrent #work function.
 * @tparam T: Result type of the concurrent #work function.
 * @tparam Compare: Comparison for objects of type Key.
 * @tparam Allocator: Allocator for pairs of constant Key and T.
 * @ingroup utils
 */
template<class Key, class T, class Compare = less<Key>, class Allocator = allocator<pair<const Key, T>>>
class Multithreaded {
private:
    /**
     * Function that should be executed concurrently on different arguments.
     * @param arg: Argument of the concurrently executed function.
     * @return Return value of the concurrently executed function.
     */
    virtual T work(Key arg) = 0;

protected:
    /**
     * Maximum number of concurrent threads to be used in ThreadPool.
     */
    const unsigned int NR_THREADS;
    /**
     * ThreadsafeQueue containing the arguments that have to be processed by the ThreadPool.
     */
    ThreadsafeQueue<Key> queue;

    /**
     * Pushes given tasks into #queue, creates concurrent threads and merges them when execution is done.
     * @param runs: Set of arguments on which #work should to be executed.
     * @return A map which maps arguments to their respective calculated values.
     */
    virtual map<Key, T, Compare, Allocator> runMultithreadedFunctions(set<Key, Compare> runs);

    /**
     * Function that is executed by each thread. As long as #queue is not empty, tasks are started.
     * When #queue is empty, the processed results are returned
     * @return A map which maps arguments to their respective calculated values.
     */
    virtual map<Key, T, Compare, Allocator> multithreadFunction();

public:
    /**
     * Creates a Multithreaded class that does not use more than the given number of threads.
     * @param threads: Maximum number of threads to use.
     */
    explicit Multithreaded(unsigned int threads);

};

#include "Multithreaded.tpp"

#endif //SIMOPTICON_MULTITHREADED_H
