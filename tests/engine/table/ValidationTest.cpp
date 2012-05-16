/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "AbstractTableTest.cpp"
#include "src/utils/TrissException.h"

class ValidationTest : public AbstractTableTest {
    protected:
    virtual void setUpColumns() {
        nrOfRows = 0;
    }
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