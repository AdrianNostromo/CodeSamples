#pragma once

#include <base/gh.h>
#include <base/exceptions/OutOfRangeException.h>
#include <cassert>
#include <base/container/util/IContainerNode.h>
#include <base/container/util/IContainerNodeWrapper.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/AlreadyExistsException.h>
#include <base/exceptions/EntryNotFoundException.h>
#include <base/container/util/ICustomComparator1.h>

template <typename Data>
class TreeAVL {pub dCtor(TreeAVL);
	pub class Node : virtual public IContainerNode {pub dCtor(Node);
		// The node can be externally managed (eg. Ho control when the memory is freed as is for playing_sounds).
		pub bool isNodeExternallyManuallyDeleted;

		pub TreeAVL* list = nullptr;

		// The current algorithm requires a parent pointer.
		pub Node* parent = nullptr;

		// Only change with setLeft.
		pub Node* left = nullptr;
		// Only change with setRight.
		pub Node* right = nullptr;

		pub Data data;

		pub int height = 1;

		// This is used to notify a wrapper when a node is removed.
		// This is used by the Events_Dispatchers_listeners.
		pub IContainerNodeWrapper* wrapper = nullptr;

		pub explicit Node(bool isNodeExternallyManuallyDeleted, Data& data) noexcept;

		pub template <typename... Vs>
		explicit Node(bool isNodeExternallyManuallyDeleted, Vs&&... args) noexcept;

		pub void setLeft(Node* newLeftNode);
		pub void setRight(Node* newRightNode);

		pub Node* getInOrderPreviousNode();
		pub Node* getInOrderNextNode();

		pub int getBallance();

		pub void remove() override;

		pub ~Node() noexcept override;
	};

	priv Node* _root = nullptr;

	priv int _count = 0;

	// Enumeration start;
	// The idea with this is to allow for multiple entries removal,adding from a node callback function call. Also removes the need for a mark_for_removal system.
	// This allows for manual enumarions.
	pub bool isEnumeratingInProgress = false;
	pub typename TreeAVL<Data>::Node* nextEnumeratingNode = nullptr;

	pub bool isEnumeratingInProgressB = false;
	pub typename TreeAVL<Data>::Node* nextEnumeratingNodeB = nullptr;
	// Enumeration end;

	pub explicit TreeAVL();

	pub Node* getRoot();

	pub Node* getMinValueNode();
	pub Node* getMinValueNode(Node* subTreeRootNode);
	pub Node* getMaxValueNode();
	pub Node* getMaxValueNode(Node* subTreeRootNode);

	pub Node* getInOrderPreviousNode(Node* originNode);
	pub Node* getInOrderNextNode(Node* originNode);
	
	pub template <typename KeyType>
	Node* getNodeFromKey(KeyType& key, bool mustExist=false);

	// Note. If needed, can add
	pub Node* getNode(Data& data, bool mustExist=false);
	pub Node* getNodeB(Data data, bool mustExist=false);
	// This is used to have custom data code or complex keys.
	pub Node* getNode(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data);
	
	pub template <typename Key>
	Node* getNodeEqualOrPreviousSmaller(Key& key);
	
	pub template <typename Key>
	Node* getNodeEqualOrNextBigger(Key& key);

	// This is used to get an existing node of if none is found, insert emplace a new one.
	/// This function uses some optimisations to avoid 2 tree passes.
	pub template <typename... Vs>
	Node* getNodeOrInsertEmplaceNew(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data, bool* oIsNewInsertOccured, Vs&&... args);

	pub bool containsReference(Data& data);
	pub bool containsDirect(Data& data);

	pub int count();

	// Rotation ilustrations: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/?ref=lbp
	pub Node* rotateLeft(Node* z);
	pub Node* rotateLeftWithSubRightRotation(Node* z);
	pub Node* rotateRight(Node* z);
	pub Node* rotateRightWithSubLeftRotation(Node* z);

	pub Node* insertDirect(Data data);
	pub Node* insertReference(Data& data);

	pub template <typename... Vs>
	Node* insertEmplace(Vs&&... args);
	
	pub Node* insertNode(Node* node);
	// If parentNode==nullptr, insert as _root in a empty list.
	pub Node* insertNode(Node* node, Node* parentNode);
	
	// This is used to remove and insert a node without delete-ing it.
	/// This is used when the node key changes in a way that affects the sowrt order.
	pub void reinsertNode(Node* node);

