/*
* Copyright 2012 Przemysław Pastuszka
*/
#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include <list>
#include "../../src/columns/ListColumn.h"

double initialValues[] = {5, 12, 7, 8, 19, 1};

class NumericalListColumnTest : public ::testing::Test {

    public:
    ListColumn<double> c;

    virtual void SetUp() {
        std::list<double> ls;
        for(int i = 0; i < 6; ++i) {
            ls.push_back(initialValues[i]);
        }

        c.add(&ls, 80);
    }
};

TEST_F(NumericalListColumnTest, shouldContainInitialElements) {
    for(int i = 0; i < 5; ++i) {
        ASSERT_EQ(initialValues[i], c.getField(i) -> value);
        ASSERT_EQ(i + 1, c.getField(i) -> nextFieldId);
        ASSERT_FALSE(c.getField(i) -> isLastListElement());
    }

    ASSERT_EQ(1, c.getField(5) -> value);
    ASSERT_EQ(80, c.getField(5) -> nextFieldId);
    ASSERT_TRUE(c.getField(5) -> isLastListElement());
}
