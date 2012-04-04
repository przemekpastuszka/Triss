/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <gtest/gtest.h>
#include <list>
#include <src/common/ValueRange.h>

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
        rangeA = ValueRange<double>::createFromConstraint(constraint);
        delete constraint;
    }
    void setUpRangeB(TypedConstraint<double>* constraint) {
        rangeB = ValueRange<double>::createFromConstraint(constraint);
        delete constraint;
    }

    void assertRangeEquality(ValueRange<double>* range, double left, double right, bool leftOpen, bool rightOpen) {
        if(left == -INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheLeft());
            ASSERT_TRUE(range -> isInRange(-INFINITY));
        }
        else {
            ASSERT_FALSE(range -> isInfiniteOnTheLeft());
            ASSERT_DOUBLE_EQ(left, range -> getLeft());
            ASSERT_FALSE(range -> isInRange(left - 1));

            ASSERT_EQ(leftOpen, range -> isOpenOnTheLeft());
            ASSERT_EQ(!leftOpen, range -> isInRange(left));
        }

        if(right == INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheRight());
            ASSERT_TRUE(range -> isInRange(INFINITY));
        }
        else {
            ASSERT_FALSE(range -> isInfiniteOnTheRight());
            ASSERT_DOUBLE_EQ(right, range -> getRight());
            ASSERT_FALSE(range -> isInRange(right + 1));

            ASSERT_EQ(rightOpen, range -> isOpenOnTheRight());
            ASSERT_EQ(!rightOpen, range -> isInRange(right));
        }

        if((leftOpen || rightOpen) && left == right) {
            ASSERT_TRUE(range -> isEmpty());
        }
        else {
            ASSERT_FALSE(range -> isEmpty());
        }

        if(leftOpen && rightOpen == false) {
            ASSERT_TRUE(range -> isInRange((left + right) / 2));
        }
    }

    void assertRangeIsEmpty(ValueRange<double>* range) {
        ASSERT_TRUE(range -> isEmpty());
        ASSERT_FALSE(range -> isInRange(-INFINITY));
        ASSERT_FALSE(range -> isInRange(INFINITY));
    }
};

TEST_F(ValueRangeTest, shouldConvertLessOrEqualToValidRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));

    assertRangeEquality(rangeA, -INFINITY, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertGreaterOrEqualToValidRange) {
    setUpRangeA(TypedConstraint<double>::greaterOrEqual(0, 17));

    assertRangeEquality(rangeA, 17, INFINITY, false, false);
}

TEST_F(ValueRangeTest, shouldConvertEqualsToValidRange) {
    setUpRangeA(TypedConstraint<double>::equals(0, 17));

    assertRangeEquality(rangeA, 17, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertContainsToValidRange) {
    setUpRangeA(TypedConstraint<double>::contains(0, 17));

    assertRangeEquality(rangeA, 17, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertLessToValidRange) {
    setUpRangeA(TypedConstraint<double>::less(0, 17));

    assertRangeEquality(rangeA, -INFINITY, 17, false, true);
}

TEST_F(ValueRangeTest, shouldConvertGreaterToValidRange) {
    setUpRangeA(TypedConstraint<double>::greater(0, 17));

    assertRangeEquality(rangeA, 17, INFINITY, true, false);
}

TEST_F(ValueRangeTest, shouldGetInfiniteRangeFromTwoInfiniteOnes) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::lessOrEqual(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, -INFINITY, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::greaterOrEqual(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes2) {
    setUpRangeB(TypedConstraint<double>::greaterOrEqual(0, 10));
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromDifferentRanges) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::equals(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetEmptyRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::equals(0, 21));
    rangeA -> intersectWith(rangeB);

    assertRangeIsEmpty(rangeA);
}

TEST_F(ValueRangeTest, shouldGetInfiniteRangeFromTwoInfiniteOnesWithOpenRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::less(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, -INFINITY, 10, false, true);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnesWithOpenRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::greater(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17, true, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnesWithOpenRange2) {
    setUpRangeB(TypedConstraint<double>::greaterOrEqual(0, 10));
    setUpRangeA(TypedConstraint<double>::less(0, 17));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 17, false, true);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromDifferentRangesWithOpenRange) {
    setUpRangeA(TypedConstraint<double>::less(0, 17));
    setUpRangeB(TypedConstraint<double>::equals(0, 10));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, 10, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetOpenRangeWhenClosedAndOpenGiven) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::less(0, 17));
    rangeA -> intersectWith(rangeB);

    assertRangeEquality(rangeA, -INFINITY, 17, false, true);
}

TEST_F(ValueRangeTest, shouldGetEmptyRangeWithOpenRange) {
    setUpRangeA(TypedConstraint<double>::lessOrEqual(0, 17));
    setUpRangeB(TypedConstraint<double>::greater(0, 17));
    rangeA -> intersectWith(rangeB);

    assertRangeIsEmpty(rangeA);
}
