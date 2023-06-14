#ifndef SIMOPTICON_MULTITHREADED_H
#define SIMOPTICON_MULTITHREADED_H


#include <mutex>
#include <queue>
#include <map>
#include <functional>
#include <set>
#include "ThreadsafeQueue.h"

using namespace std;

template<class Key, class T, class Compare = less<Key>, class Allocator = allocator<pair<const Key, T>>>
class Multithreaded {
private:
    virtual T work(Key arg) = 0;

protected:
    const unsigned int NR_THREADS;
    ThreadsafeQueue<Key> queue;

    virtual map<Key, T, Compare, Allocator> runMultithreadedFunctions(set<Key, Compare> runs);

    virtual map<Key, T, Compare, Allocator> multithreadFunction();

public:
    explicit Multithreaded(unsigned int threads);

};

#include "Multithreaded.tpp"

#endif //SIMOPTICON_MULTITHREADED_H
