#pragma once

#include <base/gh.h>

class Vector2Int {
    pub typedef int type;

    pub int x;
    pub int y;
    
    //pub Vector2Int (Vector2Int const&) = delete;
    pub Vector2Int(Vector2Int &&) = default;
    pub Vector2Int& operator=(Vector2Int const&) = delete;
    pub Vector2Int& operator=(Vector2Int &&) = default;

    pub explicit Vector2Int();
    pub explicit Vector2Int(int x, int y);
    pub explicit Vector2Int(Vector2Int const& src);

    pub void idt();

    pub void set(int x, int y);
    pub void set(Vector2Int const& src);

    pub void replaceEachOther(Vector2Int& other);

    pub void setMin(int x, int y);
    pub void setMin(Vector2Int const& o);
    pub void setMax(int x, int y);
    pub void setMax(Vector2Int const& o);

    pub void add(int x, int y);

    pub bool equals(int x, int y);
    pub bool equals(Vector2Int const& o);

    pub ~Vector2Int();
};

