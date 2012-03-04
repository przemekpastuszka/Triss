/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include <list>
#include "../../src/columns/ScalarColumn.h"

class ColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;

    std::list<Constraint*> constraints;

    virtual void SetUp() {
        double initialValues[] = {5, 12, 7, 8, 19, 1, 12, 5};
        for(int i = 0; i < 8; ++i) {
            c.add(&initialValues[i], 0);
        }
        c.sort(); // 1 5 5 7 8 12 12 19
    }

    virtual void TearDown() {
        for(std::list<Constraint*>::iterator it = constraints.begin();
                it != constraints.end();
                it++) {
            delete *it;
        }
        constraints.clear();
    }

    void checkForConstraints(int left, int right) {
        for(std::list<Constraint*>::iterator it = constraints.begin();
                it != constraints.end();
                it++) {
            c.addConstraint(*it);
        }

        Column::IndexRange range = c.getRangeFromConstraints();

        ASSERT_EQ(left, range.left);
        ASSERT_EQ(right, range.right);
    }

    void assertThatRangeEquals(double leftValue, double rightValue, int left, int right) {
        Constraint* l = TypedConstraint<double>::greaterOrEqual(0, leftValue);
        Constraint* r = TypedConstraint<double>::lessOrEqual(0, rightValue);

        c.addConstraint(l);
        c.addConstraint(r);

        Column::IndexRange range = c.getRangeFromConstraints();

        delete l; delete r;

        ASSERT_EQ(left, range.left);
        ASSERT_EQ(right, range.right);
    }
};

TEST_F(ColumnTest, shouldReturnEmptyRangeForDisjointConstraints) {
    constraints.push_back(TypedConstraint<double>::lessOrEqual(0, 10));
    constraints.push_back(TypedConstraint<double>::greaterOrEqual(0, 11));

    checkForConstraints(-1, -1);
}

TEST_F(ColumnTest, shouldReturnEmptyRange) {
    constraints.push_back(TypedConstraint<double>::equals(0, 10));

    checkForConstraints(-1, -1);
}

TEST_F(ColumnTest, shouldReturnValidRangeUsingLeftInfinityConstraint) {
    constraints.push_back(TypedConstraint<double>::lessOrEqual(0, 10));

    checkForConstraints(0, 4);
}

TEST_F(ColumnTest, shouldReturnValidRangeUsingRightInfinityConstraint) {
    constraints.push_back(TypedConstraint<double>::greaterOrEqual(0, 10));

    checkForConstraints(5, 7);
}

TEST_F(ColumnTest, shouldFindGoodRanges) {
    assertThatRangeEquals(5, 12, /**/ 1, 6);
    assertThatRangeEquals(6, 11, /**/ 3, 4);
    assertThatRangeEquals(13, 27, /**/ 7, 7);
    assertThatRangeEquals(-10, 5, /**/ 0, 2);
    assertThatRangeEquals(10, 10, /**/ -1, -1);
    assertThatRangeEquals(-14, -4, /**/ -1, -1);
    assertThatRangeEquals(24, 28, /**/ -1, -1);
}
