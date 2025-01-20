#include <gtest/gtest.h>
#include <iostream>
#include "../src/CircularBuffer.h"

TEST(CircularBuffer, testPush)
{
    CircularBuffer b(5);

    b.push_front('2');
    b.push_front('1');
    b.push_back('3');
    b.push_front('0');
    b.push_back('4');
    EXPECT_EQ(b[0], '0');
    EXPECT_EQ(b[1], '1');
    EXPECT_EQ(b[2], '2');
    EXPECT_EQ(b[3], '3');
    EXPECT_EQ(b[4], '4');
}

TEST(CircularBuffer, testEquals)
{
    CircularBuffer a(5, '+');
    CircularBuffer b(a);
    CircularBuffer c(5, '-');

    for (int i = 0; i < a.size(); i++)
    {
        EXPECT_EQ(a.at(i), '+');
        EXPECT_EQ(b.at(i), '+');
    }
    EXPECT_EQ(b == a, true);
    EXPECT_EQ(b == c, false);

    b = c;
    EXPECT_EQ(b == c, true);
    EXPECT_EQ(b != c, false);
}

TEST(CircularBuffer, testFrontBack)
{
    CircularBuffer b(2, '-');

    b[0] = '0';
    b.at(1) = '1';
    EXPECT_EQ(b.front(), '0');
    EXPECT_EQ(b.back(), '1');

    b.front() = '2';
    b.back() = '3';
    EXPECT_EQ(b.front(), '2');
    EXPECT_EQ(b.back(), '3');
}

TEST(CircularBuffer, testFill)
{
    CircularBuffer b(5);

    b.push_front('2');
    b.push_front('1');
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b.capacity(), 5);
    EXPECT_EQ(b.empty(), false);
    EXPECT_EQ(b.full(), false);
    EXPECT_EQ(b.reserve(), 3);
}

TEST(CircularBuffer, testSetCapacity)
{
    CircularBuffer b(5);
    EXPECT_EQ(b.capacity(), 5);

    b.set_capacity(10);
    EXPECT_EQ(b.capacity(), 10);

    b.set_capacity(0);
    EXPECT_EQ(b.capacity(), 0);
}

TEST(CircularBuffer, testMoveAndCopyConstruct)
{
    CircularBuffer a(5, '-');
    CircularBuffer b(a);
    EXPECT_EQ(b == a, true);
    CircularBuffer c(std::move(a));
    EXPECT_EQ(b == a, false);
    EXPECT_EQ(c == a, false);
    EXPECT_EQ(c == b, true);
}

TEST(CircularBuffer, testRotate1)
{
    CircularBuffer a(5);
    a.insert(0, '0');
    a.insert(1, '1');
    a.insert(2, '2');
    a.insert(3, '3');
    a.insert(4, '4');

    EXPECT_EQ(a.front(), '0');
    EXPECT_EQ(a.back(), '4');

    a.rotate(3);
    EXPECT_EQ(a.front(), '3');
    EXPECT_EQ(a.back(), '2');
}

TEST(CircularBuffer, testRotate2)
{
    CircularBuffer a(5);
    a.insert(0, '0');
    a.insert(1, '2');
    a.insert(1, '1');

    a.rotate(2);
    EXPECT_EQ(a[0], '2');
    EXPECT_EQ(a[1], '0');
    EXPECT_EQ(a[2], '1');
}

TEST(CircularBuffer, testLinearize)
{
    CircularBuffer a(5, '-');
    a.linearize();
    EXPECT_EQ(a.is_linearized(), true);
}

TEST(CircularBuffer, testPop)
{
    CircularBuffer a(5);
    a.insert(0, '0');
    a.insert(1, '1');
    a.insert(2, '2');
    a.insert(3, '3');
    a.insert(4, '4');

    EXPECT_EQ(a.back(), '4');
    a.pop_back();
    EXPECT_EQ(a.back(), '3');
    a.pop_back();
    EXPECT_EQ(a.back(), '2');

    EXPECT_EQ(a.front(), '0');
    a.pop_front();
    EXPECT_EQ(a.front(), '1');
    a.pop_front();
    EXPECT_EQ(a.front(), '2');
}

TEST(CircularBuffer, testSwap)
{
    CircularBuffer a(5, '-');
    CircularBuffer b(3, '+');
    for (int i = 0; i < a.size(); i++)
    {
        EXPECT_EQ(a[i], '-');
    }
    for (int i = 0; i < b.size(); i++)
    {
        EXPECT_EQ(b[i], '+');
    }

    b.swap(a);
    for (int i = 0; i < a.size(); i++)
    {
        EXPECT_EQ(a[i], '+');
    }
    for (int i = 0; i < b.size(); i++)
    {
        EXPECT_EQ(b[i], '-');
    }
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(b.size(), 5);
}

TEST(CircularBuffer, testClear)
{
    CircularBuffer a(5, '+');
    a[0] = '0';
    a[1] = '1';
    a[2] = '2';
    a[3] = '3';
    a[4] = '4';
    EXPECT_EQ(a.size(), 5);
    EXPECT_EQ(a.front() == a.back(), false);

    a.clear();
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.front() == a.back(), true);
}

TEST(CircularBuffer, testResize)
{
    CircularBuffer a(5, '+');
    a.resize(10, '-');
    EXPECT_EQ(a.size(), 10);
    for (int i = 0; i < a.size(); i++)
    {
        if (i < 5) 
        {
            EXPECT_EQ(a[i], '+');
        }
        else
        {
            EXPECT_EQ(a[i], '-');
        }
    }

    a.resize(6);
    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a.back(), '-');
    EXPECT_EQ(a.back(), a[5]);
}

TEST(CircularBuffer, testErase)
{
    CircularBuffer a(5, '+');
    a[0] = '0';
    a[1] = '1';
    a[2] = '2';
    a[3] = '3';
    a[4] = '4';

    a.erase(1, 3);
    EXPECT_EQ(a[0], '0');
    EXPECT_EQ(a[1], '3');
    EXPECT_EQ(a[2], '4');
    EXPECT_EQ(a.size(), 3);
}

TEST(CircularBuffer, testConstFrontBack)
{
    const CircularBuffer a(5, '+');

    EXPECT_EQ(a.front(), '+');
    EXPECT_EQ(a.back(), '+');
}

TEST(CircularBuffer, testLinearizeInsert)
{
    CircularBuffer a(5, '+');

    a.insert(4, '-');
    EXPECT_EQ(a.at(3), '-');
    EXPECT_ANY_THROW(a.at(7));
}

TEST(CircularBuffer, testInsertAndPushCapacityZero)
{
    CircularBuffer a(0);
    a.insert(1, '-');
    EXPECT_EQ(a.size(), 0);

    a.push_back('-');
    EXPECT_EQ(a.size(), 0);
    
    a.push_front('-');
    EXPECT_EQ(a.size(), 0);
}

TEST(CircularBuffer, testPopSizeZero)
{
    CircularBuffer a(0);
    EXPECT_EQ(a.size(), 0);
    a.pop_back();
    a.pop_front();
    EXPECT_EQ(a.size(), 0);
}

TEST(CircularBuffer, testPushThrow)
{
    CircularBuffer a(5, '-');
    EXPECT_ANY_THROW(a.push_front('+'));
    EXPECT_ANY_THROW(a.push_back('+'));
}

TEST(CircularBuffer, testConstAt)
{
    const CircularBuffer a(5, '-');
    EXPECT_EQ(a.at(0), '-');
    EXPECT_EQ(a.at(3), '-');

    EXPECT_ANY_THROW(a.at(6));
    EXPECT_ANY_THROW(a.at(10));
}