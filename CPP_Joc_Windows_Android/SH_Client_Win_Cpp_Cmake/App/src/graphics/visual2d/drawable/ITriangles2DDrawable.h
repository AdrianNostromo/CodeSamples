#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IGeometryDrawable2D.h>
#include <base/math/Vector2.h>
#include <graphics/visual/Color.h>

class Texture;

class ITriangles2DDrawable : virtual public IGeometryDrawable2D {
	// This is a convenience class, NOT a gl_type, don't use gl types in it.
	// This is also used because there is no texture index provided in these vertices, the texture index is one for the entire Triangles2DDrawable.
	pub class LocalVertex {pub dCtor(LocalVertex);
		pub Vector2 pos;

		pub Vector2 uv;

		// Use a tint color to allow for linear gradients.
		pub Color tintColor;

		pub explicit LocalVertex() {};
		pub explicit LocalVertex(Vector2 pos, Vector2 uv, Color tintColor)
			: pos(std::move(pos)), uv(std::move(uv)), tintColor(std::move(tintColor))
		{};
		pub explicit LocalVertex(Vector2 pos, Vector2 uv, Color* tintColor)
			: pos(std::move(pos)), uv(std::move(uv)), tintColor(*tintColor)
		{};
	};

	pub virtual const Texture* getTexture() = 0;
	pub virtual void setTexture(Texture* texture) = 0;

    pub ~ITriangles2DDrawable() override = default;
};
