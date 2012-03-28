/*
* Copyright 2012 Michał Rychlik
*/
#include "AliceTableSimpleDataTester.cpp"

class AliceTableSimpleQueriesTest : public AliceTableSimpleDataTester {
};


TEST_F(AliceTableSimpleQueriesTest, shouldReturnEmptyResultForDisjointConstraints) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 10));
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 11));

    assertEmptyResult();
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnEmptyResultWhenComputedRangesOnEachColumnsDoNotIntersect) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    q.addConstraint(TypedConstraint<std::string>::equals(2, "astaroth"));

    assertEmptyResult();
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnEmptyResult) {
    q.addConstraint(TypedConstraint<double>::equals(0, 11));

    assertEmptyResult();
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnLastRowForContainsConstraint) {
    q.addConstraint(TypedConstraint<double>::contains(1, 10));
    result = aliceTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    assertThatRowIsEqualTo(row, 3);
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnSecondRowForContainsConstraint) {
    q.addConstraint(TypedConstraint<double>::contains(1, 7));
    result = aliceTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    assertThatRowIsEqualTo(row, 1);
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnOneRowWithLimit) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.limit(1);
    result = aliceTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_GE(row -> get<double>(0), 7);
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnOneRowWithTwoConstraintsOnOneColumn) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    result = aliceTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    assertThatRowIsEqualTo(row, 0);
}

TEST_F(AliceTableSimpleQueriesTest, shouldReturnTwoRows) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    result = aliceTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_FALSE(result -> hasNext());
}

