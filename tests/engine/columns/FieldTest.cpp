/*
* Copyright 2012 Przemysław Pastuszka
*/

#include <gtest/gtest.h>
#include <src/engine/columns/Fields.h>

class FieldTest : public ::testing::Test {
    public:
    void assertThatIsLessThan(Field<double>& left, Field<double>& right) {
        ASSERT_TRUE(left < right && (right < left == false));
    }
};


TEST_F(FieldTest, shouldOrderFieldsCorrectly) {
    Field<double> smallValue(1);
    Field<double> bigValue(100);
    Field<double> emptyList; emptyList.isEmptyList = true;
    Field<double> nullField; nullField.isNull = true;
    
    assertThatIsLessThan(smallValue, bigValue);
    assertThatIsLessThan(bigValue, emptyList);
    assertThatIsLessThan(emptyList, nullField);
}