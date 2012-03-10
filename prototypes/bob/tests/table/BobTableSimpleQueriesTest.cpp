/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include "BobTableSimpleDataTester.cpp"


class BobTableSimpleQueriesTest : public BobTableSimpleDataTester {
};


TEST_F(BobTableSimpleQueriesTest, shouldReturnEmptyResultForDisjointConstraints) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 10));
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 11));

    assertEmptyResult();
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnEmptyResultWhenComputedRangesOnEachColumnsDoNotIntersect) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    q.addConstraint(TypedConstraint<std::string>::equals(2, "abba"));

    assertEmptyResult();
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnEmptyResult) {
    q.addConstraint(TypedConstraint<double>::equals(0, 11));

    assertEmptyResult();
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnLastRowForContainsConstraint) {
    q.addConstraint(TypedConstraint<double>::contains(1, 10));
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_EQ(3, row -> get<double>(0));
    Tools::assertThatListIsEqualTo<double>(row -> get<std::list<double> >(1), Tools::vector<double>(3, /**/ 0.0, 10.0, 9.0));
    ASSERT_EQ("gaździna", row -> get<std::string>(2));
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnOneRowWithLimit) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.limit(1);
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_GE(row -> get<double>(0), 7);
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnOneRowWithTwoConstraintsOnOneColumn) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 7));
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    ASSERT_EQ(7, row -> get<double>(0));
    Tools::assertThatListIsEqualTo<double>(row -> get<std::list<double> >(1), Tools::vector<double>(2, /**/ 1.0, 2.0));
    ASSERT_EQ("gazda", row -> get<std::string>(2));
}

TEST_F(BobTableSimpleQueriesTest, shouldReturnTwoRows) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 7));
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_TRUE(result -> hasNext());
    ASSERT_GE(result -> next() -> get<double>(0), 7);

    ASSERT_FALSE(result -> hasNext());
}

