#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>

namespace fb_alloc {

struct Block {
    size_t size;
    char *start;
};

class Allocator {
private:
    size_t size;
    char *start, *end; // end is the next after last block
    std::list<Block> blocks;
    std::unordered_map<void*, size_t> sizes;
public:

    Allocator(size_t _size);
    ~Allocator();
    void* Alloc(size_t blockSize);
    void Free(void *block);

};

}
