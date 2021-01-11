#pragma once

#include <base/gh.h>

class Vector1 {
    pub typedef float type;

    pub type x;

    //Vector1(Vector1 const& src) = delete;
    pub Vector1(Vector1 &&) = default;
    pub Vector1& operator=(Vector1 const&) = delete;
    pub Vector1& operator=(Vector1 &&) = default;

    pub explicit Vector1();
    pub explicit Vector1(type x);
    pub explicit Vector1(Vector1 const& src);

    pub Vector1* set(type x);
    pub Vector1* set(Vector1 const& val);

    pub type len();

    pub Vector1* idt();

    pub Vector1* nor();
    pub Vector1* clipLength(type maxLength);

    pub bool equals(Vector1& o);
    pub bool equals(type x, type y);

    pub virtual ~Vector1();
};
