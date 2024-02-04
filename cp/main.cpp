#include "timer.hpp"
#include "allocator1.hpp"
#include "allocator2.hpp"

template<typename Alloc>
void Test1(uint64_t& timeAlloc, uint64_t& timeFree) {
    Alloc alloc(1000000);
    timer::Timer tmAlloc, tmFree;
    std::vector<void*> p;
    tmAlloc.StartMeasure();
    for (int i = 0; i < 50000; i++) {
        p.push_back(alloc.Alloc(8));
    }
    tmAlloc.FinishMeasure();
    tmFree.StartMeasure();
    for (const auto& i : p) {
        alloc.Free(i);
    }
    tmFree.FinishMeasure();
    timeAlloc = tmAlloc.Get();
    timeFree = tmFree.Get();
}

template<typename Alloc>
void Test2(uint64_t& timeAlloc, uint64_t& timeFree) {
    Alloc alloc(2000000);
    timer::Timer tmAlloc, tmFree;
    std::vector<void*> p;
    //
    tmAlloc.StartMeasure();
    for (int i = 0; i < 50000; i++) {
        p.push_back(alloc.Alloc(3));
    }
    tmAlloc.FinishMeasure();
    for (int i = 0; i < 50000; i += 2) {
        tmFree.StartMeasure();
        alloc.Free(p[i]);
        alloc.Free(p[i + 1]);
        tmFree.FinishMeasure();
        tmAlloc.StartMeasure();
        p[i] = alloc.Alloc(3);
        tmAlloc.FinishMeasure();
    }
    //
    timeAlloc = tmAlloc.Get();
    timeFree = tmFree.Get();
}

template<typename Alloc>
void Test3(uint64_t& timeAlloc, uint64_t& timeFree) {
    Alloc alloc(2000000); // 2 000 000
    timer::Timer tmAlloc, tmFree;
    //
    constexpr size_t N = 7;
    std::vector<std::stack<void*> > p(N);
    std::vector<size_t> sizes = {10, 20, 100, 1000, 20000, 100000, 500000};
    std::vector<size_t> alloc1 = {5, 10, 5, 25, 15, 5, 1};
    std::vector<size_t> free1 = {4, 4, 2, 23, 12, 0, 0};
    std::vector<size_t> alloc2 = {5, 5, 4, 23, 15, 0, 0};
    std::vector<size_t> free2 = {5, 5, 4, 24, 12, 0, 0};

    auto MassAlloc = [&](const std::vector<size_t>& data) {
        tmAlloc.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                p[i].push(alloc.Alloc(sizes[i]));
            }
        }
        tmAlloc.FinishMeasure();
    };

    auto MassFree = [&](const std::vector<size_t>& data) {
        tmFree.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                void* ptr = p[i].top();
                p[i].pop();
                alloc.Free(ptr);
            }
        }
        tmFree.FinishMeasure();
    };

    MassAlloc(alloc1);
    MassFree(free1);
    MassAlloc(alloc2);
    MassFree(free2);

    //
    timeAlloc = tmAlloc.Get();
    timeFree = tmFree.Get();
}

// Сильная дефрагментация
template<typename Alloc>
void Test4(uint64_t& timeAlloc, uint64_t& timeFree) {
    Alloc alloc(2000000); // 2 000 000
    timer::Timer tmAlloc, tmFree;
    //
    constexpr size_t N = 7;
    std::vector<std::stack<void*> > p(N);
    std::vector<size_t> sizes = {10, 20, 100, 1000, 20000, 100000, 500000};

    std::vector<std::vector<size_t> > allocs = {
        {1000, 100, 10, 10, 5, 3, 1},
        {500, 50, 5, 5, 3, 1, 0},
        {100, 10, 2, 1, 1, 0, 0},
        {100, 10, 2, 1, 1, 0, 0},
        {100, 10, 2, 1, 1, 0, 0}
    };

    std::vector<std::vector<size_t> > frees = {
        {250, 25, 2, 3, 2, 1, 0},
        {250, 25, 2, 3, 2, 1, 0},
        {250, 25, 2, 3, 2, 0, 0},
        {250, 25, 2, 3, 2, 0, 0},
        {250, 25, 2, 3, 2, 0, 0}
    };

    auto MassAlloc = [&](const std::vector<size_t>& data) {
        tmAlloc.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                p[i].push(alloc.Alloc(sizes[i]));
            }
        }
        tmAlloc.FinishMeasure();
    };

    auto MassFree = [&](const std::vector<size_t>& data) {
        tmFree.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                void* ptr = p[i].top();
                p[i].pop();
                alloc.Free(ptr);
            }
        }
        tmFree.FinishMeasure();
    };

    for (size_t i = 0; i < allocs.size(); i++) {
        MassAlloc(allocs[i]);
        MassFree(frees[i]);
    }

    //
    timeAlloc = tmAlloc.Get();
    timeFree = tmFree.Get();
}

