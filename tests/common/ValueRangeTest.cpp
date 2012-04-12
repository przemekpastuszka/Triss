/*
 * Copyright 2012 Przemysław Pastuszka
 */
#include <gtest/gtest.h>
#include <list>
#include <src/common/ValueRangeSet.h>
#include <src/utils/Tools.h>

#define INFINITY  1000

class ValueRangeTest : public testing::Test {
public:
    ValueRangeSet<double> *rangeSet;

    struct RangeDesc {
        double left, right;
        bool leftOpen, rightOpen;
        
        RangeDesc(double l, double r, bool lO, bool rO)
                : left(l), right(r), leftOpen(lO), rightOpen(rO) {}
    };
    
    virtual void SetUp() {
        rangeSet = NULL;
    }

    virtual void TearDown() {
        if (rangeSet != NULL) {
            delete rangeSet;
        }
    }

    void setUpRange(TypedConstraint<double>* constraint) {
        rangeSet = ValueRangeSet<double>::createFromConstraint(constraint);
        delete constraint;
    }

    void intersectWith(TypedConstraint<double>* constraint) {
        rangeSet -> intersectWith(constraint);
        delete constraint;
    }

    void assertFirstRangeEquality(double left, double right, bool leftOpen, bool rightOpen) {
        assertRangeSetEquality(Tools::vector<RangeDesc>(1, /**/ RangeDesc(left, right, leftOpen, rightOpen)));
    }
    
    void assertRangeSetEquality(std::vector<RangeDesc> descs) {
        rangeSet -> begin();
        
        for(int i = 0; i < descs.size(); ++i) {
            ASSERT_TRUE(rangeSet -> hasNext());
            assertRangeEquality(rangeSet -> next(), descs[i].left, descs[i].right, descs[i].leftOpen, descs[i].rightOpen);
        }
        ASSERT_FALSE(rangeSet -> hasNext());
    }

    void assertRangeEquality(ValueRange<double>* range, double left, double right, bool leftOpen, bool rightOpen) {
        if (left == -INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheLeft());
            ASSERT_TRUE(range -> isInRange(-INFINITY));
        } else {
            ASSERT_FALSE(range -> isInfiniteOnTheLeft());
            ASSERT_DOUBLE_EQ(left, range -> getLeft());
            ASSERT_FALSE(range -> isInRange(left - 1));

            ASSERT_EQ(leftOpen, range -> isOpenOnTheLeft());
            ASSERT_EQ(!leftOpen, range -> isInRange(left));
        }

        if (right == INFINITY) {
            ASSERT_TRUE(range -> isInfiniteOnTheRight());
            ASSERT_TRUE(range -> isInRange(INFINITY));
        } else {
            ASSERT_FALSE(range -> isInfiniteOnTheRight());
            ASSERT_DOUBLE_EQ(right, range -> getRight());
            ASSERT_FALSE(range -> isInRange(right + 1));

            ASSERT_EQ(rightOpen, range -> isOpenOnTheRight());
            ASSERT_EQ(!rightOpen, range -> isInRange(right));
        }

        if ((leftOpen || rightOpen) && left == right) {
            ASSERT_TRUE(range -> isEmpty());
        } else {
            ASSERT_FALSE(range -> isEmpty());
        }

        if (leftOpen && rightOpen == false) {
            ASSERT_TRUE(range -> isInRange((left + right) / 2));
        }
    }

    void assertRangeIsEmpty() {
        rangeSet -> begin();
        ValueRange<double> *range = rangeSet -> next();

        ASSERT_TRUE(range -> isEmpty());
        ASSERT_FALSE(range -> isInRange(-INFINITY));
        ASSERT_FALSE(range -> isInRange(INFINITY));
    }
};

TEST_F(ValueRangeTest, shouldConvertLessOrEqualToValidRange) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));

    assertFirstRangeEquality( -INFINITY, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertGreaterOrEqualToValidRange) {
    setUpRange(TypedConstraint<double>::greaterOrEqual(0, 17));

    assertFirstRangeEquality( 17, INFINITY, false, false);
}

