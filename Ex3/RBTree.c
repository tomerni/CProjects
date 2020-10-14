#include <string.h>
#include <stdlib.h>
#include "RBTree.h"

#define FAIL 0
#define SUCCESS 1

/**
 * responsible on the third delete case
 * @param delete the node to delete
 * @param parent the parent of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCase3(Node* delete, Node* parent, Node* brother);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree* newTree = (RBTree *) malloc(sizeof(RBTree));
	if(newTree == NULL)
	{
		return NULL;
	}
	newTree->compFunc = compFunc;
	newTree->freeFunc = freeFunc;
	newTree->size = 0;
	newTree->root = NULL;
	return newTree;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	if(tree == NULL || data == NULL)
	{
		return FAIL;
	}
	Node* runner = tree->root;
	while(runner != NULL)
	{
		int res = tree->compFunc(runner->data, data);
		if(res == 0)
		{
			return SUCCESS;
		}
		if(res > 0)
		{
			runner = runner->left;
		}
		else
		{
			runner = runner->right;
		}
	}
	return FAIL;
}

/**
 * frees all the nodes and the data in the tree
 * @param treeNode the node to free
 * @param freeFunc the freeFunc from the user
 */
void freeNode(Node* treeNode, FreeFunc freeFunc)
{
	if(treeNode->right == NULL && treeNode->left == NULL)
	{
		freeFunc(treeNode->data);
		free(treeNode);
	}
	else if(treeNode->right == NULL)
	{
		freeNode(treeNode->left, freeFunc);
		freeFunc(treeNode->data);
		free(treeNode);
	}
	else if(treeNode->left == NULL)
	{
		freeNode(treeNode->right, freeFunc);
		freeFunc(treeNode->data);
		free(treeNode);
	}
	else
	{
		freeNode(treeNode->left, freeFunc);
		freeNode(treeNode->right, freeFunc);
		freeFunc(treeNode->data);
		free(treeNode);
	}
}


/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
	if(*tree != NULL)
	{
		if((*tree)->root != NULL)
		{
			freeNode((*tree)->root, (*tree)->freeFunc);
		}
		free(*tree);
	}
	*tree = NULL;
}

/**
 * find the location of a given data
 * @param tree the tree
 * @param data the data
 * @return NULL if the data is not in the tree, else pointer to the node with the data
 */
Node* findLocation(const RBTree *tree, const void *data)
{
	if(tree == NULL)
	{
		return NULL;
	}
	Node* runner = tree->root;
	Node* location = NULL;
	while(runner != NULL)
	{
		if(tree->compFunc(runner->data, data) == 0)
		{
			return runner;
		}
		if(tree->compFunc(runner->data, data) > 0)
		{
			if(runner->left == NULL)
			{
				location = runner;
				break;
			}
			runner = runner->left;
		}
		else
		{
			if(runner->right == NULL)
			{
				location = runner;
				break;
			}
			runner = runner->right;
		}
	}
	return location;
}

/**
 * inits the values in the node
 * @param data the data
 * @return pointer to the new node
 */
Node* initNode(void* data)
{
	Node* newNode = (Node*) malloc(sizeof(Node));
	if(newNode == NULL)
	{
		return NULL;
	}
	newNode->data = data;
	newNode->color = RED;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	return newNode;
}

/**
 * finds the uncle
 * @param parent node's parent
 * @param grandParent node's grandparent
 * @return pointer to the uncle node
 */
Node* getUncle(const Node* parent, Node* grandParent)
{
	if(grandParent->right == parent)
	{
		return grandParent->left;
	}
	else
	{
		return grandParent->right;
	}
}

/**
 * insert case number 3
 * @param parent node's parent
 * @param grandParent node's grandparent
 * @param uncle node's uncle
 */
void redFatherRedUncle(Node* parent, Node* grandParent, Node* uncle)
{
	parent->color = BLACK;
	uncle->color = BLACK;
	grandParent->color = RED;
}

/**
 * rotates the tree to the right
 * @param newNode the node to rotate
 */
