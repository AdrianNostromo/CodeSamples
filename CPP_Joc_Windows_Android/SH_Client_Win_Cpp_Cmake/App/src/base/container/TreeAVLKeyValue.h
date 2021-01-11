#pragma once

#include <base/gh.h>
#include <base/exceptions/OutOfRangeException.h>
#include <cassert>
#include <base/container/util/IContainerNode.h>
#include <base/container/util/IContainerNodeWrapper.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/AlreadyExistsException.h>
#include <base/exceptions/EntryNotFoundException.h>

template <typename K, typename V>
class TreeAVLKeyValue {pub dCtor(TreeAVLKeyValue);
	pub class Node : virtual public IContainerNode {pub dCtor(Node);
		// The node can be externally managed (eg. Ho control when the memory is freed as is for playing_sounds).
		pub bool isNodeExternallyManuallyDeleted;

		pub TreeAVLKeyValue* list = nullptr;

		// The current algorithm requires a parent pointer.
		pub Node* parent = nullptr;

		// Only change with setLeft.
		pub Node* left = nullptr;
		// Only change with setRight.
		pub Node* right = nullptr;

		pub K key;
		pub V value;

		pub int height = 1;

		pub explicit Node(bool isNodeExternallyManuallyDeleted, K& key, V& value) noexcept;

		pub template <typename... Vs>
		explicit Node(bool isNodeExternallyManuallyDeleted, K& key, Vs&&... args) noexcept;

		pub void setLeft(Node* newLeftNode);
		pub void setRight(Node* newRightNode);

		pub int getBallance();

		pub void remove() override;

		pub ~Node() noexcept override;
	};

	// This is true when this tree is used for memory management and to avoid memory management calls because that would create endless loops.
	priv bool bypassMemoryManagement;

	priv size_t nodeSizeBytes;

	priv Node* _root = nullptr;

	priv int _count = 0;
	priv size_t _sizeBytes = 0;

	// Enumeration start;
	// The idea with this is to allow for multiple entries removal,adding from a node callback function call. Also removes the need for a mark_for_removal system.
	// This allows for manual enumarions.
	pub bool isEnumeratingInProgress = false;
	pub typename TreeAVLKeyValue<K, V>::Node* nextEnumeratingNode = nullptr;

	pub bool isEnumeratingInProgressB = false;
	pub typename TreeAVLKeyValue<K, V>::Node* nextEnumeratingNodeB = nullptr;
	// Enumeration end;

	pub explicit TreeAVLKeyValue(bool bypassMemoryManagement=false);

	pub Node* getRoot();

	pub Node* getMinValueNode();
	pub Node* getMinValueNode(Node* subTreeRootNode);
	pub Node* getMaxValueNode();
	pub Node* getMaxValueNode(Node* subTreeRootNode);

	pub Node* getNode(K& key, bool mustExist=false);
	pub Node* getNodeB(K key, bool mustExist=false);
	
	pub V& getValueReference(K& key);
	pub V& getValueReferenceB(K key);
	
	pub V* getValuePointer(K& key, bool mustExist=false);
	pub V* getValuePointerB(K key, bool mustExist=false);
	
	pub V getValueDirect(K& key, V fallbackValue);
	pub V getValueDirectB(K key, V fallbackValue);
	
	pub bool containsReference(K& key);
	pub bool containsDirect(K& key);

	pub int count();
	pub int sizeBytes();

	// Rotation ilustrations: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/?ref=lbp
	pub Node* rotateLeft(Node* z);
	pub Node* rotateLeftWithSubRightRotation(Node* z);
	pub Node* rotateRight(Node* z);
	pub Node* rotateRightWithSubLeftRotation(Node* z);

	pub Node* insertDirect(K& key, V value);
	pub Node* insertDirectB(K key, V value);
	pub Node* insertReference(K& key, V& value);
	pub Node* insertReferenceB(K key, V& value);