TEST_F(ValueRangeTest, shouldConvertEqualsToValidRange) {
    setUpRange(TypedConstraint<double>::equals(0, 17));

    assertFirstRangeEquality( 17, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertContainsToValidRange) {
    setUpRange(TypedConstraint<double>::contains(0, 17));

    assertFirstRangeEquality( 17, 17, false, false);
}

TEST_F(ValueRangeTest, shouldConvertLessToValidRange) {
    setUpRange(TypedConstraint<double>::less(0, 17));

    assertFirstRangeEquality( -INFINITY, 17, false, true);
}

TEST_F(ValueRangeTest, shouldConvertGreaterToValidRange) {
    setUpRange(TypedConstraint<double>::greater(0, 17));

    assertFirstRangeEquality( 17, INFINITY, true, false);
}

TEST_F(ValueRangeTest, shouldGetInfiniteRangeFromTwoInfiniteOnes) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::lessOrEqual(0, 10));

    assertFirstRangeEquality( -INFINITY, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::greaterOrEqual(0, 10));

    assertFirstRangeEquality( 10, 17, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnes2) {
    setUpRange(TypedConstraint<double>::greaterOrEqual(0, 10));
    intersectWith(TypedConstraint<double>::lessOrEqual(0, 17));

    assertFirstRangeEquality( 10, 17, false, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromDifferentRanges) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::equals(0, 10));

    assertFirstRangeEquality( 10, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetEmptyRange) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::equals(0, 21));

    assertRangeIsEmpty();
}

TEST_F(ValueRangeTest, shouldGetInfiniteRangeFromTwoInfiniteOnesWithOpenRange) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::less(0, 10));

    assertFirstRangeEquality( -INFINITY, 10, false, true);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnesWithOpenRange) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::greater(0, 10));

    assertFirstRangeEquality( 10, 17, true, false);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromTwoInfiniteOnesWithOpenRange2) {
    setUpRange(TypedConstraint<double>::greaterOrEqual(0, 10));
    intersectWith(TypedConstraint<double>::less(0, 17));

    assertFirstRangeEquality( 10, 17, false, true);
}

TEST_F(ValueRangeTest, shouldGetFiniteRangeFromDifferentRangesWithOpenRange) {
    setUpRange(TypedConstraint<double>::less(0, 17));
    intersectWith(TypedConstraint<double>::equals(0, 10));

    assertFirstRangeEquality( 10, 10, false, false);
}

TEST_F(ValueRangeTest, shouldGetOpenRangeWhenClosedAndOpenGiven) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::less(0, 17));

    assertFirstRangeEquality( -INFINITY, 17, false, true);
}

TEST_F(ValueRangeTest, shouldGetEmptyRangeWithOpenRange) {
    setUpRange(TypedConstraint<double>::lessOrEqual(0, 17));
    intersectWith(TypedConstraint<double>::greater(0, 17));

    assertRangeIsEmpty();
}

TEST_F(ValueRangeTest, shouldGetTwoRangesUsingNotEqual) {
    setUpRange(TypedConstraint<double>::notEqual(0, 17));

    assertRangeSetEquality(Tools::vector<RangeDesc>(2, /**/
            RangeDesc(-INFINITY, 17, false, true),
            RangeDesc(17, INFINITY, true, false)));
}

TEST_F(ValueRangeTest, shouldGetTwoRangesUsingNotEqualAndLess) {
    setUpRange(TypedConstraint<double>::notEqual(0, 17));
    intersectWith(TypedConstraint<double>::less(0, 20));

    assertRangeSetEquality(Tools::vector<RangeDesc>(2, /**/
            RangeDesc(-INFINITY, 17, false, true),
            RangeDesc(17, 20, true, true)));
}

TEST_F(ValueRangeTest, shouldGetTwoRangesUsingNotEqualAndLessAndGreater) {
    setUpRange(TypedConstraint<double>::notEqual(0, 17));
    intersectWith(TypedConstraint<double>::less(0, 20));
    intersectWith(TypedConstraint<double>::greater(0, 5));

    assertRangeSetEquality(Tools::vector<RangeDesc>(2, /**/
            RangeDesc(5, 17, true, true),
            RangeDesc(17, 20, true, true)));
}

TEST_F(ValueRangeTest, shouldGetOneRangeUsingNotEqualAndLess) {
    setUpRange(TypedConstraint<double>::notEqual(0, 17));
    intersectWith(TypedConstraint<double>::less(0, 15));

    assertRangeSetEquality(Tools::vector<RangeDesc>(1, /**/
            RangeDesc(-INFINITY, 15, true, true)));
}

TEST_F(ValueRangeTest, shouldGetThreeRangesUsingTwoNotEquals) {
    setUpRange(TypedConstraint<double>::notEqual(0, 17));
    intersectWith(TypedConstraint<double>::notEqual(0, 20));

    assertRangeSetEquality(Tools::vector<RangeDesc>(3, /**/
            RangeDesc(-INFINITY, 17, false, true),
            RangeDesc(17, 20, true, true),
            RangeDesc(20, INFINITY, true, false)));
}