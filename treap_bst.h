#ifndef _TREAP_BST_H_
#define _TREAP_BST_H_

#include <random>

#include "abstract_bst.h"

template <typename KeyType, typename ValueType>
class TreapBST : public AbstractBST<KeyType, ValueType>
{
public:
	// constructor
	TreapBST();

	// copy constructor
	TreapBST(const TreapBST& x);
	// copy-assignment
	TreapBST& operator=(TreapBST x);

	// swap
	void swap(TreapBST& x, TreapBST& y);

	// destructor
	~TreapBST();

	// from interface
	bool empty();

	// from interface
	ValueType search(const KeyType& key, bool& found);

	// from interface
	void insert(const KeyType& key, const ValueType& value);

	// from interface
	void remove(const KeyType& key);




private:
	// random number generator
	std::mt19937 rndgen;

	template <typename K, typename V>
	struct Node {
		K key;
		V data;
		std::mt19937::result_type priority;
		Node* parent;
		Node* childl;
		Node* childr;

		Node(const K& k, const V& d, Node* p = nullptr)
			: key(k)
			, data(d)
			, parent(p)
			, childl(nullptr)
			, childr(nullptr)
		{
		}
	};

	typedef Node<KeyType, ValueType>* NodePtr;

	NodePtr root;

	//helper function to insert recursively
	NodePtr recursiveInsert(NodePtr myRoot, const KeyType& myKey, const ValueType& myValue);
	
	//helper function to delete recursively
	NodePtr recursiveDelete(NodePtr myRoot, const KeyType& myKey);
	
	//Helper function to maintain order
	NodePtr rotateRight(NodePtr myRoot);
	
	//Helper function to maintain order
	NodePtr rotateLeft(NodePtr myRoot);

	//used for copy constructor
	template <typename K, typename I>
	struct stackvar {
		Node<K, I>* rhs;
		Node<K, I>** lhs;
	};

	//copy constructor helper function
	void clone(Node<KeyType, ValueType>* rhs);

};

#include "treap_bst.txx"

#endif // _TREAP_BST_H_
