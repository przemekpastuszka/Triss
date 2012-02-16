#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdio>
#include <list>
#include <string>
#include "../src/Table.h"


class TableTest : public testing::Test {
    public:

    double *numericColumn;
    Table::DataType* schema;
    std::string* stringColumn;

    std::list<double> listColumn[4];
    Table table;

    void setUpListColumn() {
        int listSizes[] = { 2, 1, 1, 3 };
        double listColumnDefinition[][3] = { { 1, 2 }, { 7 }, { 3 }, { 0, 10, 9 } };

        for (int i = 0; i < 4; ++i) {
            insertToList(listColumn[i], listColumnDefinition[i], listSizes[i]);
        }
    }

    void insertToList(std::list<double>& ls, double elements[], int n) {
        for (int i = 0; i < n; ++i) {
            ls.push_back(elements[i]);
        }
    }

    void** generateRow(int i) {
        void** row = new void*[3];
        row[0] = &numericColumn[i];
        row[1] = &listColumn[i];
        row[2] = &stringColumn[i];
        return row;
    }

    void setUpNumericalColumn() {
        numericColumn = new double[4];
        numericColumn[0] = 7;
        numericColumn[1] = 1;
        numericColumn[2] = 8;
        numericColumn[3] = 3;
    }

    void setUpStringColumn(){
        stringColumn = new std::string[4];
        stringColumn[0] = "gazda";
        stringColumn[1] = "mazda";
        stringColumn[2] = "abba";
        stringColumn[3] = "gaździna";
    }

    void setUpSchema() {
        schema = new Table::DataType[3];
        schema[0] = Table::NUMERICAL;
        schema[1] = Table::NUMERICAL_LIST;
        schema[2] = Table::STRING;
    }

    virtual void SetUp() {
        setUpNumericalColumn();
        setUpStringColumn();
        setUpSchema();

        int capacities[] = {10, 10, 10};

        table.init(3, schema, capacities);

        setUpListColumn();
        for(int i = 0; i < 4; ++i) {
            void** row = generateRow(i);
            table.addRow(row);
            delete [] row;
        }

        table.prepareStructure();
    }

    virtual void TearDown() {
        delete [] schema;
        delete [] numericColumn;
        delete [] stringColumn;
    }
};

TEST_F(TableTest, shouldSortElements) {
    double sortedNumericalColumn[] = {1, 3, 7, 8};
    double sortedNumericalListColumn[] = {0, 1, 2, 3, 7, 9, 10};
    std::string sortedStringColumn[] = {"abba", "gazda", "gaździna", "mazda"};

    for(int i = 0; i < 4; ++i) {
        double value = *static_cast<double*>(table.getColumns()[0] -> getFields()[i] -> getValue());
        ASSERT_EQ(sortedNumericalColumn[i], value);
    }
    for(int i = 0; i < 7; ++i) {
        double value = *static_cast<double*>(table.getColumns()[1] -> getFields()[i] -> getValue());
        ASSERT_EQ(sortedNumericalListColumn[i], value);
    }

// FIXME this is kind of problematic. Don't know why yet.
//    for(int i = 0; i < 4; ++i) {
//        std::string value = *static_cast<std::string*>(table.getColumns()[0] -> getFields()[i] -> getValue());
//        ASSERT_EQ(sortedStringColumn[i], value);
//    }
}

TEST_F(TableTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {4, 0, 1, 3};
    int numericalListColumnPointers[] = {6, 2, 1, 0, 3, 2, 5};
    int stringColumnPointers[] = {3, 2, 1, 0};

    for(int i = 0; i < 4; ++i) {
        int nextField = table.getColumns()[0] -> getFields()[i] -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 7; ++i) {
        int nextField = table.getColumns()[1] -> getFields()[i] -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = table.getColumns()[2] -> getFields()[i] -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}