void rotateRight(Node* newNode)
{
	Node* leftChild = newNode->left;
	Node* parent = newNode->parent;
	if(leftChild != NULL)
	{
		newNode->left = leftChild->right;
		leftChild->right = newNode;
	}
	else
	{
		newNode->left = NULL;
	}
	newNode->parent = leftChild;
	if (newNode->left != NULL)
	{
		newNode->left->parent = newNode;
	}
	if (parent != NULL)
	{
		if (newNode == parent->left)
		{
			parent->left = leftChild;
		}
		else if (newNode == parent->right)
		{
			parent->right = leftChild;
		}
	}
	leftChild->parent = parent;
}

/**
 * rotates the tree to the left
 * @param newNode the node to rotate
 */
void rotateLeft(Node* newNode)
{
	Node* rightChild = newNode->right;
	Node* parent = newNode->parent;
	if(rightChild != NULL)
	{
		newNode->right = rightChild->left;
		rightChild->left = newNode;
	}
	else
	{
		newNode->right = NULL;
	}
	newNode->parent = rightChild;
	if (newNode->right != NULL)
	{
		newNode->right->parent = newNode;
	}
	if (parent != NULL)
	{
		if (newNode == parent->left)
		{
			parent->left = rightChild;
		}
		else if (newNode == parent->right)
		{
			parent->right = rightChild;
		}
	}
	rightChild->parent = parent;
}

/**
 * the second step of the 4 insert case
 * @param newNode the node to change
 */
void secondStep(Node* newNode)
{
	Node* parent = newNode->parent;
	Node* grandParent = parent->parent;
	if(newNode == parent->left)
	{
		rotateRight(grandParent);
	}
	else
	{
		rotateLeft(grandParent);
	}
	parent->color = BLACK;
	grandParent->color = RED;
}

/**
 * insert case number 3
 * @param newNode the node to insert
 * @param parent the node's parent
 * @param grandParent the node's grandparent
 */
void redFatherBlackUncle(Node* newNode, Node* parent, Node* grandParent)
{
	if(newNode == parent->right && parent == grandParent->left)
	{
		rotateLeft(parent);
		newNode = newNode->left;
	}
	else if(newNode == parent->left && parent == grandParent->right)
	{
		rotateRight(parent);
		newNode = newNode->right;
	}
	secondStep(newNode);
}

/**
 * the main function that responsible on the insert repairs
 * @param tree the tree
 * @param parent node's parent
 * @param newNode the node to insert
 */
void insertRepairs(RBTree* tree, Node* parent, Node* newNode)
{
	if(parent == NULL)
	{
		newNode->color = BLACK;
	}
	else if(parent->color == BLACK)
	{
		return;
	}
	else
	{
		Node* grandParent = parent->parent;
		Node* uncle = getUncle(parent, grandParent);
		if(uncle == NULL || uncle->color == BLACK)
		{
			redFatherBlackUncle(newNode, parent, grandParent);
		}
		else
		{
			redFatherRedUncle(parent, grandParent, uncle);
			insertRepairs(tree, grandParent->parent, grandParent);
		}
	}
}

/**
 * hangs the node on the tree
 * @param parent the parent of the node
 * @param newNode the node to hang
 * @param tree the tree
 */
void hangNode(Node* parent, Node* newNode, RBTree *tree)
{
	if(parent == NULL)
	{
		tree->root = newNode;
	}
	else
	{
		newNode->parent = parent;
		if (tree->compFunc(parent->data, newNode->data) > 0)
		{
			parent->left = newNode;
		}
		else
		{
			parent->right = newNode;
		}
	}
}

/**
 * finds the new root after the insert
 * @param newNode
 * @return pointer to the root
 */
Node* findNewRoot(Node* newNode)
{
	while (newNode->parent != NULL)
	{
		newNode = newNode->parent;
	}
	return newNode;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
	if(tree == NULL || data == NULL || RBTreeContains(tree, data) != 0)
	{
		return FAIL;
	}
	Node* parent = findLocation(tree, data);
	Node* newNode = initNode(data);
	if(newNode == NULL)
	{
		return FAIL;
	}
	hangNode(parent, newNode, tree);
	insertRepairs(tree, parent, newNode);
	tree->size++;
	tree->root = findNewRoot(newNode);
	return SUCCESS;
}

