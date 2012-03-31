/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdio>
#include <list>
#include <string>
#include <src/engine/Table.h>
#include <src/common/Constraint.h>
#include <utils/src/Tools.h>
#include <src/engine/columns/Fields.h>
#include <src/engine/columns/TypedColumn.h>

class AbstractTableTest : public testing::Test {
    protected:
    std::vector<std::list<double> > listColumn;
    std::vector<double> numericColumn;
    std::vector<std::string> stringColumn;
    Schema* schema;

    public:
    Table *table;
    Result* result;
    Query q;

    template <class T>
    Field<T>* getField(int columnId, int fieldId) {
        TypedColumn<T>* column = static_cast<TypedColumn<T>*>(table -> columns[columnId]);
        return column -> getField(fieldId);
    }

    AbstractTableTest() {
        result = NULL;
    }

    virtual void setUpSchemaAndColumns() = 0;

    virtual void SetUp() {
        setUpSchemaAndColumns();

        Table *table = new Table(*schema);

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
        delete table;

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
        result = table -> select(q);

        ASSERT_FALSE(result -> hasNext());
        std::list<Row*>* results = result -> fetchAll();
        ASSERT_EQ(0, results -> size());
    }
};
