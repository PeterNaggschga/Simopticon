#include <future>
#include <list>
#include <ranges>


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
    Key run;
    while (!(run = getNextRun()).empty()) {
        result.insert(make_pair(run, work(run)));
    }
    return result;
}

template<class Key, class T, class Compare, class Allocator>
Key Multithreaded<Key, T, Compare, Allocator>::getNextRun() {
    queueLock.lock();
    if (taskQueue.empty()) {
        queueLock.unlock();
        return {};
    }
    auto result = taskQueue.front();
    taskQueue.pop();
    queueLock.unlock();
    return result;
}