/**
 *
 * @param curNode the current node
 * @param func the func to apply on the nodes
 * @param args extra args
 * @return 0 if the func failed, else the value of the run
 */
int forEachRBTreeHelper(Node* curNode, forEachFunc func, void *args)
{
	if(curNode == NULL)
	{
		return FAIL;
	}
	if(curNode->left != NULL)
	{
		int res = forEachRBTreeHelper(curNode->left, func, args);
		if(res == 0)
		{
			return FAIL;
		}
	}
	int val = func(curNode->data, args);
	if(curNode->right != NULL)
	{
		int res = forEachRBTreeHelper(curNode->right, func, args);
		if(res == 0)
		{
			return FAIL;
		}
	}
	return val;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if(tree == NULL)
	{
		return FAIL;
	}
	if(tree->root == NULL)
	{
		return SUCCESS;
	}
	int res = forEachRBTreeHelper(tree->root, func, args);
	if(res == 0)
	{
		return FAIL;
	}
	return SUCCESS;
}

/**
 * finds the successor of the node
 * @param n the node
 * @return the successor
 */
Node* findSuccessor(Node* n)
{
	if(n->left == NULL)
	{
		return n;
	}
	else
	{
		return findSuccessor(n->left);
	}
}

/**
 * changes the data of the node to delete with his successor
 * @param deleteNode the node to delete
 * @return the successor node
 */
Node* changeWithSuccessor(Node* deleteNode)
{
	Node* successor = findSuccessor(deleteNode->right);
	void* temp = deleteNode->data;
	deleteNode->data = successor->data;
	successor->data = temp;
	return successor;
}

/**
 * finds the child of the node
 * @param n the node
 * @return the child
 */
Node* findChild(Node* n)
{
	if(n->left != NULL)
	{
		return n->left;
	}
	else if(n->right != NULL)
	{
		return n->right;
	}
	return NULL;
}

/**
 * finds the brother of the node
 * @param n the node
 * @return the brother
 */
Node* findBrother(Node* n)
{
	if(n->parent == NULL)
	{
		return NULL;
	}
	if(n->parent->left == n)
	{
		return n->parent->right;
	}
	return n->parent->left;
}

/**
 * executes delete case number 1
 * @param delete the node to delete
 * @param parent the parent of the node to delete
 */
void deleteCase1(const Node* delete, Node* parent)
{
	if(parent->left == delete)
	{
		parent->left = NULL;
	}
	else
	{
		parent->right = NULL;
	}
}

/**
 * executes delete case number 2
 * @param delete the node to delete
 * @param child the child of the node
 * @param parent the parent of the node
 */
void deleteCase2(const Node* delete, Node* child, Node* parent)
{
	if(parent->right == delete)
	{
		parent->right = child;
		child->parent = parent;
	}
	else
	{
		parent->left = child;
		child->parent = parent;
	}
	child->color = BLACK;
}

/**
 * executes delete case 3B1
 * @param parent the parent of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCase3B1(Node* parent, Node* brother)
{
	parent->color = BLACK;
	brother->color = RED;
}

/**
 * executes delete case 3B2
 * @param parent the parent of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCase3B2(Node* parent, Node* brother)
{
	brother->color = RED;
	deleteCase3(parent, parent->parent, findBrother(parent));
}

/**
 * executes delete case 3C
 * @param delete the node to delete
 * @param parent the parent of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCase3C(Node* delete, Node* parent, Node* brother)
{
	brother->color = BLACK;
	parent->color = RED;
	if(parent->left == delete)
	{
		rotateLeft(parent);
	}
	else
	{
		rotateRight(parent);
	}
	deleteCase3(delete, parent, findBrother(delete));
}

/**
 * finds the far child of the brother
 * @param delete the node to delete
 * @param brother the brother
 * @return the far child of the brother
 */
Node* findFarChild(const Node* delete, Node* brother)
{
	if(delete->parent->right == delete)
	{
		return brother->left;
	}
	return brother->right;
}

