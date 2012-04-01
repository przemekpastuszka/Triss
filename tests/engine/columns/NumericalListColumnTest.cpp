/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <list>
#include <src/engine/columns/ListColumn.h>
#include <src/utils/Tools.h>

double initialValues[] = {5, 12, 7, 8, 19, 1};

class NumericalListColumnTest : public ::testing::Test {

    public:
    ListColumn<double> c;

    virtual void SetUp() {
        c.setColumnId(1);

        Schema::DataType schema[] = { Schema::NUMERICAL, Schema::NUMERICAL_LIST};
        Row row(std::vector<Schema::DataType>(schema, schema + 2));

        std::list<double> ls;
        for(int i = 0; i < 6; ++i) {
            ls.push_back(initialValues[i]);
        }

        row.set<std::list<double> >(1, ls);
        c.add(row, 80);
    }
};

TEST_F(NumericalListColumnTest, shouldContainInitialElements) {
    for(int i = 0; i < 5; ++i) {
        ASSERT_EQ(initialValues[i], c.getField(i) -> value);
        ASSERT_EQ(i + 1, c.getField(i) -> nextFieldId);
        ASSERT_FALSE(c.getField(i) -> isLastListElement());
    }

    ASSERT_EQ(1, c.getField(5) -> value);
    ASSERT_EQ(80, c.getField(5) -> nextFieldId);
    ASSERT_TRUE(c.getField(5) -> isLastListElement());
}

TEST_F(NumericalListColumnTest, shouldFillRowWithGoodValues) {
    Schema::DataType s[] = { Schema::NUMERICAL, Schema::NUMERICAL_LIST};
    Schema schema(s, 2);
    Row row(schema);

    ColumnQueryState* state = c.prepareColumnForQuery();
    c.reduceConstraintsToRange(state);
    c.markAsMainQueryColumn(state);

    ASSERT_EQ(80, c.fillRowWithValueAndGetNextFieldId(3, &row, state, true));
    Tools::assertThatListIsEqualTo(row.get<std::list<double> >(1), Tools::vector<double>(3, /**/ 8.0, 19.0, 1.0));

    delete state;
}
