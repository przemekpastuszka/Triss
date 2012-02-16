#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <list>
#include <string>
#include "../src/Table.h"

Table::DataType schema[] = { Table::NUMERICAL, Table::NUMERICAL_LIST,
        Table::STRING };

double numericColumn[] = { 7, 1, 8, 3 };
std::string stringColumn[] = { "gazda", "mazda", "abba", "gaździna" };

std::list<double>* listColumn;
double listColumnDefinition[][3] = { { 1, 2 }, { 7 }, { 3 }, { 0, 10, 9 } };
int listSizes[] = { 2, 1, 1, 3 };

class TableTest : public testing::Test {
    Table *table;

    void setUpListColumn() {
        listColumn = new std::list<double>[4];
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

    virtual void SetUp() {
        int capacities[] = {10, 10, 10};

        table = new Table(3, schema);
        table -> initCapacities(capacities);

        setUpListColumn();
        for(int i = 0; i < 4; ++i) {
            void** row = generateRow(i);
            table -> addRow(row);
            delete [] row;
        }

        table -> prepareStructure();
    }

    virtual void TearDown() {
        delete[] listColumn;
        delete table;
    }
};

TEST_F(TableTest, shouldContainInitialElements) {

}
