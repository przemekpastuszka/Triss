/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <list>
#include <src/engine/columns/ListColumn.h>
#include <src/utils/Tools.h>
#include <src/engine/Table.h>
#include <src/common/Schema.h>

double initialValues[] = {5, 12, 7, 8, 19, 1};

class NumericalListColumnTest : public ::testing::Test {
    public:
    ListColumn<double> c;
    TableRow* row;
    std::vector<ColumnDesc> schema;

    virtual void SetUp() {
        c.setColumnId(1);
        c.setGlobalPosition(0);

        schema.clear();
        schema.push_back(ColumnDesc("a", Schema::NUMERICAL));
        schema.push_back(ColumnDesc("b", Schema::NUMERICAL_LIST));
        Table table;
        table.setSchema(schema);
        row = table.createTableRow();

        std::list<double> ls;
        for(int i = 0; i < 6; ++i) {
            ls.push_back(initialValues[i]);
        }

        row -> set<std::list<double> >(1, ls);
        c.add(*row, 80);
    }

    virtual void TearDown() {
        delete row;
    }
};

TEST_F(NumericalListColumnTest, shouldContainInitialElements) {
    for(int i = 0; i < 5; ++i) {
        ASSERT_EQ(initialValues[i], c.getField(i) -> value);
        ASSERT_EQ(i + 1, c.getField(i) -> nextFieldId);
        ASSERT_FALSE(c.getField(i) -> isLastElement);
    }
    ASSERT_TRUE(c.getField(5) -> isLastElement);
    ASSERT_EQ(1, c.getField(5) -> value);
    ASSERT_EQ(80, c.getField(5) -> nextFieldId);
}

TEST_F(NumericalListColumnTest, shouldFillRowWithGoodValues) {
    ColumnQueryState* state = c.prepareColumnForQuery();
    c.reduceConstraintsToRangeSet(state);
    c.markAsMainQueryColumn(state);

    ASSERT_EQ(80, c.fillRowWithValueAndGetNextFieldId(3, 3, row, state, schema, true));
    Tools::assertThatListIsEqualTo(row -> get<std::list<double> >(1), Tools::vector<double>(3, /**/ 8.0, 19.0, 1.0));

    delete state;
}