	pub template <typename... Args>
	Node* insertEmplace(K& key, Args&&... args);
	pub template <typename... Args>
	Node* insertEmplaceB(K key, Args&&... args);

	pub Node* insertNode(Node* node);
	// Returns nullptr if _root should be set.
	pub Node* findInsertionParentNode(K& key);
	pub void ballanceBranchUpward(Node* startBottomNode);

	pub bool removeKeyReference(K& key, bool mustExist=false);
	pub bool removeKeyDirect(K key, bool mustExist=false);

	pub void removeNode(Node* node, bool doAvlBallancing = true);
	prot void onNodeRemovePre(Node* node);

	pub void clear();

	// Enumeration start;
	pub template<typename F>
	void enumerateInOrderReference(F& cb);
	pub template<typename F>
	void enumerateInOrderDirect(F cb);

	pub void startManualEnumeration();
	pub void endManualEnumeration();

	pub void startManualEnumerationB();
	pub void endManualEnumerationB();
	// Enumeration end;

	priv inline int max(int a, int b);
	priv int getNodeHeight(Node* node);
	priv int getNodeBalance(Node* node);

	pub ~TreeAVLKeyValue();
};

template <typename K, typename V>
TreeAVLKeyValue<K, V>::Node::Node(bool isNodeExternallyManuallyDeleted, K& key, V& value) noexcept
	: isNodeExternallyManuallyDeleted(isNodeExternallyManuallyDeleted), key(key), value(value)
{
	//void
}

