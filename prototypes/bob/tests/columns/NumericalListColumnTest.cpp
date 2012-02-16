#define TRISS_TEST

#include <gtest/gtest.h>
#include <cstdarg>
#include "../../src/columns/NumericalListColumn.h"

double initialValues[] = {5, 12, 7, 8, 19, 1};

class NumericalListColumnTest : public ::testing::Test, public NumericalListColumn {
    virtual void SetUp() {
        init(6);

        std::list<double> ls;
        for(int i = 0; i < 6; ++i) {
            ls.push_back(initialValues[i]);
        }

        addField(ls, 80);
    }
};

TEST_F(NumericalListColumnTest, shouldContainInitialElements) {
    for(int i = 0; i < 5; ++i) {
        NumericalListField* field = static_cast<NumericalListField*>(getFields()[i]);
        ASSERT_EQ(initialValues[i], field -> value);
        ASSERT_EQ(i + 1, field -> nextFieldId);
        ASSERT_FALSE(field -> lastListElement);
    }

    NumericalListField* field = static_cast<NumericalListField*>(getFields()[5]);
    ASSERT_EQ(1, field -> value);
    ASSERT_EQ(80, field -> nextFieldId);
    ASSERT_TRUE(field -> lastListElement);
}
