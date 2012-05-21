/*
 * Copyright 2012 Przemysław Pastuszka
 */
#include <gtest/gtest.h>
#include <triss/src/utils/TrissException.h>

using namespace std;


class TrissExceptionTest : public :: testing :: Test {
};

typedef std::string str;

TEST_F(TrissExceptionTest, shouldReturnCorrectMessage) {
    TrissException ex = TrissException() << "c++ is " << 4 << " dummies";

    ASSERT_EQ(str(ex.what()), str("c++ is 4 dummies"));
}

