#pragma once

#include <base/gh.h>
#include <base/buffer/Buffer.h>
#include <base/buffer/StreamBuffer.h>
#include <base/memory/SharedPointer.h>

class BinPackingBinaryTree {pub dCtor(BinPackingBinaryTree);
    pub class Node {pub dCtor(Node);
		pub class Bounds {
			// The top-left coordinate of the rectangle.
			pub short x;
			pub short y;

			// The dimension of the rectangle.
			pub short width;
			pub short height;

			pub explicit Bounds()
				: x(0), y(0), width(0), height(0)
			{}
		};

		// Use a container class for memcpy to, from save data.
		Bounds bounds;

		pub Node* parent;

		// Left and right child. We don't really distinguish which is which, so these could
		// as well be child1 and child2.
		pub Node* left;
		pub Node* right;

		pub explicit Node();
	};

	priv Node root;
	priv int nodesCount = 0;

	// The total size of the bin we started with.
	priv int binWidth;
	priv int binHeight;

    pub explicit BinPackingBinaryTree(int width, int height);

	pub int getNodesCount();
	// This is used to detect if a texture is used (if nothing is packed, the texture asset is not saved.
	pub bool getIsAnythingPacked();

	/// Inserts a new rectangle of the given size into the bin.
	/** Running time is linear to the number of rectangles that have been already packed.
		@return A pointer to the node that stores the newly added rectangle, or 0 if it didn't fit. */
	pub Node* insert(int width, int height);

	/// Inserts a new rectangle in the subtree rooted at the given node.
	priv Node* insert(Node* node, int width, int height);

	pub base::Buffer serialiseBinary(int prefixReservedBitesCount);
	pub void parseBinary(base::StreamBuffer& sBuff);

    pub virtual ~BinPackingBinaryTree();
};
