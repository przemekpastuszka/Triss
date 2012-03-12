/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "BobTableSimpleDataTester.cpp"

class BobTableInternalStorageTest : public BobTableSimpleDataTester {
};

TEST_F(BobTableInternalStorageTest, shouldSortElements) {
    double sortedNumericalColumn[] = {1, 3, 7, 8};
    double sortedNumericalListColumn[] = {0, 1, 2, 3, 7, 9, 10};
    std::string sortedStringColumn[] = {"astaroth", "belial", "belphegor", "merihem"};

    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedNumericalColumn[i], getField<double>(bobTable, 0, i) -> value);
    }
    for(int i = 0; i < 7; ++i) {
        ASSERT_EQ(sortedNumericalListColumn[i], getField<double>(bobTable, 1, i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedStringColumn[i], getField<std::string>(bobTable, 2, i) -> value);
    }
}

TEST_F(BobTableInternalStorageTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {4, 0, 1, 3};
    int numericalListColumnPointers[] = {6, 2, 1, 0, 3, 2, 5};
    int stringColumnPointers[] = {3, 2, 1, 0};

    for(int i = 0; i < 4; ++i) {
        int nextField = getField<double>(bobTable, 0, i) -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 7; ++i) {
        int nextField = getField<double>(bobTable, 1, i) -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = getField<std::string>(bobTable, 2, i) -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}

TEST_F(BobTableInternalStorageTest, shouldStoreInformationAboutListsEnds) {
    bool numericalListEnds[] = {false, false, true, true, true, true, false};

    for(int i = 0; i < 7; ++i) {
        bool isLastElement = getField<double>(bobTable, 1, i) -> isLastListElement();
        ASSERT_EQ(numericalListEnds[i], isLastElement);
    }
}



