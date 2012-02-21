/*
 * Copyright 2012 Michał Rychlik
 */
#include <gtest/gtest.h>
#include <sparsehash/dense_hash_map>
#include <cstdlib>
#include <ext/hash_map>
#include <iostream>
#include "Tools.h"

#define ASSERT_THAT_HASH_MAP_HAS_KEYS(count, ...) INVOKE_WITH_VAR_ARGS(char*, assertThatHashMapHasKeys, count, __VA_ARGS__)
#define ASSERT_THAT_HASH_MAP_HAS_CORRECT_VALUES(count, ...) INVOKE_WITH_VAR_ARGS(char*, assertThatHashMapHasCorrectValues, 2 * count, __VA_ARGS__)

struct eqstr {
  bool operator()(const char* s1, const char* s2) const {
    return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
  }
};

class DenseHashMapTest : public :: testing :: Test {
protected:
    google::dense_hash_map<const char*, int, __gnu_cxx::hash<const char*>,
                           eqstr> hashMap;

    virtual void SetUp() {
        hashMap.set_empty_key(NULL);
        hashMap["january"] = 31;
        hashMap["february"] = 29;
        hashMap["march"] = 31;
        hashMap["april"] = 30;
        hashMap["may"] = 31;
        hashMap["june"] = 30;
        hashMap["july"] = 31;
        hashMap["august"] = 31;
        hashMap["september"] = 30;
        hashMap["october"] = 31;
        hashMap["november"] = 30;
        hashMap["december"] = 31;
    }

    void assertThatHashMapIsEmpty() {
        ASSERT_TRUE(hashMap.empty());
        ASSERT_EQ(0, hashMap.size());
    }

    void assertThatHashMapHasKeys(int count, char** elements) {
        for (int i = 0; i < count; ++i) {
           ASSERT_FALSE(hashMap.find(elements[i]) == hashMap.end());
        }
        delete [] elements;
    }

    void assertHashMapSizeEqualTo(int count) {
        ASSERT_FALSE(hashMap.empty());
        ASSERT_EQ(count, hashMap.size());
    }

    void assertThatHashMapHasCorrectValues(int count, char** elements) {
        for (int i = 0; i < count; i += 2) {
            ASSERT_FALSE(hashMap.find(elements[i]) == hashMap.end());
            ASSERT_EQ(hashMap[elements[i]], atoi(elements[i + 1]));
        }
        delete [] elements;
    }
};

TEST_F(DenseHashMapTest, shouldContainInitialKeys) {
    ASSERT_THAT_HASH_MAP_HAS_KEYS(12, /**/ "january", "february", "march", "april",
                                      "may", "june", "july", "august",
                                      "september", "october", "november",
                                      "december");
}

TEST_F(DenseHashMapTest, shouldContainCorrectInitialValues) {
    ASSERT_THAT_HASH_MAP_HAS_CORRECT_VALUES(12, /**/ "january", "31",
                                               "february", "29",
                                               "march", "31",
                                               "april", "30",
                                               "may", "31",
                                               "june", "30",
                                               "july", "31",
                                               "august", "31",
                                               "september", "30",
                                               "october", "31",
                                               "november", "30",
                                               "december", "31");
}


TEST_F(DenseHashMapTest, shouldShortenHashMapAfterDeletes) {
    hashMap.set_deleted_key("");
    hashMap.erase("february");
    assertHashMapSizeEqualTo(11);
}

TEST_F(DenseHashMapTest, shouldAddElement) {
    hashMap["new_month"] = 30;
    assertHashMapSizeEqualTo(13);
    ASSERT_THAT_HASH_MAP_HAS_KEYS(1, "new_month");
    ASSERT_THAT_HASH_MAP_HAS_CORRECT_VALUES(1, "new_month", "30");
}

TEST_F(DenseHashMapTest, shouldOverwriteElementWithSameKey) {
    ASSERT_EQ(hashMap["february"], 29);
    hashMap["february"] = 28;
    ASSERT_EQ(hashMap["february"], 28);
    assertHashMapSizeEqualTo(12);
}

TEST_F(DenseHashMapTest, shouldRemoveAllElements) {
    hashMap.clear();
    assertThatHashMapIsEmpty();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