/**
 * finds the close child of the brother
 * @param delete the node to delete
 * @param brother the brother
 * @return the close child of the brother
 */
Node* findCloseChild(const Node* delete, Node* brother)
{
	if(delete->parent->right == delete)
	{
		return brother->right;
	}
	return brother->left;
}

/**
 * executes delete case 3D
 * @param delete the node to delete
 * @param parent the parent
 * @param brother the brother
 * @param closeChild the close child of the brother
 */
void deleteCase3D(Node* delete, Node* parent, Node* brother, Node* closeChild)
{
	closeChild->color = BLACK;
	brother->color = RED;
	if(brother->left == closeChild)
	{
		rotateRight(brother);
	}
	else
	{
		rotateLeft(brother);
	}
	deleteCase3(delete, parent, findBrother(delete));
}

/**
 * executes delete case 3E
 * @param delete the node to delete
 * @param parent the parent
 * @param brother the brother
 * @param farChild the far child of the brother
 */
void deleteCase3E(Node* delete, Node* parent, Node* brother, Node* farChild)
{
	Color temp = brother->color;
	brother->color = parent->color;
	parent->color = temp;
	if(parent->right == delete)
	{
		rotateRight(parent);
	}
	else
	{
		rotateLeft(parent);
	}
	farChild->color = BLACK;
}

/**
 * responsible on the third delete case
 * @param delete the node to delete
 * @param parent the parent of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCase3(Node* delete, Node* parent, Node* brother)
{
	if(delete->parent == NULL)
	{
		return;
	}
	else if(brother->color == BLACK && (brother->left == NULL || brother->left->color == BLACK) &&
			(brother->right == NULL || brother->right->color == BLACK))
	{
		if(parent->color == RED)
		{
			deleteCase3B1(parent, brother);
		}
		else
		{
			deleteCase3B2(parent, brother);
		}
	}
	else if(brother->color == RED)
	{
		deleteCase3C(delete, parent, brother);
	}
	else
	{
		Node* farChild = findFarChild(delete, brother);
		Node* closeChild = findCloseChild(delete, brother);
		if (brother->color == BLACK && (closeChild != NULL && closeChild->color == RED) &&
			(farChild == NULL || farChild->color == BLACK))
		{
			deleteCase3D(delete, parent, brother, closeChild);
		}
		else
		{
			deleteCase3E(delete, parent, brother, farChild);
		}
	}
}

/**
 * responsible of the delete cases
 * @param delete the node to delete
 * @param parent the parent of the node to delete
 * @param child the child of the node to delete
 * @param brother the brother of the node to delete
 */
void deleteCases(Node* delete, Node* parent, Node* child, Node* brother)
{
	if(delete->color == RED)
	{
		deleteCase1(delete, parent);
	}
	else if(child != NULL && delete->color == BLACK && child->color == RED)
	{
		deleteCase2(delete, child, parent);
	}
	else
	{
		deleteCase3(delete, parent, brother);
		if(parent->right == delete)
		{
			parent->right = NULL;
		}
		else
		{
			parent->left = NULL;
		}
	}

}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
	if(data == NULL)
	{
		return FAIL;
	}
	if(RBTreeContains(tree, data) == 0)
	{
		return FAIL;
	}
	Node* deleteNode = findLocation(tree, data);
	if(deleteNode->right != NULL && deleteNode->left != NULL)
	{
		deleteNode = changeWithSuccessor(deleteNode);
	}
	Node* child = findChild(deleteNode);
	Node* brother = findBrother(deleteNode);
	Node* parent = deleteNode->parent;
	if(parent == NULL)
	{
		tree->root = child;
		if(child != NULL)
		{
			child->color = BLACK;
			child->parent = NULL;
		}
	}
	else
	{
		deleteCases(deleteNode, parent, child, brother);
		tree->root = findNewRoot(parent);
	}
	tree->freeFunc(deleteNode->data);
	free(deleteNode);
	deleteNode = NULL;
	tree->size--;
	return SUCCESS;
}
