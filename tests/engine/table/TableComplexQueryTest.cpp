/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "AbstractTableTest.cpp"

class TableComplexQueryTest : public AbstractTableTest {
    void setUpSchemaAndColumns() {
        Schema :: DataType ss[] = {Schema::NUMERICAL, Schema::NUMERICAL_LIST,
                Schema::STRING};
        schema = new Schema(ss, 3);
        nrOfRows = 7;

        listColumn.clear();
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(4, /**/ 7.0, 9.0, 9.0, 10.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(2, /**/ 21.0, 9.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 4.0, 4.0, 5.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 0.0, -3.0, 0.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(1, /**/ -1.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 1.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ -1000)));

        numericColumn = Tools::vector<double>(7, /**/ 3.141, 2.718, 1.618, 1.414, 1.732, 2.236, 0.577);

        std::string strings[] = { "luxuria", "gula", "avaritia", "acedia", "ira", "invidia", "superbia" };
        stringColumn.assign(strings, strings + 7);
    }
};

TEST_F(TableComplexQueryTest, shouldReturnFirstRow) {
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 3));
    q.addConstraint(TypedConstraint<double>::contains(1, 9));
    
    assertOneRowInResult(0);
}

TEST_F(TableComplexQueryTest, shouldReturnThirdRow) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(1, 5));
    q.addConstraint(TypedConstraint<double>::greaterOrEqual(1, 4));
    
    assertOneRowInResult(2);
}

TEST_F(TableComplexQueryTest, shouldReturnFirstTwoRows) {
    q.addConstraint(TypedConstraint<double>::contains(1, 9));
    result = table. select(q);

    for(int i = 0; i < 2; ++i) {
        ASSERT_TRUE(result -> hasNext());
        Row* row = result -> next();

        Tools::assertThatListContains<double>(row -> get<std::list<double> >(1), 9.0);
    }

    ASSERT_FALSE(result -> hasNext());
}

//TEST_F(TableComplexQueryTest, shouldReturnLastRowUsingNotEqual) {
//    q.addConstraint(TypedConstraint<double>::lessOrEqual(1, 0));
//    q.addConstraint(TypedConstraint<double>::notEqual(0, 1.414));
//    q.addConstraint(TypedConstraint<std::string>::notEqual(2, "ira"));
//    
//    assertOneRowInResult(6);
//}

TEST_F(TableComplexQueryTest, shouldReturnAnswerForConstraintsOnAllColumns) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 1.7));
    q.addConstraint(TypedConstraint<double>::lessOrEqual(1, 0));
    q.addConstraint(TypedConstraint<std::string>::lessOrEqual(2, "s"));
    
    assertOneRowInResult(3);
}

TEST_F(TableComplexQueryTest, shouldReturnAllRowsWhenNoConstraintsGiven) {
    result = table. select(q);

    ASSERT_EQ(7, result -> fetchAll() -> size());
}