	// If oExistingEntry != nullptr, it will contain be set with a existing entry if a exact match is founc.
	// If oExistingEntry == nullptr, existing entries will throw a alread_exists_exception.
	// oInsertionParent contains the insertion parent. This is set only if no exact match found.
	pub void findNodeOrInsertionParentNode(Data& data, Node*& oInsertionParent, Node** oExistingEntry);
	pub void findNodeOrInsertionParentNode(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data, Node*& oInsertionParent, Node** oExistingEntry);

	pub void ballanceBranchUpward(Node* startBottomNode);

	pub bool removeKeyReference(Data& data, bool mustExist=false);
	pub bool removeKeyDirect(Data data, bool mustExist=false);

	// doAvlBallancing is false when removing all entries to avoid the ballancing steps.
	// disableDeleteIfManagedLogic is used by reInsertNode(...) to avoid a delete and new call.
	pub void removeNode(Node* node, bool doAvlBallancing=true, bool disableDeleteIfManagedLogic=false);
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

	pub ~TreeAVL();
};

template <typename Data>
TreeAVL<Data>::Node::Node(bool isNodeExternallyManuallyDeleted, Data& data) noexcept
	: isNodeExternallyManuallyDeleted(isNodeExternallyManuallyDeleted), data(data)
{
	//void
}

template <typename Data>
template <typename... Vs>
TreeAVL<Data>::Node::Node(bool isNodeExternallyManuallyDeleted, Vs&&... args) noexcept
	: isNodeExternallyManuallyDeleted(isNodeExternallyManuallyDeleted), data(std::forward<Vs>(args)...)
{
	//void
}

template <typename Data>
void TreeAVL<Data>::Node::setLeft(Node* newLeftNode) {
	// Don't cleanup the parent of the old entry because the caller assumes no cleanup.

	left = newLeftNode;

	if (left != nullptr) {
		left->parent = this;
	}
}

