#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stack>

namespace buddy_alloc {

class Allocator {
private:

    size_t N, size;
    char* start;
    // freeBlocks : (n) -> (set of char* of size 2^n)
    std::vector< std::unordered_set<char*> > freeBlocks;
    // sizes : (block) -> (n) where size(block) = 2^n
    std::unordered_map<char*, size_t> sizes;

    // return pow
    std::vector<size_t> PowOf2;
    size_t Pow2(size_t n);
    static size_t ClosestPow2(size_t n);
    static char* GetFirstAndDelete(std::unordered_set<char*>& s);

    void TryMerge(char* block, size_t p);

public:

    Allocator(size_t _size);
    ~Allocator();
    void* Alloc(size_t blockSize);
    void Free(void *block);

};

}
