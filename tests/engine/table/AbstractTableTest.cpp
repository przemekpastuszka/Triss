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
#include <src/utils/Tools.h>
#include <src/engine/columns/Fields.h>
#include <src/engine/columns/TypedColumn.h>

class AbstractTableTest : public testing::Test {
    protected:
    std::vector<std::list<double> > listColumn;
    std::vector<double> numericColumn;
    std::vector<std::string> stringColumn;
    std::vector<ColumnDesc> schema;
    int nrOfRows;

    public:
    Table table;
    Result* result;
    Query q;

    template <class T>
    Field<T>* getField(int columnId, int fieldId) {
        TypedColumn<T>* column = static_cast<TypedColumn<T>*>(table.columns[columnId]);
        return column -> getField(fieldId);
    }

    AbstractTableTest() {
        result = NULL;
    }

    virtual void setUpColumns() = 0;

    virtual void SetUp() {
        schema.clear();
        schema.push_back(ColumnDesc("a", Schema::NUMERICAL));
        schema.push_back(ColumnDesc("b", Schema::NUMERICAL_LIST));
        schema.push_back(ColumnDesc("c", Schema::STRING));
        
        setUpColumns();

        table.setSchema(schema);

        for(int i = 0; i < nrOfRows; ++i) {
            Row* row = table.createTableRow();
            if(i < numericColumn.size()) {
                row -> set<double>(0, numericColumn[i]);
            }
            if(i < listColumn.size()) {
                row -> set<std::list<double> >(1, listColumn[i]);
            }
            if(i < stringColumn.size()) {
                row -> set<std::string >(2, stringColumn[i]);
            }
            table.addRow(*row);
            delete row;
        }

        table.prepareStructure();

        std::list<int> ls = Tools::listFrom(Tools::vector<int>(3, 0, 1, 2));
        q.selectColumns(ls);
    }

    virtual void TearDown() {
        if(result != NULL) {
            delete result;
            result = NULL;
        }
    }

    void assertOneRowInResult(int rowId) {
        result = table. select(q);
        
        ASSERT_TRUE(result -> hasNext());
        Row* row = result -> next();
        ASSERT_FALSE(result -> hasNext());

        assertThatRowIsEqualTo(row, rowId);
    }
    
    void assertThatRowIsEqualTo(Row* row, int index) {
        ASSERT_EQ(numericColumn[index], row -> get<double>(0));
        Tools::assertThatListIsEqualTo(listColumn[index], row -> get<std::list<double> >(1));
        ASSERT_EQ(stringColumn[index], row -> get<std::string>(2));
    }

    void assertEmptyResult() {
        result = table.select(q);

        ASSERT_FALSE(result -> hasNext());
        std::list<Row*>* results = result -> fetchAll();
        ASSERT_EQ(0, results -> size());
    }
};
