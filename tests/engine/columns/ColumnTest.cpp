/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include <list>
#include <src/engine/columns/ScalarColumn.h>
#include <src/engine/Table.h>
#include <src/common/Schema.h>

class ColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;

    std::list<Constraint*> constraints;

    virtual void SetUp() {
        c.setColumnId(0);

        Schema::DataType schema[] = { Schema::NUMERICAL };
        Schema sch(schema, 1);
        Table table;
        table.setSchema(sch);
        Row* row = table.createTableRow();

        double initialValues[] = {5, 12, 7, 8, 19, 1, 12, 5};
        for(int i = 0; i < 8; ++i) {
            row -> set<double>(0, initialValues[i]);
            c.add(*row, 0);
        }
        c.sort(); // 1 5 5 7 8 12 12 19

        delete row;
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
        ColumnQueryState* state = c.prepareColumnForQuery();


        for(std::list<Constraint*>::iterator it = constraints.begin();
                it != constraints.end();
                it++) {
            c.addConstraint(*it, state);
        }

        IndexRange range = c.reduceConstraintsToRange(state);

        delete state;

        ASSERT_EQ(left, range.left);
        ASSERT_EQ(right, range.right);
    }

    void assertThatRangeEquals(double leftValue, double rightValue, int left, int right) {
        Constraint* l = TypedConstraint<double>::greaterOrEqual(0, leftValue);
        Constraint* r = TypedConstraint<double>::lessOrEqual(0, rightValue);

        ColumnQueryState* state = c.prepareColumnForQuery();
        c.addConstraint(l, state);
        c.addConstraint(r, state);

        IndexRange range = c.reduceConstraintsToRange(state);

        delete l; delete r; delete state;

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

TEST_F(ColumnTest, shouldReturnWholeRange) {
    checkForConstraints(0, 7);
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
