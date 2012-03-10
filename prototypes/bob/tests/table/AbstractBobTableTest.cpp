/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdio>
#include <list>
#include <string>
#include <prototypes/bob/src/BobTable.h>
#include <prototypes/common/src/Constraint.h>
#include <utils/src/Tools.h>


class AbstractBobTableTest : public testing::Test {
    protected:
    std::vector<std::list<double> > listColumn;
    std::vector<double> numericColumn;
    std::vector<std::string> stringColumn;
    Schema* schema;

    public:
    BobTable *bobTable;
    Result* result;
    Query q;

    AbstractBobTableTest() {
        result = NULL;
    }

    virtual void setUpSchemaAndColumns() = 0;

    virtual void SetUp() {
        setUpSchemaAndColumns();

        bobTable = new BobTable(*schema);
        Table *table = bobTable;

        for(int i = 0; i < numericColumn.size(); ++i) {
            Row row(*schema);
            row.set<double>(0, numericColumn[i]);
            row.set<std::list<double> >(1, listColumn[i]);
            row.set<std::string >(2, stringColumn[i]);
            table -> addRow(row);
        }

        table -> prepareStructure();

        std::list<int> ls = Tools::listFrom(Tools::vector<int>(3, 0, 1, 2));
        q.selectColumns(ls);
    }

    virtual void TearDown() {
        delete schema;
        delete bobTable;

        if(result != NULL) {
            delete result;
            result = NULL;
        }
    }

    void assertThatRowIsEqualTo(Row* row, int index) {
        ASSERT_EQ(numericColumn[index], row -> get<double>(0));
        Tools::assertThatListIsEqualTo(listColumn[index], row -> get<std::list<double> >(1));
        ASSERT_EQ(stringColumn[index], row -> get<std::string>(2));
    }

    void assertEmptyResult() {
        result = bobTable -> select(q);

        ASSERT_FALSE(result -> hasNext());
        std::list<Row*> results = result -> fetchAll();
        ASSERT_EQ(0, results.size());
    }
};
