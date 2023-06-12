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
    mutex queueLock;
    queue<Key> taskQueue;

    pair<Key, bool> getNextRun();

    virtual T work(Key arg) = 0;

protected:
    virtual map<Key, T, Compare, Allocator> runMultithreadedFunctions(set<Key, Compare> runs);

    virtual map<Key, T, Compare, Allocator> multithreadFunction();

public:
    explicit Multithreaded(unsigned int threads);

};

#include "Multithreaded.tpp"

#endif //SIMOPTICON_MULTITHREADED_H
