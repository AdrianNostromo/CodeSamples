#pragma once

#include <base/gh.h>

class Vector1Int {
    pub typedef int type;

    pub int x;

    //pub Vector1Int (Vector1Int const&) = delete;
    pub Vector1Int(Vector1Int &&) = default;
    pub Vector1Int& operator=(Vector1Int const&) = delete;
    pub Vector1Int& operator=(Vector1Int &&) = default;

    pub explicit Vector1Int();
    pub explicit Vector1Int(int x);
    pub explicit Vector1Int(Vector1Int const& src);

    pub void idt();

    pub void set(int x);
    pub void set(Vector1Int const& src);

    pub void replaceEachOther(Vector1Int& other);

    pub void setMin(int x);
    pub void setMin(Vector1Int const& o);
    pub void setMax(int x);
    pub void setMax(Vector1Int const& o);

    pub void add(int x);

    pub bool equals(int x);
    pub bool equals(Vector1Int const& o);

    pub ~Vector1Int();
};

