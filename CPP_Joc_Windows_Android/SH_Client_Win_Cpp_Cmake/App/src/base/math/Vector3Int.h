#pragma once

#include <base/gh.h>

class Vector3Int {
	pub typedef int type;

	pub type x;
	pub type y;
	pub type z;

    //pub Vector3Int (Vector3Int const&) = delete;
	pub Vector3Int(Vector3Int &&) = default;
	pub Vector3Int& operator=(Vector3Int const&) = delete;
	pub Vector3Int& operator=(Vector3Int &&) = default;
	pub explicit Vector3Int();

	pub explicit Vector3Int(Vector3Int const& o);
	pub explicit Vector3Int(Vector3Int const* o);
	pub explicit Vector3Int(type x, type y, type z);

	pub void set(Vector3Int const& src);
	pub void set(type x, type y, type z);
	
	pub void add(Vector3Int const& src);
	pub void add(type x, type y, type z);

	pub void min(type x, type y, type z);
	pub void min(Vector3Int const& o);
	pub void max(type x, type y, type z);
	pub void max(Vector3Int const& o);
	
	pub void setMin(type x, type y, type z);
	pub void setMin(Vector3Int const& o);
	pub void setMax(type x, type y, type z);
	pub void setMax(Vector3Int const& o);

	pub bool equals(Vector3Int const& o);
	pub bool equals(type x, type y, type z);
	
	pub ~Vector3Int();
};

