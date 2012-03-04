/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <list>
#include "../src/ValueRange.h"

#define INFINITY  1000

class ValueRangeTest : public testing::Test {
    public:
    ValueRange<double> *rangeA, *rangeB;

    virtual void SetUp() {
        rangeA = rangeB = NULL;
    }
    virtual void TearDown() {
        if(rangeA != NULL) { delete rangeA; }
        if(rangeB != NULL) { delete rangeB; }
    }

    void setUpRangeA(TypedConstraint<double>* constraint) {
        rangeA = ValueRange<double>::createFromConstraint(*constraint);
        delete constraint;
    }
    void setUpRangeB(TypedConstraint<double>* constraint) {
        rangeB = ValueRange<double>::createFromConstraint(*constraint);
        delete constraint;
    }

    void assertRangeEquality(ValueRange<double>* range, double left, double right) {
        if(left == -INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheLeft());
            ASSERT_TRUE(range -> isInRange(-INFINITY));
        }
        else {
            ASSERT_FALSE(range -> isInfiniteOnTheLeft());
            ASSERT_DOUBLE_EQ(left, range -> getLeft());
            ASSERT_FALSE(range -> isInRange(left - 1));
            ASSERT_TRUE(range -> isInRange(left));
        }

        if(right == INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheRight());
            ASSERT_TRUE(range -> isInRange(INFINITY));
        }
        else {
            ASSERT_FALSE(range -> isInfiniteOnTheRight());
            ASSERT_DOUBLE_EQ(right, range -> getRight());
            ASSERT_FALSE(range -> isInRange(right + 1));
            ASSERT_TRUE(range -> isInRange(right));
        }

        ASSERT_FALSE(range -> isEmpty());
        ASSERT_TRUE(range -> isInRange((left + right) / 2));
    }

    void assertRangeIsEmpty(ValueRange<double>* range) {
        ASSERT_TRUE(range -> isEmpty());
        ASSERT_FALSE(range -> isInRange(-INFINITY));
        ASSERT_FALSE(range -> isInRange(INFINITY));
    }
};

TEST_F(ValueRangeTest, shouldConvertLessToValidRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));

    assertRangeEquality(rangeA, -INFINITY, 17);
}

TEST_F(ValueRangeTest, shouldConvertGreaterToValidRange) {
    setUpRangeA(TypedConstraint<double>::greaterOrEqual(0, 17));

    assertRangeEquality(rangeA, 17, INFINITY);
}

TEST_F(ValueRangeTest, shouldConvertEqualsToValidRange) {
    setUpRangeA(TypedConstraint<double>::equals(0, 17));

    assertRangeEquality(rangeA, 17, 17);
}

TEST_F(ValueRangeTest, shouldConvertContainsToValidRange) {
    setUpRangeA(TypedConstraint<double>::contains(0, 17));

    assertRangeEquality(rangeA, 17, 17);
}

TEST_F(ValueRangeTest, shouldGetInfiniteRangeFromTwoInfiniteOnes) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::lessOrEqual(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, -INFINITY, 10);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::greaterOrEqual(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes2) {
    setUpRangeB(TypedConstraint<double>::greaterOrEqual(0, 10));
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromDifferentRanges) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::equals(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 10);
}

TEST_F(ValueRangeTest, shouldGetEmptyRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::equals(0, 21));
    rangeA -> intersectWith(rangeB);

    assertRangeIsEmpty(rangeA);
}
