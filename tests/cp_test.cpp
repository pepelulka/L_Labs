#include <gtest/gtest.h>

#include <allocator1.hpp>
#include <allocator2.hpp>

#include <cstring>

TEST(CpTest, Test1) {
    fb_alloc::Allocator alloc(30);
    char* p1 = static_cast<char*>(alloc.Alloc(10));
    char* p2 = static_cast<char*>(alloc.Alloc(10));
    char* p3 = static_cast<char*>(alloc.Alloc(10));
    EXPECT_ANY_THROW(alloc.Alloc(1));
    std::strcpy(p1, "foobar");
    std::strcpy(p2, "meow");
    std::strcpy(p3, "hohoho");
    EXPECT_TRUE(std::strcmp(p1, "foobar") == 0);
    EXPECT_TRUE(std::strcmp(p2, "meow") == 0);
    EXPECT_TRUE(std::strcmp(p3, "hohoho") == 0);
    alloc.Free(p1);
    alloc.Free(p2);
    alloc.Free(p3);
    p1 = static_cast<char*>(alloc.Alloc(10));
    p2 = static_cast<char*>(alloc.Alloc(10));
    p3 = static_cast<char*>(alloc.Alloc(10));
    std::strcpy(p1, "foobar");
    std::strcpy(p2, "meow");
    std::strcpy(p3, "hohoho");
    EXPECT_TRUE(std::strcmp(p1, "foobar") == 0);
    EXPECT_TRUE(std::strcmp(p2, "meow") == 0);
    EXPECT_TRUE(std::strcmp(p3, "hohoho") == 0);
}

TEST(CpTest, Test2) {
    buddy_alloc::Allocator alloc(40);
    char* p1 = static_cast<char*>(alloc.Alloc(10));
    char* p2 = static_cast<char*>(alloc.Alloc(10));
    char* p3 = static_cast<char*>(alloc.Alloc(10));
    std::strcpy(p1, "foobar");
    std::strcpy(p2, "meow");
    std::strcpy(p3, "hohoho");
    EXPECT_TRUE(std::strcmp(p1, "foobar") == 0);
    EXPECT_TRUE(std::strcmp(p2, "meow") == 0);
    EXPECT_TRUE(std::strcmp(p3, "hohoho") == 0);
    alloc.Free(p1);
    alloc.Free(p2);
    alloc.Free(p3);
    p1 = static_cast<char*>(alloc.Alloc(10));
    p2 = static_cast<char*>(alloc.Alloc(10));
    p3 = static_cast<char*>(alloc.Alloc(10));
    std::strcpy(p1, "foobar");
    std::strcpy(p2, "meow");
    std::strcpy(p3, "hohoho");
    EXPECT_TRUE(std::strcmp(p1, "foobar") == 0);
    EXPECT_TRUE(std::strcmp(p2, "meow") == 0);
    EXPECT_TRUE(std::strcmp(p3, "hohoho") == 0);
    EXPECT_ANY_THROW(alloc.Alloc(100));
}