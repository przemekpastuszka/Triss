/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "TableSimpleDataTester.cpp"

class TableInternalStorageWithNullsTest : public TableSimpleDataTester {
    void setUpColumns() {
        TableSimpleDataTester :: setUpColumns();
        numericColumn.resize(2);
        listColumn.resize(3);
    }
};

TEST_F(TableInternalStorageWithNullsTest, shouldSortElements) {
    double sortedNumericalColumn[] = {1, 7};
    double sortedNumericalListColumn[] = {1, 2, 3, 7};
    std::string sortedStringColumn[] = {"astaroth", "belial", "belphegor", "merihem"};

    for(int i = 0; i < 2; ++i) {
        ASSERT_EQ(sortedNumericalColumn[i], getField<double>(0, i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedNumericalListColumn[i], getField<double>(1, i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedStringColumn[i], getField<std::string>(2, i) -> value);
    }
}

TEST_F(TableInternalStorageWithNullsTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {11, 8};
    int numericalListColumnPointers[] = {1, 14, 13, 16};
    int stringColumnPointers[] = {10, 5, 15, 4};

    for(int i = 0; i < 2; ++i) {
        int nextField = getField<double>(0, i) -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = getField<double>(1, i) -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = getField<std::string>(2, i) -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}

TEST_F(TableInternalStorageWithNullsTest, shouldStoreInformationAboutListsEnds) {
    bool numericalListEnds[] = {false, true, true, true};

    for(int i = 0; i < 4; ++i) {
        bool isLastElement = getField<double>(1, i) -> isLastElement;
        ASSERT_EQ(numericalListEnds[i], isLastElement);
    }
}

TEST_F(TableInternalStorageWithNullsTest, shouldReturnRowWithNulls) {
    q.addConstraint(TypedConstraint<std::string>::lessOrEqual(2, "belphegor"));
    result = table.select(q);

    std :: list<Row*>* rows = result -> fetchAll();
    ASSERT_EQ(3, rows -> size());
    std :: list<Row*> :: iterator it = rows -> begin();
    std :: advance(it, 2);

    ASSERT_EQ("belphegor", (*it) -> get<std::string>(2));
    ASSERT_TRUE((*it) -> isNull(0));
    ASSERT_TRUE((*it) -> isNull(1));

    ASSERT_FALSE(result -> hasNext());
}