template <typename K, typename V>
template <typename... Vs>
TreeAVLKeyValue<K, V>::Node::Node(bool isNodeExternallyManuallyDeleted, K& key, Vs&&... args) noexcept
	: isNodeExternallyManuallyDeleted(isNodeExternallyManuallyDeleted), key(key), value(std::forward<Vs>(args)...)
{
	//void
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::Node::setLeft(Node* newLeftNode) {
	// Don't cleanup the parent of the old entry because the caller assumes no cleanup.

	left = newLeftNode;

	if (left != nullptr) {
		left->parent = this;
	}
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::Node::setRight(Node* newRightNode) {
	// Don't cleanup the parent of the old entry because the caller assumes no cleanup.

	right = newRightNode;

	if (right != nullptr) {
		right->parent = this;
	}
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::Node::getBallance() {
	int leftHeight = (left != nullptr) ? left->height : 0;
	int rightHeight = (right != nullptr) ? right->height : 0;

	return rightHeight - leftHeight;
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::Node::remove() {
	if (list == nullptr || height > 10000/*invalid_node_probably*/) {
		throw LogicException(LOC);
	}

	list->removeNode(this);
}

template <typename K, typename V>
TreeAVLKeyValue<K, V>::Node::~Node() noexcept {
	//void
}

template <typename K, typename V>
TreeAVLKeyValue<K, V>::TreeAVLKeyValue(bool bypassMemoryManagement)
	: bypassMemoryManagement(bypassMemoryManagement)
{
	nodeSizeBytes = sizeof(Node);
	_sizeBytes = sizeof(TreeAVLKeyValue<K, V>);
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getRoot() {
	return _root;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getMinValueNode() {
	return getMinValueNode(_root);
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getMinValueNode(Node* subTreeRootNode) {
	TreeAVLKeyValue<K, V>::Node* currentNode = subTreeRootNode;

	/* loop down to find the leftmost leaf */
	while (currentNode != nullptr && currentNode->left != nullptr) {
		currentNode = currentNode->left;
	}

	return currentNode;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getMaxValueNode() {
	return getMaxValueNode(_root);
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getMaxValueNode(Node* subTreeRootNode) {
	TreeAVLKeyValue<K, V>::Node* currentNode = subTreeRootNode;

	/* loop down to find the rightmost leaf */
	while (currentNode != nullptr && currentNode->right != nullptr) {
		currentNode = currentNode->right;
	}

	return currentNode;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getNode(K& key, bool mustExist) {
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr && key != subTreeRootNode->key) {
		subTreeRootNode = (key > subTreeRootNode->key) ? subTreeRootNode->right : subTreeRootNode->left;
	}

	if (mustExist && subTreeRootNode == nullptr) {
		throw EntryNotFoundException();
	}

	return subTreeRootNode;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::getNodeB(K key, bool mustExist) {
	return getNode(key, mustExist);
}

template <typename K, typename V>
V& TreeAVLKeyValue<K, V>::getValueReference(K& key) {
	Node* node = getNode(key, false/*mustExist*/);
	if (node != nullptr) {
		return node->value;
	}

	throw EntryNotFoundException();
}

template <typename K, typename V>
V& TreeAVLKeyValue<K, V>::getValueReferenceB(K key) {
	return getValueReference(key);
}

template <typename K, typename V>
V* TreeAVLKeyValue<K, V>::getValuePointer(K& key, bool mustExist) {
	Node* node = getNode(key, mustExist);
	if (node == nullptr) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return nullptr;
	}

	return &node->value;
}

template <typename K, typename V>
V* TreeAVLKeyValue<K, V>::getValuePointerB(K key, bool mustExist) {
	return getValuePointer(key, mustExist);
}

template <typename K, typename V>
V TreeAVLKeyValue<K, V>::getValueDirect(K& key, V fallbackValue) {
	Node* node = getNode(key, false/*mustExist*/);
	if (node != nullptr) {
		return node->value;
	}

	return fallbackValue;
}

template <typename K, typename V>
V TreeAVLKeyValue<K, V>::getValueDirectB(K key, V fallbackValue) {
	Node* node = getNode(key, false/*mustExist*/);
	if (node != nullptr) {
		return node->value;
	}

	return fallbackValue;
}

template <typename K, typename V>
bool TreeAVLKeyValue<K, V>::containsReference(K& key) {
	Node* node = getNode(key);
	if (node != nullptr) {
		return true;
	}

	return false;
}

template <typename K, typename V>
bool TreeAVLKeyValue<K, V>::containsDirect(K& key) {
	Node* node = getNode(key);
	if (node != nullptr) {
		return true;
	}

	return false;
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::count() {
	return _count;
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::sizeBytes() {
	return _sizeBytes;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::rotateLeft(Node* z) {
	Node* y = z->right;
	Node* tB = y->left;

	// Perform rotation
	if (z->parent == nullptr) {
		y->parent = nullptr;
		_root = y;
	}
	if (z->parent != nullptr) {
		if (z->parent->left == z) {
			z->parent->setLeft(y);
		} else if (z->parent->right == z) {
			z->parent->setRight(y);
		} else {
			throw LogicException(LOC);
		}
	}

	y->setLeft(z);
	z->setRight(tB);

	// Update heights  
	z->height = 1 + max(getNodeHeight(z->left), getNodeHeight(z->right));
	y->height = 1 + max(getNodeHeight(y->left), getNodeHeight(y->right));

	// Return new root  
	return y;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::rotateLeftWithSubRightRotation(Node* z) {
	Node* y = z->right;
	Node* x = y->left;
	Node* tC = x->left;
	Node* tD = x->right;

	// Perform rotation
	if (z->parent == nullptr) {
		x->parent = nullptr;
		_root = x;
	}
	if (z->parent != nullptr) {
		// x is the new root.
		if (z->parent->left == z) {
			z->parent->setLeft(x);
		} else if (z->parent->right == z) {
			z->parent->setRight(x);
		} else {
			throw LogicException(LOC);
		}
	}

	x->setLeft(z);
	x->setRight(y);
	z->setRight(tC);
	y->setLeft(tD);

	// Update heights
	y->height = 1 + max(getNodeHeight(y->left), getNodeHeight(y->right));
	z->height = 1 + max(getNodeHeight(z->left), getNodeHeight(z->right));
	x->height = 1 + max(getNodeHeight(x->left), getNodeHeight(x->right));

	// Return new root
	return x;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::rotateRight(Node* z) {
	Node* y = z->left;
	Node* tB = y->right;
  
    // Perform rotation
	if (z->parent == nullptr) {
		y->parent = nullptr;
		_root = y;
	}
	if (z->parent != nullptr) {
		if (z->parent->left == z) {
			z->parent->setLeft(y);
		} else if (z->parent->right == z) {
			z->parent->setRight(y);
		}else {
			throw LogicException(LOC);
		}
	}

    y->setRight(z);  
    z->setLeft(tB);
  
    // Update heights
    z->height = 1 + max(getNodeHeight(z->left), getNodeHeight(z->right));
    y->height = 1 + max(getNodeHeight(y->left), getNodeHeight(y->right));
  
    // Return new root  
    return y;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::rotateRightWithSubLeftRotation(Node* z) {
	Node* y = z->left;
	Node* x = y->right;
	Node* tC = x->left;
	Node* tD = x->right;

	// Perform rotation
	if (z->parent == nullptr) {
		x->parent = nullptr;
		_root = x;
	}
	if (z->parent != nullptr) {
		// x is the new root.
		if (z->parent->left == z) {
			z->parent->setLeft(x);
		} else if (z->parent->right == z) {
			z->parent->setRight(x);
		} else {
			throw LogicException(LOC);
		}
	}

	x->setLeft(y);
	x->setRight(z);
	y->setRight(tC);
	z->setLeft(tD);

	// Update heights
	y->height = 1 + max(getNodeHeight(y->left), getNodeHeight(y->right));
	z->height = 1 + max(getNodeHeight(z->left), getNodeHeight(z->right));
	x->height = 1 + max(getNodeHeight(x->left), getNodeHeight(x->right));

	// Return new root
	return x;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertNode(Node* newNode) {
	if (newNode == nullptr || newNode->list) {
		throw LogicException(LOC);
	}

	Node* insertionParentNode = findInsertionParentNode(newNode->key);
	if (insertionParentNode == nullptr) {
		_root = newNode;
	} else {
		// Check if insertion as left or right child.
		if (newNode->key < insertionParentNode->key) {
			// Left

			if (insertionParentNode->left != nullptr) {
				throw LogicException(LOC);
			}

			insertionParentNode->setLeft(newNode);
		} else {
			// Right
			
			if (insertionParentNode->right != nullptr) {
				throw LogicException(LOC);
			}

			insertionParentNode->setRight(newNode);
		}

		// Go up the branch until root and increase the height of each node and do needed balancing.
		if (newNode->parent != nullptr) {
			ballanceBranchUpward(newNode->parent);
		}
	}

	newNode->list = this;

	_count++;
	_sizeBytes += nodeSizeBytes;

	return newNode;
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::findInsertionParentNode(K& key) {
	// This initial value is not important because there is no entry at the insert location so this is guaranteed to change before return occurs or remain nullptr.
	Node* prevSubTreeRootNode = nullptr;
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr && key != subTreeRootNode->key) {
		prevSubTreeRootNode = subTreeRootNode;
		subTreeRootNode = (key > subTreeRootNode->key) ? subTreeRootNode->right : subTreeRootNode->left;
	}

	if (subTreeRootNode != nullptr) {
		// The entry is already in the tree.
		throw AlreadyExistsException();
	}

	return prevSubTreeRootNode;
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::ballanceBranchUpward(Node* startBottomNode) {
	Node* currentNode = startBottomNode;

	while(currentNode != nullptr) {
		int leftHeight = (currentNode->left != nullptr) ? currentNode->left->height : 0;
		int rightHeight = (currentNode->right != nullptr) ? currentNode->right->height : 0;
		
		// Use this instead of incrementing,decrementing because the other branch may keep the height value lower than this branch.
		currentNode->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);

		int ballance = currentNode->getBallance();

		// Check for imbalance of 2. If larger imbalance, error.
		if (ballance == 2) {
			// Right is too heavy. Left is too light.
			if (currentNode->right->getBallance() <= -1) {
				// The right child is left heavy a bit, it requires 2 rotations or the branch will become imbalanced in the other direction.
				rotateLeftWithSubRightRotation(currentNode);
			} else {
				rotateLeft(currentNode);
			}
		} else if (ballance == -2) {
			// Left is too heavy. Right is too light.
			if (currentNode->left->getBallance() >= 1) {
				// The left child is right heavy a bit, it requires 2 rotations or the branch will become imbalanced in the other direction.
				rotateRightWithSubLeftRotation(currentNode);
			} else {
				rotateRight(currentNode);
			}
		}

		currentNode = currentNode->parent;
	}
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertDirect(K& key, V value) {
	return insertReference(key, value);
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertDirectB(K key, V value) {
	return insertReference(key, value);
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertReference(K& key, V& value) {
	if (bypassMemoryManagement) {
		return insertNode(new (std::malloc(sizeof(Node)))Node(false/*isNodeExternallyManuallyDeleted*/, key, value));
	} else {
		return insertNode(new Node(false/*isNodeExternallyManuallyDeleted*/, key, value));
	}
}

template <typename K, typename V>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertReferenceB(K key, V& value) {
	return insertReference(key, value);
}

template <typename K, typename V>
template <typename... Vs>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertEmplace(K& key, Vs&&... args) {
	if (bypassMemoryManagement) {
		return insertNode(new (std::malloc(sizeof(Node)))Node(false/*isNodeExternallyManuallyDeleted*/, key, std::forward<Vs>(args)...));
	} else {
		return insertNode(new Node(false/*isNodeExternallyManuallyDeleted*/, key, std::forward<Vs>(args)...));
	}
}

template <typename K, typename V>
template <typename... Vs>
typename TreeAVLKeyValue<K, V>::Node* TreeAVLKeyValue<K, V>::insertEmplaceB(K key, Vs&&... args) {
	if (bypassMemoryManagement) {
		return insertNodeB(new (std::malloc(sizeof(Node)))Node(false/*isNodeExternallyManuallyDeleted*/, key, std::forward<Vs>(args)...));
	} else {
		return insertNodeB(new Node(false/*isNodeExternallyManuallyDeleted*/, key, std::forward<Vs>(args)...));
	}
}

template <typename K, typename V>
bool TreeAVLKeyValue<K, V>::removeKeyReference(K& key, bool mustExist) {
	Node* node = getNode(key, mustExist);
	if (node == nullptr) {
		return false;
	}

	removeNode(node);

	return true;
}

template <typename K, typename V>
bool TreeAVLKeyValue<K, V>::removeKeyDirect(K key, bool mustExist) {
	return removeKeyReference(key, mustExist);
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::removeNode(Node* node, bool doAvlBallancing) {
	if (isEnumeratingInProgress || isEnumeratingInProgressB) {
		//asd_1;// implement these.
		// Currently inserts, removes are not allowed while enumerating;
		throw LogicException(LOC);
	}
	if (node->list != this || _count < 0/*this_is_probably_not_valid_pointer*/) {
		throw LogicException(LOC);
	}

	onNodeRemovePre(node);

	Node* balancingStartParentNode = node->parent;

	// Extract the node from the tree.
	/// Make sure to handle _root changes.
	if (node->left != nullptr && node->right != nullptr) {
		// Both nodes exist.
		// Extract the node->right subtree min from the.
		Node* extractedNode = getMinValueNode(node->right);

		// Update balancingStartParentNode if needed.
		if (extractedNode->parent != node) {
			// Need to balance the the extractedNode parent branch also. The branches are combinable.
			balancingStartParentNode = extractedNode->parent;
		}

		// Do the node extraction.
		Node* extractedNodeSingleChild = nullptr;
		if (extractedNode->left != nullptr && extractedNode->right != nullptr) {
			// extractedNode may have only 1 child, if it has 2 then it is not the min value node.
			throw LogicException(LOC);
		} else if (extractedNode->left != nullptr) {
			extractedNodeSingleChild = extractedNode->left;
		} else if (extractedNode->right != nullptr) {
			extractedNodeSingleChild = extractedNode->right;
		}

		if (extractedNode->parent->left == extractedNode) {
			extractedNode->parent->setLeft(extractedNodeSingleChild);
		} else if (extractedNode->parent->right == extractedNode) {
			extractedNode->parent->setRight(extractedNodeSingleChild);
		} else {
			throw LogicException(LOC);
		}

		// Replace node with the extracted node.
		// Note. No need to update the height because that is done in the re-balancing.
		if (node->left != nullptr) {
			extractedNode->setLeft(node->left);
		}
		if (node->right != nullptr) {
			extractedNode->setRight(node->right);
		}
		if (node->parent != nullptr) {
			if (node->parent->left == node) {
				node->parent->setLeft(extractedNode);
			}else if (node->parent->right == node) {
				node->parent->setRight(extractedNode);
			} else {
				throw LogicException(LOC);
			}
		} else {
			// Handle _root changes.
			// There is not parent, the left node is the new root.
			extractedNode->parent = nullptr;
			_root = extractedNode;
		}
	} else if (node->left != nullptr) {
		// Only left node exists.
		if (node->parent != nullptr) {
			if (node->parent->left == node) {
				node->parent->setLeft(node->left);
			} else if (node->parent->right == node) {
				node->parent->setRight(node->left);
			} else {
				throw LogicException(LOC);
			}
		} else {
			// There is not parent, the left node is the new root.
			node->left->parent = nullptr;
			_root = node->left;
		}
	} else if (node->right != nullptr) {
		// Only right node exists.
		if (node->parent != nullptr) {
			if (node->parent->left == node) {
				node->parent->setLeft(node->right);
			} else if (node->parent->right == node) {
				node->parent->setRight(node->right);
			} else {
				throw LogicException(LOC);
			}
		} else {
			// There is not parent, the left node is the new root.
			node->right->parent = nullptr;
			_root = node->right;
		}
	} else {
		// The node has no children.
		if (node->parent != nullptr) {
			if (node->parent->left == node) {
				node->parent->setLeft(nullptr);
			} else if (node->parent->right == node) {
				node->parent->setRight(nullptr);
			} else {
				throw LogicException(LOC);
			}
		} else {
			// There is not parent this was the only entry in the tree.
			_root = nullptr;
		}
	}

	// Dispose the node.
	node->list = nullptr;

	if (!node->isNodeExternallyManuallyDeleted) {
		if (bypassMemoryManagement) {
			node->~Node();
			std::free(node);
		} else {
			delete node;
		}
	}

	_count--;
	_sizeBytes -= nodeSizeBytes;

	// Ballance the branch above the removed node if this was not the root node.
	// Ballancing can be skipped with doAvlBallancing when a tree clear is called for efficiency.
	if (doAvlBallancing && balancingStartParentNode != nullptr) {
		ballanceBranchUpward(balancingStartParentNode);
	}
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::onNodeRemovePre(Node* node) {
	//asd_1;// implement this logic.
	/*if (isEnumeratingInProgress && node == nextEnumeratingNode) {
		nextEnumeratingNode = node->next;
	}
	if (isEnumeratingInProgressB && node == nextEnumeratingNodeB) {
		nextEnumeratingNodeB = node->next;
	}*/
}

template <typename K, typename V>
template <typename F>
void TreeAVLKeyValue<K, V>::enumerateInOrderReference(F& cb) {
	// Call the start and stop even if no nodes in the list to handle enumerations error checks (start while another enumeration is in progress).
	startManualEnumeration();

	bool stopEnumeration = false;
	if (_root != nullptr) {
		Node* cNode = _root;
		int dir = -1;

		while (true) {
			if (dir == -1) {
				// Try to go left.
				if (cNode->left != nullptr) {
					// Go left.
					dir = -1;
					cNode = cNode->left;
				} else {
					// There is no left, process this node on the next loop.
					dir = 0;
				}
			}else if (dir == 0) {
				// Process node.
				cb(cNode, cNode->value, stopEnumeration);

				if (stopEnumeration) {
					break;
				}

				dir = 1;
			}else if (dir == 1) {
				// Try to go right.
				if (cNode->right != nullptr) {
					// Go right.
					dir = -1;
					cNode = cNode->right;
				} else {
					// There is no right, prepare to go up on next loop.
					dir = 2;
				}
			}else if (dir == 2) {
				// Go to the parent when no right entries exist.
				if (cNode->parent != nullptr) {
					// Go up.
					if (cNode->parent->left == cNode) {
						// Go up from the left node.
						dir = 0;
					} else if (cNode->parent->right == cNode) {
						// Go up from the right node.
						dir = 2;
					} else {
						throw LogicException(LOC);
					}

					cNode = cNode->parent;
				} else {
					// This occurs when the _root node has no right branch.
					/// Enumeration finished.
					break;
				}
			} else {
				throw LogicException(LOC);
			}
		}
	}

	endManualEnumeration();
}

template <typename K, typename V>
template <typename F>
void TreeAVLKeyValue<K, V>::enumerateInOrderDirect(F cb) {
	enumerateInOrderReference(cb);
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::startManualEnumeration() {
	if (isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingNode = nullptr;
	isEnumeratingInProgress = true;
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::endManualEnumeration() {
	if (!isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingNode = nullptr;
	isEnumeratingInProgress = false;
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::startManualEnumerationB() {
	if (isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingNodeB = nullptr;
	isEnumeratingInProgressB = true;
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::endManualEnumerationB() {
	if (!isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingNodeB = nullptr;
	isEnumeratingInProgressB = false;
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::max(int a, int b) {
	return (a > b) ? a : b;
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::getNodeHeight(Node* node) {
	if (node == nullptr) {
		return 0;
	}

	return node->height;
}

template <typename K, typename V>
int TreeAVLKeyValue<K, V>::getNodeBalance(Node* node) {
	if (node == nullptr) {
		return 0;
	}

	return getNodeHeight(node->left) - getNodeHeight(node->right);
}

template <typename K, typename V>
void TreeAVLKeyValue<K, V>::clear() {
	// Do a depth first enumeration and remove all nodes ending with the _root removal.
	if (_root != nullptr) {
		Node* cNode = _root;
		int dir = -1;

		while (true) {
			if (dir == -1) {
				// Try to go left.
				if (cNode->left != nullptr) {
					// Go left.
					dir = -1;
					cNode = cNode->left;
				} else if (cNode->right != nullptr) {
					// There is no left, go right on the next loop.
					dir = 1;
				} else {
					// There is no right, process cNode next.
					dir = 0;
				}
			} else if (dir == 0) {
				// Remove node.
				// Because the node removal will remove the node, need to prepare the next loop tick cNode;
				Node* nodeToRemove = cNode;
				if (cNode->parent != nullptr) {
					if (cNode->parent->left == cNode) {
						dir = 1;
					} else if (cNode->parent->right == cNode) {
						dir = 0;
					} else {
						throw LogicException(LOC);
					}
				}
				cNode = cNode->parent;

				removeNode(nodeToRemove, false/*doAvlBallancing*/);

				if (cNode == nullptr) {
					break;
				}
			} else if (dir == 1) {
				// Try to go right.
				if (cNode->right != nullptr) {
					// Go right.
					dir = -1;
					cNode = cNode->right;
				} else {
					// There is no right, prepare to process the current node on next loop.
					dir = 0;
				}
			} else {
				throw LogicException(LOC);
			}
		}
	}
}

template <typename K, typename V>
TreeAVLKeyValue<K, V>::~TreeAVLKeyValue() {
	clear();
}
