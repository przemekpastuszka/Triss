/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include <vector>
#include <src/engine/columns/ScalarColumn.h>
#include <src/engine/Table.h>
#include <src/common/ColumnDesc.h>
#include <src/utils/Tools.h>

class ColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;

    std::vector<Constraint*> constraints;

    virtual void SetUp() {
        c.setColumnId(0);

        std::vector<ColumnDesc> schema;
        schema.push_back(ColumnDesc("a", Schema::NUMERICAL));
        Table table;
        table.setSchema(schema);
        TableRow* row = table.createTableRow();

        double initialValues[] = {5, 12, 7, 8, 19, 1, 12, 5};
        for(int i = 0; i < 8; ++i) {
            row -> set<double>(0, initialValues[i]);
            c.add(*row, 0);
        }
        c.sort(); // 1 5 5 7 8 12 12 19

        delete row;
    }

    virtual void TearDown() {
        deleteConstraints();
    }
    
    void assertThatRangeEquals(double leftValue, double rightValue, int left, int right) {
        deleteConstraints();
        constraints.push_back(TypedConstraint<double>::greaterOrEqual(0, leftValue));
        constraints.push_back(TypedConstraint<double>::lessOrEqual(0, rightValue));
        
        checkForConstraints(left, right);
    }
    
    void assertThatRangeIsEmpty(double leftValue, double rightValue) {
        deleteConstraints();
        constraints.push_back(TypedConstraint<double>::greaterOrEqual(0, leftValue));
        constraints.push_back(TypedConstraint<double>::lessOrEqual(0, rightValue));
        
        checkForConstraints(std::vector<IndexRange>());
    }
    
    void checkForConstraints(int left, int right) {
        checkForConstraints(Tools::vector<IndexRange>(1, /**/ IndexRange(left, right)));
    }
    
    void checkForConstraints(std::vector<IndexRange> ranges) {
        ColumnQueryState* state = c.prepareColumnForQuery();

        for(int i = 0; i < constraints.size(); ++i) {
            c.addConstraint(constraints[i], state);
        }

        IndexRangeSet rangeSet = c.reduceConstraintsToRangeSet(state);

        delete state;
        
        ASSERT_EQ(ranges.size(), rangeSet.ranges.size());

        for(int i = 0; i < ranges.size(); ++i) {
            ASSERT_EQ(ranges[i].left, rangeSet.ranges[i].left);
            ASSERT_EQ(ranges[i].right, rangeSet.ranges[i].right);
        }
    }
    
   void deleteConstraints() {
        for(int i = 0; i < constraints.size(); ++i) {
            delete constraints[i];
        }
        constraints.clear();
    }
};

TEST_F(ColumnTest, shouldReturnEmptyRangeForDisjointConstraints) {
    constraints.push_back(TypedConstraint<double>::lessOrEqual(0, 10));
    constraints.push_back(TypedConstraint<double>::greaterOrEqual(0, 11));

    checkForConstraints(std::vector<IndexRange>());
}

TEST_F(ColumnTest, shouldReturnEmptyRange) {
    constraints.push_back(TypedConstraint<double>::equals(0, 10));

    checkForConstraints(std::vector<IndexRange>());
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

TEST_F(ColumnTest, shouldReturnValidRangeUsingRightInfinityOpenConstraint) {
    constraints.push_back(TypedConstraint<double>::greater(0, 10));

    checkForConstraints(5, 7);
}

TEST_F(ColumnTest, shouldReturnValidRangeUsingRightInfinityOpenConstraint2) {
    constraints.push_back(TypedConstraint<double>::greater(0, 12));

    checkForConstraints(7, 7);
}

TEST_F(ColumnTest, shouldReturnValidRangeUsingLessInfinityOpenConstraint) {
    constraints.push_back(TypedConstraint<double>::less(0, 12));

    checkForConstraints(0, 4);
}

TEST_F(ColumnTest, shouldReturnValidRangeUsingLessInfinityOpenConstraint2) {
    constraints.push_back(TypedConstraint<double>::less(0, 10));

    checkForConstraints(0, 4);
}

TEST_F(ColumnTest, shouldReturnValidRangesForNotEqual) {
    constraints.push_back(TypedConstraint<double>::notEqual(0, 12));

    checkForConstraints(Tools::vector<IndexRange>(2, /**/
            IndexRange(0, 4),
            IndexRange(7, 7)));
}

TEST_F(ColumnTest, shouldFindGoodRanges) {
    assertThatRangeEquals(5, 12, /**/ 1, 6);
    assertThatRangeEquals(6, 11, /**/ 3, 4);
    assertThatRangeEquals(13, 27, /**/ 7, 7);
    assertThatRangeEquals(-10, 5, /**/ 0, 2);
    assertThatRangeIsEmpty(10, 10);
    assertThatRangeIsEmpty(-14, -4);
    assertThatRangeIsEmpty(24, 28);
}