template <typename Data>
void TreeAVL<Data>::Node::setRight(Node* newRightNode) {
	// Don't cleanup the parent of the old entry because the caller assumes no cleanup.

	right = newRightNode;

	if (right != nullptr) {
		right->parent = this;
	}
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::Node::getInOrderPreviousNode() {
	return list->getInOrderPreviousNode(this);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::Node::getInOrderNextNode() {
	return list->getInOrderNextNode(this);
}

template <typename Data>
int TreeAVL<Data>::Node::getBallance() {
	int leftHeight = (left != nullptr) ? left->height : 0;
	int rightHeight = (right != nullptr) ? right->height : 0;

	return rightHeight - leftHeight;
}

template <typename Data>
void TreeAVL<Data>::Node::remove() {
	if (list == nullptr) {
		throw LogicException(LOC);
	}

	list->removeNode(this);
}

template <typename Data>
TreeAVL<Data>::Node::~Node() noexcept {
	//void
}

template <typename Data>
TreeAVL<Data>::TreeAVL() {
	//void
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getRoot() {
	return _root;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getMinValueNode() {
	return getMinValueNode(_root);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getMinValueNode(Node* subTreeRootNode) {
	TreeAVL<Data>::Node* currentNode = subTreeRootNode;

	/* loop down to find the leftmost leaf */
	while (currentNode != nullptr && currentNode->left != nullptr) {
		currentNode = currentNode->left;
	}

	return currentNode;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getMaxValueNode() {
	return getMaxValueNode(_root);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getMaxValueNode(Node* subTreeRootNode) {
	TreeAVL<Data>::Node* currentNode = subTreeRootNode;

	/* loop down to find the rightmost leaf */
	while (currentNode != nullptr && currentNode->right != nullptr) {
		currentNode = currentNode->right;
	}

	return currentNode;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getInOrderPreviousNode(Node* originNode) {
	if (originNode->left != nullptr) {
		// Go down the tree to find the max value.
		return getMaxValueNode(originNode->left);
	}
	
	// Go up the tree to find the FIRST_OCCURENCE of a parent that is on the left.
	Node* cNode = originNode;
	while (cNode != nullptr && cNode->parent != nullptr) {
		if (cNode == cNode->parent->right) {
			return cNode->parent;
		}

		cNode = cNode->parent;
	}

	return nullptr;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getInOrderNextNode(Node* originNode) {
	if (originNode->right != nullptr) {
		// Go down the tree to find the min value.
		return getMinValueNode(originNode->right);
	}
	
	// Go up the tree to find the FIRST_OCCURENCE of a parent that is on the left.
	Node* cNode = originNode;
	while (cNode != nullptr && cNode->parent != nullptr) {
		if (cNode == cNode->parent->left) {
			return cNode->parent;
		}

		cNode = cNode->parent;
	}

	return nullptr;
}

template <typename Data>
template <typename KeyType>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNodeFromKey(KeyType& key, bool mustExist) {
	Node* subTreeRootNode = _root;
	
	while (subTreeRootNode != nullptr) {
		if (key == subTreeRootNode->data.key) {
			break;
		}

		subTreeRootNode = (key > subTreeRootNode->data.key) ? subTreeRootNode->right : subTreeRootNode->left;
	}

	if (mustExist && subTreeRootNode == nullptr) {
		throw EntryNotFoundException();
	}

	return subTreeRootNode;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNode(Data& data, bool mustExist) {
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		if (data == subTreeRootNode->data) {
			break;
		}

		subTreeRootNode = (data > subTreeRootNode->data) ? subTreeRootNode->right : subTreeRootNode->left;
	}

	if (mustExist && subTreeRootNode == nullptr) {
		throw EntryNotFoundException();
	}

	return subTreeRootNode;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNodeB(Data data, bool mustExist) {
	return getNode(data, mustExist);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNode(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data) {
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		int compareResult = data(subTreeRootNode->data);
		
		if (compareResult == 0) {
			break;
		}

		subTreeRootNode = (compareResult > 0) ? subTreeRootNode->right : subTreeRootNode->left;
	}

	return subTreeRootNode;
}

template <typename Data>
template <typename Key>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNodeEqualOrPreviousSmaller(Key& key) {
	Node* bestMatchNode = nullptr;
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		if (subTreeRootNode->data <= key) {
			// Solution candidate found. This is guaranteed to be better than the current bestMatchNode if it exists (because it is smaller).
			bestMatchNode = subTreeRootNode;

			// Try to find a better (larger) solution.
			subTreeRootNode = subTreeRootNode->right;
		} else {
			// Node is too small, go to it's left child if it exists.
			subTreeRootNode = subTreeRootNode->left;
		}
	}

	return bestMatchNode;
}

template <typename Data>
template <typename Key>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNodeEqualOrNextBigger(Key& key) {
	Node* bestMatchNode = nullptr;
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		if (subTreeRootNode->data >= key) {
			// Solution candidate found. This is guaranteed to be better than the current bestMatchNode if it exists (because it is smaller).
			bestMatchNode = subTreeRootNode;

			// Try to find a better (smaller) solution.
			subTreeRootNode = subTreeRootNode->left;
		}else {
			// Node is too small, go to it's right child if it exists.
			subTreeRootNode = subTreeRootNode->right;
		}
	}

	return bestMatchNode;
}

template <typename Data>
template <typename... Vs>
typename TreeAVL<Data>::Node* TreeAVL<Data>::getNodeOrInsertEmplaceNew(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data, bool* oIsNewInsertOccured, Vs&&... args) {
	Node* parentNode = nullptr;
	Node* existingNode = nullptr;
	findNodeOrInsertionParentNode(data, parentNode, &existingNode/*oExistingEntry*/);

	if (existingNode != nullptr) {
		// Node already exists.
		if (oIsNewInsertOccured != nullptr) {
			*oIsNewInsertOccured = false;
		}
		return existingNode;
	}

	// Insert a new node with "parentNode" as parent.
	Node* newNode = new Node(false/*isNodeExternallyManuallyDeleted*/, std::forward<Vs>(args)...);
	if (oIsNewInsertOccured != nullptr) {
		*oIsNewInsertOccured = true;
	}
	return insertNode(newNode, parentNode);
}

template <typename Data>
bool TreeAVL<Data>::containsReference(Data& data) {
	Node* node = getNode(data);
	if (node != nullptr) {
		return true;
	}

	return false;
}

template <typename Data>
bool TreeAVL<Data>::containsDirect(Data& data) {
	Node* node = getNode(data);
	if (node != nullptr) {
		return true;
	}

	return false;
}

template <typename Data>
int TreeAVL<Data>::count() {
	return _count;
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::rotateLeft(Node* z) {
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

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::rotateLeftWithSubRightRotation(Node* z) {
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

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::rotateRight(Node* z) {
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

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::rotateRightWithSubLeftRotation(Node* z) {
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

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::insertNode(Node* newNode) {
	if (newNode == nullptr || newNode->list) {
		throw LogicException(LOC);
	}

	Node* parentNode = nullptr;
	findNodeOrInsertionParentNode(newNode->data, parentNode, nullptr/*oExistingEntry*/);

	return insertNode(newNode, parentNode);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::insertNode(Node* newNode, Node* parentNode) {
	if (newNode == nullptr || newNode->list) {
		throw LogicException(LOC);
	}

	if (parentNode == nullptr) {
		// The list is empty, insert as _root.
		if (_root != nullptr) {
			throw LogicException(LOC);
		}

		_root = newNode;
	} else {
		// Check if insertion as left or right child.
		if (newNode->data < parentNode->data) {
			// Left

			if (parentNode->left != nullptr) {
				throw LogicException(LOC);
			}

			parentNode->setLeft(newNode);
		} else {
			// Right
			
			if (parentNode->right != nullptr) {
				throw LogicException(LOC);
			}

			parentNode->setRight(newNode);
		}

		// Go up the branch until root and increase the height of each node and do needed balancing.
		if (newNode->parent != nullptr) {
			ballanceBranchUpward(newNode->parent);
		}
	}

	newNode->list = this;

	_count++;

	return newNode;
}

template <typename Data>
void TreeAVL<Data>::reinsertNode(Node* node) {
	if (node == nullptr) {
		throw LogicException(LOC);
	}
	removeNode(node, true/*doAvlBallancing*/, true/*disableDeleteIfManagedLogic*/);
	insertNode(node);
}

template <typename Data>
void TreeAVL<Data>::findNodeOrInsertionParentNode(Data& data, Node*& oInsertionParent, Node** oExistingEntry) {
	// This initial value is not important because there is no entry at the insert location so this is guaranteed to change before return occurs or remain nullptr.
	Node* prevSubTreeRootNode = nullptr;
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		prevSubTreeRootNode = subTreeRootNode;

		if (data < subTreeRootNode->data) {
			subTreeRootNode = subTreeRootNode->left;
		} else if (data > subTreeRootNode->data) {
			subTreeRootNode = subTreeRootNode->right;
		} else {
			// The entry is already in the tree.
			if (oExistingEntry != nullptr) {
				*oExistingEntry = subTreeRootNode;
				return;
			}

			throw AlreadyExistsException();
		}
	}

	oInsertionParent = prevSubTreeRootNode;
}

template <typename Data>
void TreeAVL<Data>::findNodeOrInsertionParentNode(ICustomComparator1<Data&/*ArgumentType*/, int/*ResultType*/>& data, Node*& oInsertionParent, Node** oExistingEntry) {
	// This initial value is not important because there is no entry at the insert location so this is guaranteed to change before return occurs or remain nullptr.
	Node* prevSubTreeRootNode = nullptr;
	Node* subTreeRootNode = _root;

	while (subTreeRootNode != nullptr) {
		prevSubTreeRootNode = subTreeRootNode;

		int compareResult = data(subTreeRootNode->data);
		if (compareResult < 0) {
			subTreeRootNode = subTreeRootNode->left;
		} else if (compareResult > 0) {
			subTreeRootNode = subTreeRootNode->right;
		} else {
			// The entry is already in the tree.
			if (oExistingEntry != nullptr) {
				*oExistingEntry = subTreeRootNode;
				return;
			}

			throw AlreadyExistsException();
		}
	}

	oInsertionParent = prevSubTreeRootNode;
}

template <typename Data>
void TreeAVL<Data>::ballanceBranchUpward(Node* startBottomNode) {
	Node* currentNode = startBottomNode;

	while(currentNode != nullptr) {
		int leftHeight = (currentNode->left != nullptr) ? currentNode->left->height : 0;
		int rightHeight = (currentNode->right != nullptr) ? currentNode->right->height : 0;
		
		// Use this instead of incrementing,decrementing because the other branch may keep the height value lower than this branch.
		currentNode->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
		if (currentNode->height > 10000) {
			// A bug occured. THe height should never be this large.
			throw LogicException(LOC);
		}

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

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::insertDirect(Data data) {
	return insertReference(data);
}

template <typename Data>
typename TreeAVL<Data>::Node* TreeAVL<Data>::insertReference(Data& data) {
	return insertNode(new Node(false/*isNodeExternallyManuallyDeleted*/, data));
}

template <typename Data>
template <typename... Vs>
typename TreeAVL<Data>::Node* TreeAVL<Data>::insertEmplace(Vs&&... args) {
	return insertNode(new Node(false/*isNodeExternallyManuallyDeleted*/, std::forward<Vs>(args)...));
}

template <typename Data>
bool TreeAVL<Data>::removeKeyReference(Data& data, bool mustExist) {
	Node* node = getNode(data, mustExist);
	if (node == nullptr) {
		return false;
	}

	removeNode(node);

	return true;
}

template <typename Data>
bool TreeAVL<Data>::removeKeyDirect(Data data, bool mustExist) {
	return removeKeyReference(data, mustExist);
}

template <typename Data>
void TreeAVL<Data>::removeNode(Node* node, bool doAvlBallancing, bool disableDeleteIfManagedLogic) {
	if (isEnumeratingInProgress || isEnumeratingInProgressB) {
		//asd_1;// implement these.
		// Currently inserts, removes are not allowed while enumerating;
		throw LogicException(LOC);
	}
	if (node->list != this) {
		throw LogicException(LOC);
	}

	onNodeRemovePre(node);
	if (node->wrapper) {
		node->wrapper->onContainerNodeRemovePre();
	}

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
	if (!disableDeleteIfManagedLogic && !node->isNodeExternallyManuallyDeleted) {
		delete node;
	} else {
		// Clear node for larer re-used.
		node->list = nullptr;
		node->parent = nullptr;
		node->left = nullptr;
		node->right = nullptr;
		node->height = 1;
	}

	_count--;

	// Ballance the branch above the removed node if this was not the root node.
	// Ballancing can be skipped with doAvlBallancing when a tree clear is called for efficiency.
	if (doAvlBallancing && balancingStartParentNode != nullptr) {
		ballanceBranchUpward(balancingStartParentNode);
	}
}

template <typename Data>
void TreeAVL<Data>::onNodeRemovePre(Node* node) {
	//asd_1;// implement this logic.
	/*if (isEnumeratingInProgress && node == nextEnumeratingNode) {
		nextEnumeratingNode = node->next;
	}
	if (isEnumeratingInProgressB && node == nextEnumeratingNodeB) {
		nextEnumeratingNodeB = node->next;
	}*/
}

template <typename Data>
template <typename F>
void TreeAVL<Data>::enumerateInOrderReference(F& cb) {
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
			} else if (dir == 0) {
				// Process node.
				cb(cNode, cNode->data, stopEnumeration);

				if (stopEnumeration) {
					break;
				}

				dir = 1;
			} else if (dir == 1) {
				// Try to go right.
				if (cNode->right != nullptr) {
					// Go right.
					dir = -1;
					cNode = cNode->right;
				} else {
					// There is no right, prepare to go up on next loop.
					dir = 2;
				}
			} else if (dir == 2) {
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

template <typename Data>
template <typename F>
void TreeAVL<Data>::enumerateInOrderDirect(F cb) {
	enumerateInOrderReference(cb);
}

template <typename Data>
void TreeAVL<Data>::startManualEnumeration() {
	if (isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingNode = nullptr;
	isEnumeratingInProgress = true;
}

template <typename Data>
void TreeAVL<Data>::endManualEnumeration() {
	if (!isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingNode = nullptr;
	isEnumeratingInProgress = false;
}

template <typename Data>
void TreeAVL<Data>::startManualEnumerationB() {
	if (isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingNodeB = nullptr;
	isEnumeratingInProgressB = true;
}

template <typename Data>
void TreeAVL<Data>::endManualEnumerationB() {
	if (!isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingNodeB = nullptr;
	isEnumeratingInProgressB = false;
}

template <typename Data>
int TreeAVL<Data>::max(int a, int b) {
	return (a > b) ? a : b;
}

template <typename Data>
int TreeAVL<Data>::getNodeHeight(Node* node) {
	if (node == nullptr) {
		return 0;
	}

	return node->height;
}

template <typename Data>
int TreeAVL<Data>::getNodeBalance(Node* node) {
	if (node == nullptr) {
		return 0;
	}

	return getNodeHeight(node->left) - getNodeHeight(node->right);
}

template <typename Data>
void TreeAVL<Data>::clear() {
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
					}else if (cNode->parent->right == cNode) {
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

template <typename Data>
TreeAVL<Data>::~TreeAVL() {
	clear();
}
