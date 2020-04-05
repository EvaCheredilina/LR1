#pragma once
#include <stdexcept>
#include "List.h"

template<typename KeyType, typename ValueType>
class Map
{
public: 
	enum NodeColor {BLACK, RED};

	class Node
	{
	public:
		Node(NodeColor color, KeyType key, ValueType value, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr):
			color(color), key(key), value(value), parent(parent), left(left), right(right)
		{
		}
		NodeColor color;
		Node* left;
		Node* right;
		Node* parent;
		KeyType key;
		ValueType value;
	};

	Node* nullLeaf;

	Node* root = nullptr;

	Node* sibling(Node* n)
	{
		if (n == n->parent->left)
			return n->parent->right;
		else
			return n->parent->left;
	}

	Node* grandparent(Node* node)
	{
		if ((node != nullLeaf) && (node->parent != nullLeaf))
			return node->parent->parent;
		else
			return nullLeaf;
	}

	Node* uncle(struct Node* node)
	{
		Node* g = grandparent(node);
		if (g == nullLeaf)
			return nullLeaf; // No grandparent means no uncle
		if (node->parent == g->left)
			return g->right;
		else
			return g->left;
	}

	void replace_node(Node* n, Node* child) {
		child->parent = n->parent;
		if (n == n->parent->left) {
			n->parent->left = child;
		}
		else {
			n->parent->right = child;
		}
	}

	void delete_one_child(Node* n)
	{
		/*
		 * Condition: does not have more than one nonzero descendant.
		 */
		Node* child;

		if (n->right == nullLeaf) child = n->left;
		else child = n->right;

		replace_node(n, child);
		if (n->color == BLACK) {
			if (child->color == RED)
				child->color = BLACK;
			else
				delete_case1(child);
		}

		if (n == root)
		{
			delete root;
			root = child;
		}
	}

	void rotate_left(Node* node)
	{
		Node* right = node->right;
		/* Create node->right link */
		node->right = right->left;
		if (right->left != nullLeaf)
			right->left->parent = node;
		/* Create right->parent link */
		if (right != nullLeaf)
			right->parent = node->parent;
		if (node->parent != nullLeaf) {
			if (node == node->parent->left)
				node->parent->left = right;
			else
				node->parent->right = right;
		}
		else {
			root = right;
		}
		right->left = node;
		if (node != nullLeaf)
			node->parent = right;
	}

	void rotate_right(Node* node)
	{
		Node* left = node->left;
		/* Create node->left link */
		node->left = left->right;
		if (left->right != nullLeaf)
			left->right->parent = node;
		/* Create left->parent link */
		if (left != nullLeaf)
			left->parent = node->parent;
		if (node->parent != nullLeaf) {
			if (node == node->parent->right)
				node->parent->right = left;
			else
				node->parent->left = left;
		}
		else {
			root = left;
		}
		left->right = node;
		if (node != nullLeaf)
			node->parent = left;
	}

	void insert_case1(Node* n)
	{
		if (n->parent == nullLeaf)
			n->color = BLACK;
		else
			insert_case2(n);
	}
	void insert_case2(Node* n)
	{
		if (n->parent->color == BLACK)
			return; /* Tree is still valid */
		else
			insert_case3(n);
	}
	void insert_case3(Node* n)
	{
		Node* u = uncle(n), * g;

		if ((u != nullLeaf) && (u->color == RED)) {
			// && (n->parent->color == RED) Второе условие проверяется в insert_case2, то есть родитель уже является красным.
			n->parent->color = BLACK;
			u->color = BLACK;
			g = grandparent(n);
			g->color = RED;
			insert_case1(g);
		}
		else {
			insert_case4(n);
		}
	}
	void insert_case4(Node* n)
	{
		Node* g = grandparent(n);

		if ((n == n->parent->right) && (n->parent == g->left)) {
			rotate_left(n->parent);

			/*
			 * rotate_left can be performed as follows, given that there are already *g =  grandparent(n)
			 *
			 * struct node *saved_p=g->left, *saved_left_n=n->left;
			 * g->left=n;
			 * n->left=saved_p;
			 * saved_p->right=saved_left_n;
			 *
			 */

			n = n->left;
		}
		else if ((n == n->parent->left) && (n->parent == g->right)) {
			rotate_right(n->parent);

			/*
			 * rotate_right can be performed as follows, given that there are already *g =  grandparent(n)
			 *
			 * struct node *saved_p=g->right, *saved_right_n=n->right;
			 * g->right=n;
			 * n->right=saved_p;
			 * saved_p->left=saved_right_n;
			 *
			 */

			n = n->right;
		}
		insert_case5(n);
	}
	void insert_case5(Node* n)
	{
		Node* g = grandparent(n);

		n->parent->color = BLACK;
		g->color = RED;
		if ((n == n->parent->left) && (n->parent == g->left)) {
			rotate_right(g);
		}
		else { /* (n == n->parent->right) && (n->parent == g->right) */
			rotate_left(g);
		}
	}

