/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include <prototypes/bob/src/columns/ScalarColumn.h>

class NumericalColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;

    virtual void SetUp() {
        double initialValues[] = {5, 12, 7, 8, 19, 1};
        for(int i = 0; i < 6; ++i) {
            c.add(&initialValues[i], 5 - i);
        }
    }
};

TEST_F(NumericalColumnTest, shouldBeSorted) {
    c.sort();

    double sortedValues[] = {1, 5, 7, 8, 12, 19};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(sortedValues[i], c.getField(i) -> value);
    }
}

TEST_F(NumericalColumnTest, shouldCreateValidMapping) {
    int* mapping = c.getMappingFromCurrentToSortedPositions();

    int validMapping[] = {1, 4, 2, 3, 5, 0};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validMapping[i], mapping[i]);
    }

    delete [] mapping;
}

TEST_F(NumericalColumnTest, shouldUpdateNextIdsUsingMapping) {
    int* mapping = c.getMappingFromCurrentToSortedPositions();
    c.updateNextFieldIdsUsingMapping(NULL, mapping);

    int validNextFields[] = {0, 5, 3, 2, 4, 1};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validNextFields[i], c.getField(i) -> nextFieldId);
    }

    delete [] mapping;
}
