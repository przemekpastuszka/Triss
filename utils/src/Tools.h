/*
 * Copyright 2012 Przemysław Pastuszka
 */
#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdarg>
#include <vector>
#include <list>
#include <gtest/gtest.h>
#include <algorithm>

class Tools {
    public:
    template <class T>
    static std::vector<T> vector(int count, ...){
        std::vector<T> elements;
        elements.reserve(count);

        va_list argument_list;
        va_start(argument_list, count);
        for(int i = 0; i < count; ++i) {
            elements.push_back(va_arg(argument_list, T));
        }
        va_end(argument_list);
        return elements;
    }

    template <class T>
    static std::list<T> listFrom(std::vector<T> elements) {
        std::list<T> ls(elements.begin(), elements.end());
        return ls;
    }

    template <class T>
    static void assertThatListIsEqualTo(std::list<T>& ls, std::vector<T> elements) {
        ASSERT_EQ(ls.size(), elements.size());

        int i = 0;
        for(typename std::list<T>::iterator it = ls.begin(); it != ls.end(); it++, ++i) {
            ASSERT_EQ(*it, elements[i]);
        }
    }

    template <class T>
    static void assertThatListIsEqualTo(std::list<T>& a, std::list<T>& b) {
        ASSERT_EQ(a.size(), b.size());

        for(typename std::list<T>::iterator left = a.begin(), right = b.begin();
                left != a.end();
                left++, right++) {
            ASSERT_EQ(*left, *right);
        }
    }

    template <class T>
    static void assertThatListContains(std::list<T>& ls, const T& value) {
        ASSERT_TRUE(std::find(ls.begin(), ls.end(), value) != ls.end());
    }
};

#endif /* TOOLS_H_ */
