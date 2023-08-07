/**
 * @file
 * In this file the implementation of the Multithreaded class is defined.
 */

#include <future>
#include <list>


template<class Key, class T, class Compare, class Allocator>
Multithreaded<Key, T, Compare, Allocator>::Multithreaded(unsigned int threads) : NR_THREADS(threads) {
}

template<class Key, class T, class Compare, class Allocator>
map<Key, T, Compare, Allocator>
Multithreaded<Key, T, Compare, Allocator>::runMultithreadedFunctions(set<Key, Compare> runs) {
    for (const auto &entry: runs) {
        queue.push(entry);
    }

    list<future < map<Key, T, Compare, Allocator>> > threads;
    for (int i = 0; i < min((size_t) NR_THREADS, runs.size()); ++i) {
        threads.push_back(async(launch::async, &Multithreaded::multithreadFunction, this));
    }

    map<Key, T, Compare, Allocator> result;
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        it->wait();
        result.merge(it->get());
    }
    return result;
}

template<class Key, class T, class Compare, class Allocator>
map<Key, T, Compare, Allocator> Multithreaded<Key, T, Compare, Allocator>::multithreadFunction() {
    map<Key, T, Compare, Allocator> result;
    pair<Key, bool> run;
    while ((run = queue.pop()).second) {
        result.insert(make_pair(run.first, work(run.first)));
    }
    return result;
}
