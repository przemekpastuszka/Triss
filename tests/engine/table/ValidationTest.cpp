/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "TableSimpleDataTester.cpp"
#include "src/utils/TrissException.h"

class ValidationTest : public TableSimpleDataTester {
};

TEST_F(ValidationTest, shouldThrowExceptionIfWrongTypeGiven) {
    ASSERT_THROW_WITH_MSG(
            generateColumn(static_cast<Schema::DataType>(20)), 
            "Unknown data type in given schema: 20");
}

TEST_F(ValidationTest, shouldThrowExceptionWhenLimitIsZero) {
    q.limit(0);
    
    ASSERT_THROW_WITH_MSG(
            table.select(q), 
            "Limit cannot be set to 0");
}

TEST_F(ValidationTest, shouldThrowExceptionWhenGivenWrongColumnIdByConstraint) {
    q.addConstraint(TypedConstraint<double>::less(100, 2));
    
    ASSERT_THROW_WITH_MSG(
            table.select(q), 
            "Column id should be in [0, 3) range. Id given: 100");
}

TEST_F(ValidationTest, shouldThrowExceptionWhenGivenWrongColumnIdBySelect) {
    std::list<unsigned int> ls = Tools::listFrom(Tools::vector<unsigned int>(3, 0, 27, 2));
    q.selectColumns(ls);
    
    ASSERT_THROW_WITH_MSG(
            table.select(q), 
            "Column id should be in [0, 3) range. Id given: 27");
}