	void delete_case1(Node* n)
	{
		if (n->parent != nullLeaf)
			delete_case2(n);
	}
	void delete_case2(Node* n)
	{
		Node* s = sibling(n);

		if (s->color == RED) {
			n->parent->color = RED;
			s->color = BLACK;
			if (n == n->parent->left)
				rotate_left(n->parent);
			else
				rotate_right(n->parent);
		}
		delete_case3(n);
	}
	void delete_case3(Node* n)
	{
		Node* s = sibling(n);

		if ((n->parent->color == BLACK) &&
			(s->color == BLACK) &&
			(s->left->color == BLACK) &&
			(s->right->color == BLACK)) {
			s->color = RED;
			delete_case1(n->parent);
		}
		else
			delete_case4(n);
	}
	void delete_case4(Node* n)
	{
		Node* s = sibling(n);

		if ((n->parent->color == RED) &&
			(s->color == BLACK) &&
			(s->left->color == BLACK) &&
			(s->right->color == BLACK)) {
			s->color = RED;
			n->parent->color = BLACK;
		}
		else
			delete_case5(n);
	}
	void delete_case5(Node* n)
	{
		Node* s = sibling(n);

		if (s->color == BLACK) { /* this if statement is trivial,
	due to case 2 (even though case 2 changed the sibling to a sibling's child,
	the sibling's child can't be red, since no red parent can have a red child). */
	/* the following statements just force the red to be on the left of the left of the parent,
	   or right of the right, so case six will rotate correctly. */
			if ((n == n->parent->left) &&
				(s->right->color == BLACK) &&
				(s->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */
				s->color = RED;
				s->left->color = BLACK;
				rotate_right(s);
			}
			else if ((n == n->parent->right) &&
				(s->left->color == BLACK) &&
				(s->right->color == RED)) {/* this last test is trivial too due to cases 2-4. */
				s->color = RED;
				s->right->color = BLACK;
				rotate_left(s);
			}
		}
		delete_case6(n);
	}
	void delete_case6(Node* n)
	{
		Node* s = sibling(n);

		s->color = n->parent->color;
		n->parent->color = BLACK;

		if (n == n->parent->left) {
			s->right->color = BLACK;
			rotate_left(n->parent);
		}
		else {
			s->left->color = BLACK;
			rotate_right(n->parent);
		}
	}

	void tree_clearing(Node* root)
	{
		if (root != nullptr && root != nullLeaf)
		{
			tree_clearing(root->left);
			tree_clearing(root->right);
			delete root;
			root = nullptr;
		}
		if (root == nullLeaf) root = nullptr;
	}

	void fillListOfValues(Node* root, List<ValueType>* list)
	{
		if (root != nullLeaf && root != nullptr)
		{
			list->push_back(root->value);
			fillListOfValues(root->left, list);
			fillListOfValues(root->right, list);
		}
	}

	void fillListOfKeys(Node* root, List<KeyType>* list)
	{
		if (root != nullLeaf && root != nullptr)
		{
			list->push_back(root->key);
			fillListOfKeys(root->left, list);
			fillListOfKeys(root->right, list);
		}
	}

	void printToConsole(Node* root)
	{
		if (root != nullLeaf)
		{
			std::cout << "Key: " << root->key << " " << "Value: " << root->value << endl;
			printToConsole(root->left);
			printToConsole(root->right);
		}
	}

	Map()
	{
		nullLeaf = new Node(BLACK, 0, 0, nullLeaf);
		nullLeaf->left = nullptr;
		nullLeaf->right = nullptr;
	}


	void insert(KeyType key, ValueType value)
	{
		Node* temporary = root;
		Node* parent = nullLeaf;
			
		if (root == nullptr)
		{
			root = new Node(BLACK, key, value, parent, nullLeaf, nullLeaf);
			return;
		}

		//Finding the nullLeaf elements that fits our key
		while (temporary != nullLeaf)
		{
			parent = temporary;
			if (key < temporary->key)
				temporary = temporary->left;
			else
				temporary = temporary->right;
		}

		temporary = new Node(RED, key, value, parent, nullLeaf, nullLeaf);

		if (key < parent->key)
			parent->left = temporary;
		else parent->right = temporary;


		insert_case1(temporary);
	}

	void remove(KeyType key)
	{
		Node* temporary = root;

		while (temporary != nullLeaf && temporary->key != key)
		{
			if (temporary->key > key)
				temporary = temporary->left;
			else temporary = temporary->right;
		}

		if (temporary == nullLeaf)
			throw std::invalid_argument("No such key in a map");


		delete_one_child(temporary);
	}
	
	ValueType find(KeyType key)
	{
		Node* temporary = root;
		
		if (root == nullptr) throw "No such key in a map";

		while (temporary != nullLeaf && temporary->key != key)
		{
			if (temporary->key > key)
				temporary = temporary->left;
			else temporary = temporary->right;
		}

		if(temporary == nullLeaf)
			throw std::invalid_argument("No such key in a map");

		return temporary->value;
	}

	void clear()
	{
		tree_clearing(root);
		root = nullptr;
	}

	List<KeyType> get_keys()
	{
		List<KeyType>* list = new List<KeyType>();

		fillListOfKeys(root, list);

		return *list;
	}

	List<ValueType> get_values()
	{
		List<ValueType>* list = new List<ValueType>();

		fillListOfValues(root, list);

		return *list;
	}

	void print()
	{
		if (root == nullptr) throw "Tree is empty";
		printToConsole(root);
	}
};