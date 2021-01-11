#pragma once

#include <base/gh.h>

class Vector2 {
    pub typedef float type;

    pub type x;
    pub type y;

    //pub Vector2(Vector2 const& src) = delete;
    pub Vector2(Vector2 &&) = default;
    pub Vector2& operator=(Vector2 const&) = delete;
    pub Vector2& operator=(Vector2 &&) = default;

    pub explicit Vector2();
    pub explicit Vector2(type x, type y);
    pub explicit Vector2(Vector2 const& src);

    pub Vector2* set(type x, type y);
    pub Vector2* set(Vector2 const& val);

    pub Vector2* add(type x, type y);
    pub Vector2* add(Vector2 const& val);
    pub Vector2* add(Vector2 const* val);

    pub Vector2* mul(type val);

    pub type len();

    pub Vector2* idt();

    pub Vector2* nor();
    pub Vector2* clipLength(type maxLength);

    pub bool equals(Vector2& o);
    pub bool equals(type x, type y);

    pub virtual ~Vector2();
};
