/*
* Copyright 2012 Michał Rychlik
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <list>
#include "../src/AliceQuery.h"

class AliceQueryTest : public testing::Test {
    public:
    AliceQuery query;

    virtual void SetUp() {
        std::list<int> columns;
        columns.push_back(1);
        columns.push_back(2);
        query.selectColumns(columns);
    }

};

TEST_F(AliceQueryTest, shouldOverwriteSelectedColumns) {
    std::list<int> newColumns;
    newColumns.push_back(2);
    newColumns.push_back(3);
    query.selectColumns(newColumns);
    ASSERT_TRUE(query.isColumnSelected(2));
    ASSERT_TRUE(query.isColumnSelected(3));
    ASSERT_FALSE(query.isColumnSelected(1));
}
