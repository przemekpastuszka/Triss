/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "TableSimpleDataTester.cpp"

class TableSimpleQueriesTest : public TableSimpleDataTester {
};


TEST_F(TableSimpleQueriesTest, shouldReturnEmptyResultForDisjointConstraints) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 10));
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 11));

    assertEmptyResult();
}

TEST_F(TableSimpleQueriesTest, shouldReturnEmptyResultWhenComputedRangesOnEachColumnsDoNotIntersect) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    q.addConstraint(TypedConstraint<std::string>::equals(2, "astaroth"));

    assertEmptyResult();
}

TEST_F(TableSimpleQueriesTest, shouldReturnEmptyResult) {
    q.addConstraint(TypedConstraint<double>::equals(0, 11));

    assertEmptyResult();
}

TEST_F(TableSimpleQueriesTest, shouldReturnLastRowForContainsConstraint) {
    q.addConstraint(TypedConstraint<double>::contains(1, 10));
    
    assertOneRowInResult(3);
}

TEST_F(TableSimpleQueriesTest, shouldReturnSecondRowForContainsConstraint) {
    q.addConstraint(TypedConstraint<double>::contains(1, 7));
    
    assertOneRowInResult(1);
}

TEST_F(TableSimpleQueriesTest, shouldReturnOneRowWithLimit) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.limit(1);
    result = table.select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_GE(row -> get<double>(0), 7);
}

TEST_F(TableSimpleQueriesTest, shouldReturnOneRowWithTwoConstraintsOnOneColumn) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    
    assertOneRowInResult(0);
}

TEST_F(TableSimpleQueriesTest, shouldReturnTwoRows) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    result = table.select(q);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_FALSE(result -> hasNext());
}

TEST_F(TableSimpleQueriesTest, shouldReturnResultWithNewSchema) {
    q.addConstraint(TypedConstraint<double>::equals(0, 7));
    std::list<unsigned int> ls = Tools::listFrom(Tools::vector<unsigned int>(5, /**/
            0, 2, 2, 0, 1));
    q.selectColumns(ls);
    result = table.select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_EQ(numericColumn[0], row -> get<double>(0));
    ASSERT_EQ(numericColumn[0], row -> get<double>(3));
    
    ASSERT_EQ(stringColumn[0], row -> get<std::string>(1));
    ASSERT_EQ(stringColumn[0], row -> get<std::string>(2));
}

