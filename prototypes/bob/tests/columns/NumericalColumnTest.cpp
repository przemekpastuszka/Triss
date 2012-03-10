/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdlib>
#include <prototypes/bob/src/columns/ScalarColumn.h>
#include <prototypes/common/src/Schema.h>

class NumericalColumnTest : public ::testing::Test {
    public:
    ScalarColumn<double> c;
    std::vector<int> mappings[2];

    virtual void SetUp() {
        c.setColumnId(1);

        Schema::DataType schema[] = { Schema::NUMERICAL, Schema::NUMERICAL};
        Row row(std::vector<Schema::DataType>(schema, schema + 2));

        double initialValues[] = {5, 12, 7, 8, 19, 1};
        for(int i = 0; i < 6; ++i) {
            row.set<double>(1, initialValues[i]);
            c.add(row, 5 - i);
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

TEST_F(NumericalColumnTest, shouldFillRowWithGoodValue) {
    c.createMappingFromCurrentToSortedPositions(mappings[1]);
    c.updateNextFieldIdsUsingMapping(mappings[0], mappings[1]);

    Schema::DataType s[] = { Schema::NUMERICAL, Schema::NUMERICAL};
    Schema schema(s, 2);
    Row row(schema);

    c.prepareColumnForQuery();
    c.reduceConstraintsToRange();
    c.markAsMainQueryColumn();

    ASSERT_EQ(5, c.fillRowWithValueAndGetNextFieldId(1, &row));
    ASSERT_EQ(12, row.get<double>(1));
}

TEST_F(NumericalColumnTest, shouldCreateValidMapping) {
    c.createMappingFromCurrentToSortedPositions(mappings[1]);

    int validMapping[] = {1, 4, 2, 3, 5, 0};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validMapping[i], mappings[1][i]);
    }
}

TEST_F(NumericalColumnTest, shouldUpdateNextIdsUsingMapping) {
    c.createMappingFromCurrentToSortedPositions(mappings[1]);
    c.updateNextFieldIdsUsingMapping(mappings[0], mappings[1]);

    int validNextFields[] = {0, 5, 3, 2, 4, 1};
    for(int i = 0; i < 6; ++i) {
        ASSERT_EQ(validNextFields[i], c.getField(i) -> nextFieldId);
    }
}