// Сильная дефрагментация
template<typename Alloc>
void Test5(uint64_t& timeAlloc, uint64_t& timeFree) {
    Alloc alloc(2000000); // 2 000 000
    timer::Timer tmAlloc, tmFree;
    //
    constexpr size_t N = 5;
    std::vector<std::stack<void*> > p(N);
    std::vector<size_t> sizes = {2, 5, 10, 25, 50};

    std::vector<std::vector<size_t> > allocs = {
        {10000, 1000, 100, 100, 5, 3, 1},
        {5000, 500, 50, 50, 3, 1, 0},
        {1000, 100, 20, 10, 1, 0, 0},
        {1000, 100, 20, 10, 1, 0, 0},
        {1000, 100, 20, 10, 1, 0, 0}
    };

    std::vector<std::vector<size_t> > frees = {
        {2500, 250, 20, 30, 2, 1, 0},
        {2500, 250, 20, 30, 2, 1, 0},
        {2500, 250, 20, 30, 2, 0, 0},
        {2500, 250, 20, 30, 2, 0, 0},
        {2500, 250, 20, 30, 2, 0, 0}
    };

    auto MassAlloc = [&](const std::vector<size_t>& data) {
        tmAlloc.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                p[i].push(alloc.Alloc(sizes[i]));
            }
        }
        tmAlloc.FinishMeasure();
    };

    auto MassFree = [&](const std::vector<size_t>& data) {
        tmFree.StartMeasure();
        for (size_t i = 0; i < sizes.size(); i++) {
            for (size_t j = 0; j < data[i]; j++) {
                void* ptr = p[i].top();
                p[i].pop();
                alloc.Free(ptr);
            }
        }
        tmFree.FinishMeasure();
    };

    for (size_t i = 0; i < allocs.size(); i++) {
        MassAlloc(allocs[i]);
        MassFree(frees[i]);
    }

    //
    timeAlloc = tmAlloc.Get();
    timeFree = tmFree.Get();
}

int main() {
    uint64_t timeAlloc, timeFree;

    // Test1

    std::cout << "Сценарий 1: Много маленьких блоков, никакой дефрагментации.\n";
    Test1<fb_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм свободных блоков: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";
    Test1<buddy_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм двойников: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";

    // Test 2

    std::cout << "Сценарий 2: Много маленьких блоков, небольшая дефрагментация.\n";
    Test2<fb_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм свободных блоков: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";
    Test2<buddy_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм двойников: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";

    // Test 3

    std::cout << "Сценарий 3: Блоки разных размеров, несильная дефрагментация\n";
    Test3<fb_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм свободных блоков: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";
    Test3<buddy_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм двойников: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";

    // Test 4

    std::cout << "Сценарий 4: Блоки разных размеров, сильная дефрагментация\n";
    Test4<fb_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм свободных блоков: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";
    Test4<buddy_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм двойников: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";

    // Test 5

    std::cout << "Сценарий 5: Много блоков маленьких размеров, сильная дефрагментация\n";
    Test5<fb_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм свободных блоков: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";
    Test5<buddy_alloc::Allocator>(timeAlloc, timeFree);
    std::cout << "Алгоритм двойников: аллокация - " << timeAlloc << " микросекунд " << " деаллокация - " << timeFree << " микросекунд\n";

}