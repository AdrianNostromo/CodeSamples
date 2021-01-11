#pragma once

#include <base/gh.h>

class Vector2Char {
    pub typedef signed char type;

    pub type x;
    pub type y;
    
    //pub Vector2Char (Vector2Char const&) = delete;
    pub Vector2Char(Vector2Char &&) = default;
    pub Vector2Char& operator=(Vector2Char const&) = delete;
    pub Vector2Char& operator=(Vector2Char &&) = default;

    pub explicit Vector2Char();
    pub explicit Vector2Char(type x, type y);
    pub explicit Vector2Char(Vector2Char const& src);

    pub void idt();

    pub void set(type x, type y);
    pub void set(Vector2Char const& src);

    pub void replaceEachOther(Vector2Char& other);

    pub void setMin(type x, type y);
    pub void setMin(Vector2Char const& o);
    pub void setMax(type x, type y);
    pub void setMax(Vector2Char const& o);

    pub void add(type x, type y);

    pub bool equals(type x, type y);
    pub bool equals(Vector2Char const& o);

    pub ~Vector2Char();
};

