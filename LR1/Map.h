#pragma once
#include <stdexcept>
#include "List.h"

template<typename KeyType, typename ValueType>
class Map
{
public:
	enum NodeColor { BLACK, RED };

	class Node
	{
	public:
		Node(NodeColor color, KeyType key, ValueType value, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) :
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

	Node* sibling(Node* node)
	{
		if (node == node->parent->left)
			return node->parent->right;
		else
			return node->parent->left;
	}

	Node* grandparent(Node* node)
	{
		if ((node != nullLeaf) && (node->parent != nullLeaf))
			return node->parent->parent;
		else
			return nullLeaf;
	}

	Node* uncle(Node* node)
	{
		Node* g = grandparent(node);
		if (g == nullLeaf)
			return nullLeaf; // No grandparent means no uncle
		if (node->parent == g->left)
			return g->right;
		else
			return g->left;
	}

	void replace_node(Node* node, Node* child) {
		child->parent = node->parent;
		if (node == node->parent->left) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}
	}

	void delete_one_child(Node* node)
	{
		Node* child;

		if (node->right == nullLeaf) child = node->left;
		else child = node->right;

		replace_node(node, child);
		if (node->color == BLACK) {
			if (child->color == RED)
				child->color = BLACK;
			else
				delete_case1(child);
		}

		if (node == root)
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

	void insert_case1(Node* node)
	{
		if (node->parent == nullLeaf)
			node->color = BLACK;
		else
			insert_case2(node);
	}
	void insert_case2(Node* node)
	{
		if (node->parent->color == BLACK)
			return; /* Tree is still valid */
		else
			insert_case3(node);
	}
	void insert_case3(Node* node)
	{
		Node* u = uncle(node), * g;

		if ((u != nullLeaf) && (u->color == RED)) {
			// && (node->parent->color == RED) Второе условие проверяется в insert_case2, то есть родитель уже является красным.
			node->parent->color = BLACK;
			u->color = BLACK;
			g = grandparent(node);
			g->color = RED;
			insert_case1(g);
		}
		else {
			insert_case4(node);
		}
	}
	void insert_case4(Node* node)
	{
		Node* g = grandparent(node);

		if ((node == node->parent->right) && (node->parent == g->left)) {
			rotate_left(node->parent);

			/*
			 * rotate_left может быть выполнен следующим образом, учитывая что уже есть *g =  grandparent(node)
			 *
			 * struct node *saved_p=g->left, *saved_left_n=node->left;
			 * g->left=node;
			 * node->left=saved_p;
			 * saved_p->right=saved_left_n;
			 *
			 */

			node = node->left;
		}
		else if ((node == node->parent->left) && (node->parent == g->right)) {
			rotate_right(node->parent);

			/*
			 * rotate_right может быть выполнен следующим образом, учитывая что уже есть *g =  grandparent(node)
			 *
			 * struct node *saved_p=g->right, *saved_right_n=node->right;
			 * g->right=node;
			 * node->right=saved_p;
			 * saved_p->left=saved_right_n;
			 *
			 */

			node = node->right;
		}
		insert_case5(node);
	}
	void insert_case5(Node* node)
	{
		Node* g = grandparent(node);

		node->parent->color = BLACK;
		g->color = RED;
		if ((node == node->parent->left) && (node->parent == g->left)) {
			rotate_right(g);
		}
		else { /* (node == node->parent->right) && (node->parent == g->right) */
			rotate_left(g);
		}
	}

	void delete_case1(Node* node)
	{
		if (node->parent != nullLeaf)
			delete_case2(node);
	}
	void delete_case2(Node* node)
	{
		Node* s = sibling(node);

		if (s->color == RED) {
			node->parent->color = RED;
			s->color = BLACK;
			if (node == node->parent->left)
				rotate_left(node->parent);
			else
				rotate_right(node->parent);
		}
		delete_case3(node);
	}
	void delete_case3(Node* node)
	{
		Node* s = sibling(node);

		if ((node->parent->color == BLACK) &&
			(s->color == BLACK) &&
			(s->left->color == BLACK) &&
			(s->right->color == BLACK)) {
			s->color = RED;
			delete_case1(node->parent);
		}
		else
			delete_case4(node);
	}
	void delete_case4(Node* node)
	{
		Node* s = sibling(node);

		if ((node->parent->color == RED) &&
			(s->color == BLACK) &&
			(s->left->color == BLACK) &&
			(s->right->color == BLACK)) {
			s->color = RED;
			node->parent->color = BLACK;
		}
		else
			delete_case5(node);
	}
	void delete_case5(Node* node)
	{
		Node* s = sibling(node);

		if (s->color == BLACK) { /* this if statement is trivial,
	due to case 2 (even though case 2 changed the sibling to a sibling's child,
	the sibling's child can't be red, since no red parent can have a red child). */
	/* the following statements just force the red to be on the left of the left of the parent,
	   or right of the right, so case six will rotate correctly. */
			if ((node == node->parent->left) &&
				(s->right->color == BLACK) &&
				(s->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */
				s->color = RED;
				s->left->color = BLACK;
				rotate_right(s);
			}
			else if ((node == node->parent->right) &&
				(s->left->color == BLACK) &&
				(s->right->color == RED)) {/* this last test is trivial too due to cases 2-4. */
				s->color = RED;
				s->right->color = BLACK;
				rotate_left(s);
			}
		}
		delete_case6(node);
	}
	void delete_case6(Node* node)
	{
		Node* s = sibling(node);

		s->color = node->parent->color;
		node->parent->color = BLACK;

		if (node == node->parent->left) {
			s->right->color = BLACK;
			rotate_left(node->parent);
		}
		else {
			s->left->color = BLACK;
			rotate_right(node->parent);
		}
	}

	void tree_clearing(Node* nodeClearFrom)
	{
		if (nodeClearFrom != nullptr && nodeClearFrom != nullLeaf)
		{
			tree_clearing(nodeClearFrom->left);
			tree_clearing(nodeClearFrom->right);
			delete nodeClearFrom;
		}
	}

	void fillListOfValues(Node* nodeFillFrom, List<ValueType>* list)
	{
		if (nodeFillFrom != nullLeaf && nodeFillFrom != nullptr)
		{
			list->push_back(nodeFillFrom->value);
			fillListOfValues(nodeFillFrom->left, list);
			fillListOfValues(nodeFillFrom->right, list);
		}
	}

	void fillListOfKeys(Node* nodeFillFrom, List<KeyType>* list)
	{
		if (nodeFillFrom != nullLeaf && nodeFillFrom != nullptr)
		{
			list->push_back(nodeFillFrom->key);
			fillListOfKeys(nodeFillFrom->left, list);
			fillListOfKeys(nodeFillFrom->right, list);
		}
	}

	void printToConsole(Node* nodePrintFrom)
	{
		if (nodePrintFrom != nullLeaf)
		{
			std::cout << "Key: " << nodePrintFrom->key << " " << "Value: " << nodePrintFrom->value << endl;
			printToConsole(nodePrintFrom->left);
			printToConsole(nodePrintFrom->right);
		}
	}

	Map()
	{
		nullLeaf = new Node(BLACK, 0, 0, nullLeaf);
		nullLeaf->left = nullptr;
		nullLeaf->right = nullptr;
	}

	~Map()
	{
		tree_clearing(root);
		delete nullLeaf;
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

		if (temporary == nullLeaf)
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