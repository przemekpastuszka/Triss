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
        ASSERT_EQ(sortedNumericalColumn[i], getColumn<double>(0) -> getField(i) -> value);
    }
    for(int i = 0; i < 7; ++i) {
        ASSERT_EQ(sortedNumericalListColumn[i], getColumn<double>(1) -> getField(i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedStringColumn[i], getColumn<std::string>(2) -> getField(i) -> value);
    }
}

TEST_F(BobTableInternalStorageTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {4, 0, 1, 3};
    int numericalListColumnPointers[] = {6, 2, 1, 0, 3, 2, 5};
    int stringColumnPointers[] = {3, 2, 1, 0};

    for(int i = 0; i < 4; ++i) {
        int nextField = getColumn<double>(0) -> getField(i) -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 7; ++i) {
        int nextField = getColumn<double>(1) -> getField(i) -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = getColumn<double>(2) -> getField(i) -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}

TEST_F(BobTableInternalStorageTest, shouldStoreInformationAboutListsEnds) {
    bool numericalListEnds[] = {false, false, true, true, true, true, false};

    for(int i = 0; i < 7; ++i) {
        bool isLastElement = getColumn<double>(1) -> getField(i) -> isLastListElement();
        ASSERT_EQ(numericalListEnds[i], isLastElement);
    }
}



