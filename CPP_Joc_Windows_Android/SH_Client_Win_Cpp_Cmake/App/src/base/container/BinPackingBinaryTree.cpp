#include "BinPackingBinaryTree.h"
#include <base/buffer/StreamBuffer.h>
#include <base/exceptions/DataFormatException.h>
#include <base/math/util/BoolUtil.h>

BinPackingBinaryTree::Node::Node()
	: bounds(),
	parent(nullptr), left(nullptr), right(nullptr)
{
	//void
}

BinPackingBinaryTree::BinPackingBinaryTree(int width, int height)
	: root()
{
	// Add the root node.
	nodesCount++;

	binWidth = width;
	binHeight = height;

	root.bounds.width = width;
	root.bounds.height = height;
}

int BinPackingBinaryTree::getNodesCount() {
	return nodesCount;
}

bool BinPackingBinaryTree::getIsAnythingPacked() {
	if (nodesCount > 0 || (root.bounds.x != 0 || root.bounds.y != 0 || root.bounds.width != binWidth || root.bounds.height != binHeight)) {
		return true;
	}
	
	return false;
}

BinPackingBinaryTree::Node* BinPackingBinaryTree::insert(int width, int height) {
	return insert(&root, width, height);
}

/** Running time is linear to the number of rectangles already packed. Recursively calls itself.
	@return 0 If the insertion didn't succeed. */
BinPackingBinaryTree::Node* BinPackingBinaryTree::insert(Node* node, int width, int height)
{
	// If this node is an internal node, try both leaves for possible space.
	// (The rectangle in an internal node stores used space, the leaves store free space)
	if (node->left || node->right) {
		if (node->left) {
			Node* newNode = insert(node->left, width, height);
			if (newNode) {
				return newNode;
			}
		}
		if (node->right) {
			Node* newNode = insert(node->right, width, height);
			if (newNode) {
				return newNode;
			}
		}

		// Didn't fit into either subtree!
		return 0;
	}

	// This node is a leaf, but can we fit the new rectangle here?
	if (width > node->bounds.width || height > node->bounds.height) {
		// Too bad, no space.
		return 0;
	}

	// The new cell will fit, split the remaining space along the shorter axis,
	// that is probably more optimal.
	int w = node->bounds.width - width;
	int h = node->bounds.height - height;
	
	node->left = new Node();
	node->left->parent = node;
	node->right = new Node();
	node->right->parent = node;
	nodesCount += 2;

	if (w <= h) {
		// Split the remaining space in horizontal direction.
		node->left->bounds.x = node->bounds.x + width;
		node->left->bounds.y = node->bounds.y;
		node->left->bounds.width = w;
		node->left->bounds.height = height;

		node->right->bounds.x = node->bounds.x;
		node->right->bounds.y = node->bounds.y + height;
		node->right->bounds.width = node->bounds.width;
		node->right->bounds.height = h;
	} else {
		// Split the remaining space in vertical direction.
		node->left->bounds.x = node->bounds.x;
		node->left->bounds.y = node->bounds.y + height;
		node->left->bounds.width = width;
		node->left->bounds.height = h;

		node->right->bounds.x = node->bounds.x + width;
		node->right->bounds.y = node->bounds.y;
		node->right->bounds.width = w;
		node->right->bounds.height = node->bounds.height;
	}
	// Note that as a result of the above, it can happen that node->left or node->right
	// is now a degenerate (zero area) rectangle. No need to do anything about it,
	// like remove the nodes as "unnecessary" since they need to exist as children of
	// this node (this node can't be a leaf anymore).

	// This node is now a non-leaf, so shrink its area - it now denotes
	// *occupied* space instead of free space. Its children spawn the resulting
	// area of free space.
	node->bounds.width = width;
	node->bounds.height = height;

	return node;
}

