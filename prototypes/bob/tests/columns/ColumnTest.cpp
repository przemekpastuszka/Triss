/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include "../../src/columns/ScalarColumn.h"

class ColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;

    virtual void SetUp() {
        double initialValues[] = {5, 12, 7, 8, 19, 1, 12, 5};
        for(int i = 0; i < 8; ++i) {
            c.add(&initialValues[i], 0);
        }
    }

    void assertThatRangeEquals(Range range, int left, int right) {
        ASSERT_EQ(left, range.left);
        ASSERT_EQ(right, range.right);
    }
};

TEST_F(ColumnTest, shouldFindGoodRanges) {
    c.sort(); // 1 5 5 7 8 12 12 19

    double left = 5, right = 12;
    assertThatRangeEquals(c.findRange(&left, &right), 1, 6);

    left = 6, right = 11;
    assertThatRangeEquals(c.findRange(&left, &right), 3, 4);

    left = 13, right = 27;
    assertThatRangeEquals(c.findRange(&left, &right), 7, 7);

    left = -10, right = 5;
    assertThatRangeEquals(c.findRange(&left, &right), 0, 2);

    left = -14, right = -4;
    assertThatRangeEquals(c.findRange(&left, &right), -1, -1);

    left = 24, right = 28;
    assertThatRangeEquals(c.findRange(&left, &right), -1, -1);
}
