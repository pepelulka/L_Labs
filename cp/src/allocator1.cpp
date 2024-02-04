#include "allocator1.hpp"

namespace fb_alloc {

Allocator::Allocator(size_t _size) : size(_size) {
    start = new char[size];
    end = (start + size);
    blocks.push_back({size, start});
}

Allocator::~Allocator() {
    delete[] start;
}

void* Allocator::Alloc(size_t blockSize) {
    auto best = blocks.end();
    int bestValue = -1;
    for (auto it = blocks.begin(); it != blocks.end(); it++) {
        Block cur = *it;
        if (cur.size >= blockSize) {
            if ((int)(cur.size - blockSize) < bestValue || bestValue == -1) {
                bestValue = cur.size - blockSize;
                best = it;
            }
        }
    }
    if (best == blocks.end()) {
        throw std::bad_alloc();
    }
    best->size -= blockSize;
    best->start += blockSize;
    void* result = static_cast<void*>(best->start - blockSize);
    sizes[result] = blockSize;
    return result;
}

void Allocator::Free(void *block) {
    if (sizes.find(block) == sizes.end()) {
        throw std::logic_error("(fb allocator) Bad pointer for free.");
    }
    size_t size = sizes[block];
    std::list<Block>::iterator backMerge = blocks.end(), forwardMerge = blocks.end();
    for (auto it = blocks.begin(); it != blocks.end(); it++) {
        size_t sz = it->size;
        char* start = it->start, *end = it->start + sz;
        if (end == (char*)block) {
            backMerge = it;
        }
        if (start == ((char*)block + size)) {
            forwardMerge = it;
        }
    }
    if (backMerge != blocks.end() && forwardMerge != blocks.end()) {
        Block newBlock = { size + backMerge->size + forwardMerge->size, backMerge->start};
        forwardMerge++;
        auto it = blocks.erase(backMerge, forwardMerge);
        blocks.insert(it, newBlock);
    } else if (backMerge != blocks.end()) {
        Block newBlock = { size + backMerge->size, backMerge->start };
        auto it = blocks.erase(backMerge);
        blocks.insert(it, newBlock);
    } else if (forwardMerge != blocks.end()) {
        Block newBlock = {size + forwardMerge->size, (char*)block };
        auto it = blocks.erase(forwardMerge);
        blocks.insert(it, newBlock);
    } else {
        Block newBlock = { size, (char*)block };
        auto place = blocks.end();
        for (auto it = blocks.begin(); it != blocks.end(); it++) {
            if (it->start > (char*)block) {
                place = it;
                break;
            }
        }
        blocks.insert(place, newBlock);
    }
    sizes.erase(block);
}

}
