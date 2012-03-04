/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <cstdio>
#include <list>
#include <string>
#include "../src/BobTable.h"
#include "../../common/src/Constraint.h"

Schema::DataType schema[] = { Schema::NUMERICAL, Schema::NUMERICAL_LIST,
        Schema::STRING };

double numericColumn[] = { 7, 1, 8, 3 };
std::string stringColumn[] = { "gazda", "mazda", "abba", "gaździna" };

std::list<double>* listColumn;
double listColumnDefinition[][3] = { { 1, 2 }, { 7 }, { 3 }, { 0, 10, 9 } };
int listSizes[] = { 2, 1, 1, 3 };

class BobTableTest : public testing::Test {
    public:
    BobTable *bobTable;
    Query q;

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

    virtual void SetUp() {
        Schema s(schema, 3);
        bobTable = new BobTable(s);

        Table* table = bobTable;

        setUpListColumn();
        for(int i = 0; i < 4; ++i) {
            Row row(s);
            row.set<double>(0, numericColumn[i]);
            row.set<std::list<double> >(1, listColumn[i]);
            row.set<std::string >(2, stringColumn[i]);
            table -> addRow(row);
        }

        table -> prepareStructure();

//        std::list<int> ls = Tools::listFrom(Tools::vector<int>(3, 0, 1, 2));
//        q.selectColumns(ls);
    }

    virtual void TearDown() {
        delete[] listColumn;
        delete bobTable;
    }

    void assertEmptyResult() {
        Result* result = bobTable -> select(q);

        ASSERT_FALSE(result -> hasNext());
        std::list<Row*> results = result -> fetchAll();
        ASSERT_EQ(0, results.size());

        delete result;
    }
};

//TEST_F(BobTableTest, shouldReturnEmptyResultForDisjointConstraints) {
//    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 10));
//    q.addConstraint(TypedConstraint<double>::greaterOrEqual(0, 11));
//
//    assertEmptyResult();
//}
//
//TEST_F(BobTableTest, shouldReturnEmptyResult) {
//    q.addConstraint(TypedConstraint<double>::equals(0, 11));
//
//    assertEmptyResult();
//}

TEST_F(BobTableTest, shouldSortElements) {
    double sortedNumericalColumn[] = {1, 3, 7, 8};
    double sortedNumericalListColumn[] = {0, 1, 2, 3, 7, 9, 10};
    std::string sortedStringColumn[] = {"abba", "gazda", "gaździna", "mazda"};

    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedNumericalColumn[i], bobTable -> getColumn<double>(0) -> getField(i) -> value);
    }
    for(int i = 0; i < 7; ++i) {
        ASSERT_EQ(sortedNumericalListColumn[i], bobTable -> getColumn<double>(1) -> getField(i) -> value);
    }
    for(int i = 0; i < 4; ++i) {
        ASSERT_EQ(sortedStringColumn[i], bobTable -> getColumn<std::string>(2) -> getField(i) -> value);
    }
}

TEST_F(BobTableTest, shouldSetUpValidPointers) {
    int numericalColumnPointers[] = {4, 0, 1, 3};
    int numericalListColumnPointers[] = {6, 2, 1, 0, 3, 2, 5};
    int stringColumnPointers[] = {3, 2, 1, 0};

    for(int i = 0; i < 4; ++i) {
        int nextField = bobTable -> getColumn<double>(0) -> getField(i) -> nextFieldId;
        ASSERT_EQ(numericalColumnPointers[i], nextField);
    }
    for(int i = 0; i < 7; ++i) {
        int nextField = bobTable -> getColumn<double>(1) -> getField(i) -> nextFieldId;
        ASSERT_EQ(numericalListColumnPointers[i], nextField);
    }
    for(int i = 0; i < 4; ++i) {
        int nextField = bobTable -> getColumn<double>(2) -> getField(i) -> nextFieldId;
        ASSERT_EQ(stringColumnPointers[i], nextField);
    }
}

TEST_F(BobTableTest, shouldStoreInformationAboutListsEnds) {
    bool numericalListEnds[] = {false, false, true, true, true, true, false};

    for(int i = 0; i < 7; ++i) {
        bool isLastElement = bobTable -> getColumn<double>(1) -> getField(i) -> isLastListElement();
        ASSERT_EQ(numericalListEnds[i], isLastElement);
    }
}
