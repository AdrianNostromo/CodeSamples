#pragma once

class RenderingDataStructures2D {
public:
    //asdA9;// Refactor to RenderingDataStructures3D style of more structs;
    struct Vertex {
        float x;
        float y;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        float u;
        float v;

        // Use a float because byte and int are not allowed as attributes.
        // asdx;// try o use a char or something smaller (1 byte) to allow for extra vertex attributes.
        float _textureIndex;
    };
    struct Quad {
        Vertex bl;
        Vertex br;

        Vertex tr;
        Vertex tl;
    };

};
