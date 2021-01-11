#pragma once

#include <base/gh.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>
#include <sstream>
#include <base/exceptions/DataFormatException.h>

namespace graphics {
class GLTypes {
    pub struct Vect2 {
        pub float x;
        pub float y;

        pub explicit Vect2()
            : x(0.0f), y(0.0f)
        {}
        pub Vect2(const Vect2& src)
            : x(src.x), y(src.y)
        {}
        pub explicit Vect2(float x, float y)
            : x(x), y(y)
        {}

        pub void set(float x, float y) {
            this->x = x;
            this->y = y;
        }
        pub void set(Vector2& pos) {
            this->x = pos.x;
            this->y = pos.y;
        }
    };
    pub struct Vect3 {
        pub float x;
        pub float y;
        pub float z;

        pub explicit Vect3()
            : x(0.0f), y(0.0f), z(0.0f)
        {}
        pub Vect3(const Vect3& src)
            : x(src.x), y(src.y), z(src.z)
        {}
        pub explicit Vect3(float x, float y, float z)
            : x(x), y(y), z(z)
        {}

        pub void set(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        pub void set(Vector3& pos) {
            this->x = pos.x;
            this->y = pos.y;
            this->z = pos.z;
        }
    };
    pub struct Col4 {
        // [0, 255]
        pub unsigned char r;
        // [0, 255]
        pub unsigned char g;
        // [0, 255]
        pub unsigned char b;
        // [0, 255]
        pub unsigned char a;

        pub explicit Col4()
            : r(0), g(0), b(0), a(0)
        {}
        pub Col4(const Col4& src)
            : r(src.r), g(src.g), b(src.b), a(src.a)
        {}
        pub explicit Col4(float r, float g, float b, float a)
            : r(static_cast<unsigned char>(r * 255.0f)), g(static_cast<unsigned char>(g * 255.0f)), b(static_cast<unsigned char>(b * 255.0f)), a(static_cast<unsigned char>(a * 255.0f))
        {}
        pub explicit Col4(std::string hex) {
            if (hex.at(0) == '#') {
                hex = hex.erase(0, 1);
            }
            if (hex.length() != 8) {
                throw DataFormatException();
            }

            // These ints are required because std::istringstream will not parse the value correctly if the output is the class "unsigned char" params.
            unsigned int intR;
            unsigned int intG;
            unsigned int intB;
            unsigned int intA;
            std::istringstream(hex.substr(0, 2)) >> std::hex >> intR;
            std::istringstream(hex.substr(2, 2)) >> std::hex >> intG;
            std::istringstream(hex.substr(4, 2)) >> std::hex >> intB;
            std::istringstream(hex.substr(6, 2)) >> std::hex >> intA;

            r = intR;
            g = intG;
            b = intB;
            a = intA;
        }

        pub void set(float r, float g, float b, float a) {
            this->r = static_cast<unsigned char>(r * 255.0f);
            this->g = static_cast<unsigned char>(g * 255.0f);
            this->b = static_cast<unsigned char>(b * 255.0f);
            this->a = static_cast<unsigned char>(a * 255.0f);
        }

        pub void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };

    pub class Vertex_2DGenericTexture {
        pub Vect2 pos;

        pub Col4 diffuseColor;

        pub Vect2 uv;

        // This is managed by the renderer automatically.
        // Use a float because byte and int are not allowed as attributes.
        // This index is from
        // asdx;// try o use a char or something smaller (1 byte) to allow for extra vertex attributes.
        pub int a_diffuseTextureIndex = 0;

        pub explicit Vertex_2DGenericTexture()
            : pos(), diffuseColor(), uv(), a_diffuseTextureIndex(0)
        {}
        pub explicit Vertex_2DGenericTexture(Vect2 pos, Col4 diffuseColor, Vect2 uv, int a_diffuseTextureIndex)
            : pos(pos), diffuseColor(diffuseColor), uv(uv), a_diffuseTextureIndex(a_diffuseTextureIndex)
        {}
    };
    pub class Vertex_3DGenericTexture {
        pub Vect3 pos;

        pub Vect3 normal;

        pub Col4 diffuseColor;

        pub unsigned int a_modelTransformIndex;

        pub Vect2 uv;

        // This is managed by the renderer automatically.
        // Use a float because byte and int are not allowed as attributes.
        // This index is from
        // asdx;// try o use a char or something smaller (1 byte) to allow for extra vertex attributes.
        pub int a_diffuseTextureIndex = 0;

        pub explicit Vertex_3DGenericTexture()
            : pos(), normal(), diffuseColor(), a_modelTransformIndex(0u),
            uv(), a_diffuseTextureIndex(0)
        {}
        pub explicit Vertex_3DGenericTexture(Vect3 pos, Vect3 normal, Col4 diffuseColor, unsigned int a_modelTransformIndex,
            Vect2 uv, int a_diffuseTextureIndex)
            : pos(pos), normal(normal), diffuseColor(diffuseColor), a_modelTransformIndex(a_modelTransformIndex),
            uv(uv), a_diffuseTextureIndex(a_diffuseTextureIndex)
        {}
    };
    pub class Vertex_3DGenericColor {
        pub Vect3 pos;

        pub Vect3 normal;

        pub Col4 diffuseColor;

        pub unsigned int a_modelTransformIndex;

        pub explicit Vertex_3DGenericColor()
            : pos(), normal(), diffuseColor(), a_modelTransformIndex(0u)
        {}
        pub explicit Vertex_3DGenericColor(Vect3 pos, Vect3 normal, Col4 diffuseColor, unsigned int a_modelTransformIndex)
            : pos(pos), normal(normal), diffuseColor(diffuseColor), a_modelTransformIndex(a_modelTransformIndex)
        {}
    };

    pub class Quad_2DGenericTexture {
        pub Vertex_2DGenericTexture bl;
        pub Vertex_2DGenericTexture br;
        pub Vertex_2DGenericTexture tr;
        pub Vertex_2DGenericTexture tl;

        pub explicit Quad_2DGenericTexture()
            : bl(), br(), tr(), tl()
        {
            //void
        }
        pub explicit Quad_2DGenericTexture(Vertex_2DGenericTexture bl, Vertex_2DGenericTexture br, Vertex_2DGenericTexture tr, Vertex_2DGenericTexture tl)
            : bl(bl), br(br), tr(tr), tl(tl)
        {
            //void
        }

        // No destructor because this is used as void* that is sent to VRam;
    };

};
};
