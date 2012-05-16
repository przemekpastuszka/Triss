/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "AbstractTableTest.cpp"

class EmptyTableTest : public AbstractTableTest {
    protected:
    virtual void setUpColumns() {
        nrOfRows = 0;
    }

    virtual void prepareStructure() {
    }
};

TEST_F(EmptyTableTest, shouldReturnNothingForEmptyTable) {
    q.addConstraint(TypedConstraint<double>::lessOrEqual(0, 10));

    assertEmptyResult();
}
