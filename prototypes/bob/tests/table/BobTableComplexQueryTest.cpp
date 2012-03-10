/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include "AbstractBobTableTest.cpp"


class BobTableComplexQueryTest : public AbstractBobTableTest {
    void setUpSchemaAndColumns() {
        Schema :: DataType ss[] = {Schema::NUMERICAL, Schema::NUMERICAL_LIST,
                Schema::STRING};
        schema = new Schema(ss, 3);

        listColumn.clear();
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(4, /**/ 7.0, 9.0, 9.0, 10.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(2, /**/ 21.0, 9.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 4.0, 4.0, 4.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 0.0, -3.0, 0.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(1, /**/ -1.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 1.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ -1000)));

        numericColumn = Tools::vector<double>(7, /**/ 3.141, 2.718, 1.618, 1.414, 1.732, 2.236, 0.577);

        std::string strings[] = { "luxuria", "gula", "avaritia", "acedia", "ira", "invidia", "superbia" };
        stringColumn.assign(strings, strings + 7);
    }
};

TEST_F(BobTableComplexQueryTest, shouldReturnFirstRow) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 3));
    q.addConstraint(TypedConstraint<double>::contains(1, 9));
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    assertThatRowIsEqualTo(row, 0);
}

TEST_F(BobTableComplexQueryTest, shouldReturnFirstTwoRows) {
    q.addConstraint(TypedConstraint<double>::contains(1, 9));
    result = bobTable -> select(q);

    for(int i = 0; i < 2; ++i) {
        ASSERT_TRUE(result -> hasNext());
        Row* row = result -> next();

        Tools::assertThatListContains<double>(row -> get<std::list<double> >(1), 9.0);
    }

    ASSERT_FALSE(result -> hasNext());
}

TEST_F(BobTableComplexQueryTest, shouldReturnAnswerForConstraintsOnAllColumns) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 1.7));
    q.addConstraint(TypedConstraint<double>::lessOrEqual(1, 0));
    q.addConstraint(TypedConstraint<std::string>::lessOrEqual(2, "s"));
    result = bobTable -> select(q);

    ASSERT_TRUE(result -> hasNext());
    Row* row = result -> next();
    ASSERT_FALSE(result -> hasNext());

    assertThatRowIsEqualTo(row, 3);
}

TEST_F(BobTableComplexQueryTest, shouldReturnAllRowsWhenNoConstraintsGiven) {
    result = bobTable -> select(q);

    ASSERT_EQ(7, result -> fetchAll().size());
}

