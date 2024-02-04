#include "allocator2.hpp"

namespace buddy_alloc {

size_t Allocator::ClosestPow2(size_t n) {
    size_t res = 1;
    size_t p = 0;
    while (res < n) {
        p++;
        res *= 2;
    }
    return p;
}

size_t Allocator::Pow2(size_t n) {
    return PowOf2[n];
}

char* Allocator::GetFirstAndDelete(std::unordered_set<char*>& s) {
    char* res = *(s.begin());
    s.erase(s.begin());
    return res;
}

void Allocator::TryMerge(char* block, size_t p) {
    size_t size = Pow2(p);
    size_t num;
    size_t startP = p;
    std::vector<char*> toErase;
    while (true) {
        if (p == N) {
            break;
        }
        num = (block - start) / size;
        bool f1 = (num % 2 == 0) && (freeBlocks[p].find(block + size) != freeBlocks[p].end());
        bool f2 = (num % 2 == 1) && (freeBlocks[p].find(block - size) != freeBlocks[p].end());
        if (!f1 && !f2) {
            break;
        }
        if (f1) {
            toErase.push_back(block + size);
        }
        if (f2) {
            toErase.push_back(block);
            block -= size;
        }
        p++;
    }

    for (size_t i = 0; i < toErase.size(); i++) {
        freeBlocks[startP + i].erase(toErase[i]);
    }
    freeBlocks[p].insert(block);
}


Allocator::Allocator(size_t _size) {
    N = 0;
    size = 1;
    while (_size > size) {
        N++;
        size *= 2;
    }
    start = new char[size];
    freeBlocks.resize(N + 1);
    freeBlocks[N].insert(start);
    PowOf2.resize(N + 1);
    PowOf2[0] = 1;
    for (size_t i = 1; i < N + 1; i++) {
        PowOf2[i] = PowOf2[i - 1] * 2;
    }
}

Allocator::~Allocator() {
    delete[] start;
}

void* Allocator::Alloc(size_t blockSize) {
    size_t p = ClosestPow2(blockSize);
    size_t act = 0;
    char* block;
    for (size_t i = p; i <= N; i++) {
        if (!freeBlocks[i].empty()) {
            act = i;
            block = GetFirstAndDelete(freeBlocks[i]);
            break;
        }
    }
    if (act == 0) {
        throw std::bad_alloc();
    }
    while (act != p) {
        act--;
        char* buddy = block + Pow2(act);
        freeBlocks[act].insert(buddy);
    }
    sizes[block] = p;
    return block;
}

void Allocator::Free(void *block) {
    char* chBlock = (char*)block;
    size_t p = sizes[chBlock];
    sizes.erase(chBlock);
    TryMerge(chBlock, p);
}

}
