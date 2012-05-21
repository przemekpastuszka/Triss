/*
* Copyright 2012 Michał Rychlik
*/
#include <gtest/gtest.h>
#include <list>
#include <string>

#include <triss/src/common/Result.h>
#include <triss/src/engine/Table.h>

class ResultTest : public testing::Test {
    public:
    Result* result;
    std::vector<ColumnDesc> schema;

    virtual void SetUp() {
        std::list<Row*>* result_rows = new std::list<Row*>();
        schema.clear();
        schema.push_back(ColumnDesc("a", Schema::NUMERICAL));
        schema.push_back(ColumnDesc("b", Schema::NUMERICAL_LIST));
        schema.push_back(ColumnDesc("c", Schema::STRING));

        Table table;
        table.setSchema(schema);

        TableRow* row1 = table.createTableRow();
        row1->set<double>(0, 27);
        std::list<double> ls1;
        ls1.push_back(2);
        ls1.push_back(3);
        row1->set<std::list<double> >(1, ls1);
        row1->set<std::string>(2, "abba");
        result_rows -> push_back(row1);

        TableRow* row2 = table.createTableRow();
        row2->set<double>(0, 13);
        std::list<double> ls2;
        ls2.push_back(13);
        ls2.push_back(56);
        row2->set<std::list<double> >(1, ls2);
        row2->set<std::string>(2, "mama");
        result_rows -> push_back(row2);
        result = new Result(schema, result_rows);
    }

    virtual void TearDown() {
        delete result;
    }

};

TEST_F(ResultTest, shouldFetchAllResults) {
    std::list<Row*>* result_rows = result-> fetchAll();
    ASSERT_EQ(2, result_rows -> size());
}


TEST_F(ResultTest, shouldSetHasNextToFalseAfterFetch) {
    result->fetchAll();
    ASSERT_FALSE(result->hasNext());
}

TEST_F(ResultTest, shouldIterateOverRows) {
    std::list<Row*> result_rows;
    while(result->hasNext()) {
        Row* row = result->next();
        result_rows.push_back(row);
    }
    ASSERT_EQ(2, result_rows.size());
}

TEST_F(ResultTest, shouldNotHasNextForEmpty) {
    std::list<Row*>* result_rows = new std::list<Row*>();
    Result* result2 = new Result(schema, result_rows);
    ASSERT_FALSE(result2 -> hasNext());
    delete result2;
}
