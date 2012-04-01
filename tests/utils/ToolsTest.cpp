/*
 * Copyright 2012 Przemysław Pastuszka
 */
#include <gtest/gtest.h>
#include <utility>
#include <vector>
#include <src/utils/Tools.h>

using namespace std;

typedef pair<double, char*> test_pair;

class ToolsTest : public :: testing :: Test {
protected:

    void assertPairEquals(const test_pair& pair, double num, const char* str) {
        ASSERT_EQ(pair.first, num);
        ASSERT_EQ(pair.second, str);
    }
};

TEST_F(ToolsTest, shouldAddComplexTypesToVector) {
    vector<test_pair> pairs = Tools::vector<test_pair>(3,
            test_pair(1, "one"),
            test_pair(2, "two"),
            test_pair(7, "seven")
            );

    assertPairEquals(pairs[0], 1, "one");
    assertPairEquals(pairs[1], 2, "two");
    assertPairEquals(pairs[2], 7, "seven");
}

