/*
* Copyright 2012 Michał Rychlik
*/
#include <gtest/gtest.h>
#include <list>
#include <src/common/Query.h>
#include <src/common/Constraint.h>

class QueryTest : public testing::Test {
    public:
    Query query;

    int getNumberOfConstraints() {
        return query.constraints.size();
    }

    bool isColumnSelected(int i) {
        return std::find(query.selectedColumns.begin(), query.selectedColumns.end(), i) !=
               query.selectedColumns.end();
    }

    virtual void SetUp() {
        std::list<int> columns;
        columns.push_back(1);
        columns.push_back(2);
        query.selectColumns(columns);
        query.addConstraint(TypedConstraint<double>::lessOrEqual(0, 27));
        query.addConstraint(TypedConstraint<std::string>::equals(2, "zzz"));
        query.limit(10);
    }

};

TEST_F(QueryTest, shouldOverwriteSelectedColumns) {
    std::list<int> newColumns;
    newColumns.push_back(2);
    newColumns.push_back(3);
    query.selectColumns(newColumns);
    ASSERT_TRUE(isColumnSelected(2));
    ASSERT_TRUE(isColumnSelected(3));
    ASSERT_FALSE(isColumnSelected(1));
}

TEST_F(QueryTest, shouldIncreaseNumberOfConstraints) {
    ASSERT_EQ(2, getNumberOfConstraints());
    query.addConstraint(TypedConstraint<std::string>::contains(2, "baba"));
    ASSERT_EQ(3, getNumberOfConstraints());
}

TEST_F(QueryTest, shouldOverwriteLimit) {
    ASSERT_EQ(10, query.getLimit());
    query.limit(11);
    ASSERT_EQ(11, query.getLimit());
}
