#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include "../../src/columns/NumericalColumn.h"

class NumericalColumnTest : public ::testing::Test, public NumericalColumn {

    virtual void SetUp() {
        init(6);
        double initialValues[] = {5, 12, 7, 8, 19, 1};
        for(int i = 0; i < 6; ++i) {
            addField(initialValues[i], 5 - i);
        }
    }
};

TEST_F(NumericalColumnTest, shouldBeSorted) {
    sort();

    double sortedValues[] = {1, 5, 7, 8, 12, 19};
    for(int i = 0; i < 6; ++i) {
        NumericalField* field = static_cast<NumericalField*>(getFields()[i]);
        ASSERT_EQ(sortedValues[i], field -> value);
    }
}

TEST_F(NumericalColumnTest, shouldCreateValidMapping) {
    int* mapping = getMappingFromCurrentToSortedPositions();

    int validMapping[] = {1, 4, 2, 3, 5, 0};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validMapping[i], mapping[i]);
    }

    delete [] mapping;
}

TEST_F(NumericalColumnTest, shouldUpdateNextIdsUsingMapping) {
    int* mapping = getMappingFromCurrentToSortedPositions();
    updateNextFieldIdsUsingMapping(mapping);

    int validNextFields[] = {0, 5, 3, 2, 4, 1};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validNextFields[i], getFields()[i] -> nextFieldId);
    }

    delete [] mapping;
}
