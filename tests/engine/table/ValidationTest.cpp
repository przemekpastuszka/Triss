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
