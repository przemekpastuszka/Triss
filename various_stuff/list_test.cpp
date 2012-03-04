#include <gtest/gtest.h>
#include "list.h"
#include "Tools.h"

class ListTest : public testing::Test {
protected:
    List<int> list;
    virtual void SetUp()
    {
        for(int i = 0;i < 4;++i) {
            list.pushBack(i);
        }
    }

    void assertThatListIsEmpty() {
        ASSERT_TRUE(list.isEmpty());
        ASSERT_EQ(0, list.getSize());
    }

    void assertThatListIsEqualTo(std::vector<int> elements) {
        assertListSizeEqualTo(elements.size());
        assertElementsEqualityUsingGet(elements);
        assertFrontAndBackEquality(elements);
        assertIteratorElementsEquality(elements);
    }

    void assertListSizeEqualTo(int count) {
        ASSERT_FALSE(list.isEmpty());
        ASSERT_EQ(count, list.getSize());
    }

    void assertElementsEqualityUsingGet(std::vector<int>& elements) {
        for(int i = 0; i < elements.size(); ++i) {
            ASSERT_EQ(elements[i], list.getElementAt(i));
        }
    }

    void assertFrontAndBackEquality(std::vector<int>& elements) {
        ASSERT_EQ(elements[0], list.getFront());
        ASSERT_EQ(elements[elements.size() - 1], list.getBack());
    }

    void assertIteratorElementsEquality(std::vector<int>& elements) {
        List<int> :: Iterator it = list.getIterator();

        for(int i = 0; i < elements.size(); ++i) {
            ASSERT_TRUE(it.hasNext());
            ASSERT_EQ(elements[i], it.next());
        }
        ASSERT_FALSE(it.hasNext());
    }
};

TEST_F(ListTest, shouldContainInitialElements) {
    assertThatListIsEqualTo(Tools::vector<int>(4, /**/ 0, 1, 2, 3));
}

TEST_F(ListTest, shouldShortenTheListAfterPops) {
    ASSERT_EQ(0, list.popFront());
    assertThatListIsEqualTo(Tools::vector<int>(3, /**/ 1, 2, 3));

    ASSERT_EQ(3, list.popBack());
    assertThatListIsEqualTo(Tools::vector<int>(2, /**/ 1, 2));

    ASSERT_EQ(2, list.popBack());
    assertThatListIsEqualTo(Tools::vector<int>(1, /**/ 1));

    ASSERT_EQ(1, list.popFront());
    assertThatListIsEmpty();
}

TEST_F(ListTest, shouldAddElementsWithPush) {
	list.pushFront(7);
	assertThatListIsEqualTo(Tools::vector<int>(5, /**/ 7, 0, 1, 2, 3));

	list.pushBack(22);
	assertThatListIsEqualTo(Tools::vector<int>(6, /**/ 7, 0, 1, 2, 3, 22));

	list.pushBack(13);
	assertThatListIsEqualTo(Tools::vector<int>(7, /**/ 7, 0, 1, 2, 3, 22, 13));
}

TEST_F(ListTest, shouldAddElementsWithInsert) {
	list.insert(24, 2);
	assertThatListIsEqualTo(Tools::vector<int>(5, /**/ 0, 1, 24, 2, 3));
}

TEST_F(ListTest, shouldRemoveElementAtPosition) {
	list.remove(2);
	assertThatListIsEqualTo(Tools::vector<int>(3, /**/ 0, 1, 3));
}

TEST_F(ListTest, shouldMakeVariousOperationCorrectly) {
	for(int i = 0; i < 4; ++i) {
		list.popFront();
	}
	SetUp();
	list.remove(2);
	list.insert(20, 2);
	list.popBack();
	list.pushBack(30);
	list.insert(10, 1);
	list.remove(2);

	assertThatListIsEqualTo(Tools::vector<int>(4, /**/ 0, 10, 20, 30));
}
