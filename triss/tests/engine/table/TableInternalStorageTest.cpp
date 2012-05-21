/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "TableSimpleDataTester.cpp"

class TableInternalStorageTest : public TableSimpleDataTester {
};

TEST_F(TableInternalStorageTest, shouldSortElements) {
    double sortedNumericalColumn[] = {1, 3, 7, 8};
    double sortedNumericalListColumn[] = {0, 1, 2, 3, 7, 9, 10};
    std::string sortedStringColumn[] = {"astaroth", "belial", "belphegor", "merihem"};

    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedNumericalColumn[i], getField<double>(0, i) -> value);
    }
    for(int i = 0; i < 7; ++i) {
        ASSERT_EQ(sortedNumericalListColumn[i], getField<double>(1, i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedStringColumn[i], getField<std::string>(2, i) -> value);
    }
}

TEST_F(TableInternalStorageTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {12, 8, 9, 11};
    int numericalListColumnPointers[] = {6, 2, 16, 15, 18, 17, 5};
    int stringColumnPointers[] = {7, 6, 5, 4};

    for(int i = 0; i < 4; ++i) {
        int nextField = getField<double>(0, i) -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 7; ++i) {
        int nextField = getField<double>(1, i) -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = getField<std::string>(2, i) -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}

TEST_F(TableInternalStorageTest, shouldStoreInformationAboutListsEnds) {
    bool numericalListEnds[] = {false, false, true, true, true, true, false};

    for(int i = 0; i < 7; ++i) {
        bool isLastElement = getField<double>(1, i) -> isLastElement;
        ASSERT_EQ(numericalListEnds[i], isLastElement);
    }
}



