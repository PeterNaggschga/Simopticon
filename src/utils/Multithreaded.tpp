#include <future>
#include <list>
#include <ranges>
#include <Python.h>


template<class Key, class T, class Compare, class Allocator>
Multithreaded<Key, T, Compare, Allocator>::Multithreaded(unsigned int threads) : NR_THREADS(threads) {
}

template<class Key, class T, class Compare, class Allocator>
map<Key, T, Compare, Allocator>
Multithreaded<Key, T, Compare, Allocator>::runMultithreadedFunctions(set<Key, Compare> runs) {
    for (const auto &entry: runs) {
        taskQueue.push(entry);
    }

    list<future < map<Key, T, Compare, Allocator>> > threads;
    Py_Initialize();
    for (int i = 0; i < min((size_t) NR_THREADS, runs.size()); ++i) {
        threads.push_back(async(launch::async, &Multithreaded::multithreadFunction, this));
    }

    map<Key, T, Compare, Allocator> result;
    for (auto &thread: ranges::reverse_view(threads)) {
        thread.wait();
        result.merge(thread.get());
    }

    return result;
}

template<class Key, class T, class Compare, class Allocator>
map<Key, T, Compare, Allocator> Multithreaded<Key, T, Compare, Allocator>::multithreadFunction() {
    map<Key, T, Compare, Allocator> result;
    pair<Key, bool> run;
    while ((run = getNextRun()).second) {
        result.insert(make_pair(run.first, work(run.first)));
    }
    return result;
}

template<class Key, class T, class Compare, class Allocator>
pair<Key, bool> Multithreaded<Key, T, Compare, Allocator>::getNextRun() {
    queueLock.lock();
    if (taskQueue.empty()) {
        queueLock.unlock();
        return make_pair(Key{}, false);
    }
    auto nextTask = taskQueue.front();
    taskQueue.pop();
    queueLock.unlock();
    return make_pair(nextTask, true);
}
