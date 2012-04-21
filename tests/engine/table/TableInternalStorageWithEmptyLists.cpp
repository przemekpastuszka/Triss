/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "TableSimpleDataTester.cpp"

class TableInternalStorageWithEmptyListsTest : public TableSimpleDataTester {
    void setUpColumns() {
        TableSimpleDataTester :: setUpColumns();
        listColumn[1] = listColumn[2] = std::list<double>();
    }
};

TEST_F(TableInternalStorageWithEmptyListsTest, shouldPlaceEmptyListsAtTheEnd) {
    for(int i = 0; i < 5; ++i) {
        ASSERT_FALSE(getField<double>(1, i) -> isEmptyList);
    }
    ASSERT_TRUE(getField<double>(1, 5) -> isEmptyList);
    ASSERT_TRUE(getField<double>(1, 6) -> isEmptyList);
}

TEST_F(TableInternalStorageWithEmptyListsTest, shouldCalculateCorrectNumberOfEmptyLists) {
    ASSERT_EQ(2, getColumn<double>(1) -> getNumberOfEmptyLists());
    ASSERT_EQ(0, getColumn<double>(0) -> getNumberOfEmptyLists());
}

TEST_F(TableInternalStorageWithEmptyListsTest, shouldReturnRowWithEmptyLists) {
    q.addConstraint(TypedConstraint<double>::notContains(1, 0));
    q.addConstraint(TypedConstraint<double>::notContains(1, 1));
    result = table.select(q);

    for(int i = 0; i < 2; ++i) {
        ASSERT_TRUE(result -> hasNext());
        Row *row = result -> next();
        ASSERT_TRUE(row -> get<std::list<double> >(1).empty());
    }
    ASSERT_FALSE(result -> hasNext());
}

TEST_F(TableInternalStorageWithEmptyListsTest, shouldReturnEmptyResult) {
    q.addConstraint(TypedConstraint<double>::notContains(1, 0));
    q.addConstraint(TypedConstraint<double>::notContains(1, 1));
    q.addConstraint(TypedConstraint<double>::greater(1, 7));
    result = table.select(q);

    ASSERT_FALSE(result -> hasNext());
}