base::Buffer BinPackingBinaryTree::serialiseBinary(int prefixReservedBitesCount) {
	int boundsBitesCount = sizeof(Node::Bounds);
	base::StreamBuffer sBuff{ prefixReservedBitesCount + 4/*nodes_count*/ + nodesCount * (1/*left_right_existence_state_bit_group*/ + boundsBitesCount) };

	sBuff.setOffset(prefixReservedBitesCount);
	sBuff.writeInt(nodesCount);

	// This is a tree traversal that work nicely with serialising and parsing.
	// This traversal processes current node, then left subtree, then right subtree.
	Node* currentNode = &root;
	// This is used for traversing up.
	Node* childSourceNode = nullptr;
	while (currentNode != nullptr) {
		if (childSourceNode == nullptr) {
			// Traversing down.

			// Write node data.
			// Write bounds.
			sBuff.writeBytes((char*)&currentNode->bounds, boundsBitesCount);
			// Write childrens state char.
			char childrenStatus = 0;
			if (currentNode->left != nullptr) {
				childrenStatus = childrenStatus | (1 << 0);
			}
			if (currentNode->right != nullptr) {
				childrenStatus = childrenStatus | (1 << 1);
			}
			sBuff.writeByte(childrenStatus);

			if (currentNode->left != nullptr) {
				currentNode = currentNode->left;
			}else if (currentNode->right != nullptr) {
				currentNode = currentNode->right;
			} else {
				childSourceNode = currentNode;
				currentNode = currentNode->parent;
			}
		} else {
			// Traversing up.
			if (childSourceNode == currentNode->left) {
				// Arrived from the left child.
				if (currentNode->right != nullptr) {
					// Go to the right child.
					currentNode = currentNode->right;
					childSourceNode = nullptr;
				} else {
					// No more children, go to the parent.
					childSourceNode = currentNode;
					currentNode = currentNode->parent;
				}
			} else if (childSourceNode == currentNode->right) {
				childSourceNode = currentNode;
				currentNode = currentNode->parent;
			} else {
				throw LogicException(LOC);
			}
		}
	}

	if (sBuff.getOffset() != sBuff.getBoundsLimit()) {
		throw LogicException(LOC);
	}

	return sBuff.getBuffer();
}

void BinPackingBinaryTree::parseBinary(base::StreamBuffer& sBuff) {
	int boundsBitesCount = sizeof(Node::Bounds);
	
	int expectedNodesCount = sBuff.readInt();

	// This is a tree traversal that work nicely with serialising and parsing.
	// This traversal processes current node, then left subtree, then right subtree.
	Node* currentNode = &root;
	// This is used for traversing up.
	Node* childSourceNode = nullptr;
	while (currentNode != nullptr) {
		if (childSourceNode == nullptr) {
			// Traversing down.

			// Write node data.
			// Write bounds.
			sBuff.readBytes(&currentNode->bounds, boundsBitesCount);
			// Write childrens state char.
			char childrenStatus = sBuff.readByte();

			if (BoolUtil::isBitAtIndex(childrenStatus, 0)) {
				currentNode->left = new Node();
				currentNode->left->parent = currentNode;
				nodesCount++;
			}
			if (BoolUtil::isBitAtIndex(childrenStatus, 1)) {
				currentNode->right = new Node();
				currentNode->right->parent = currentNode;
				nodesCount++;
			}

			if (currentNode->left != nullptr) {
				currentNode = currentNode->left;
			} else if (currentNode->right != nullptr) {
				currentNode = currentNode->right;
			} else {
				childSourceNode = currentNode;
				currentNode = currentNode->parent;
			}
		} else {
			// Traversing up.
			if (childSourceNode == currentNode->left) {
				// Arrived from the left child.
				if (currentNode->right != nullptr) {
					// Go to the right child.
					currentNode = currentNode->right;
					childSourceNode = nullptr;
				} else {
					// No more children, go to the parent.
					childSourceNode = currentNode;
					currentNode = currentNode->parent;
				}
			} else if (childSourceNode == currentNode->right) {
				childSourceNode = currentNode;
				currentNode = currentNode->parent;
			} else {
				throw DataFormatException();
			}
		}
	}

	if (sBuff.getOffset() != sBuff.getBoundsLimit() || nodesCount != expectedNodesCount) {
		throw DataFormatException();
	}
}

BinPackingBinaryTree::~BinPackingBinaryTree() {
	//void
}
