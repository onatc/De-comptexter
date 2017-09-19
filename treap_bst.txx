#include "treap_bst.h"

#include <iostream>
#include <stdexcept>
#include <stack>
#include <cassert>

/*
*  Purpose for program
*  This is a treap template.
*
*
*  Programmer: Onat Calik
*  Date: December 07, 2016
*
*  Honor Code: I have neither given nor received any unauthorized assistance with this program.
*/

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST()
	: root(nullptr)
{
	std::random_device rd;
	rndgen.seed(rd());
}

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST(const TreapBST& x)
{
	root = nullptr;
	clone(x.root);

}

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>& TreapBST<KeyType, ValueType>::operator=(
	TreapBST x)
{
	swap(*this, x);
	return *this;
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::swap(TreapBST& x, TreapBST& y)
{
	std::swap(x.root, y.root);
}


template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::~TreapBST()
{
	std::stack<Node<KeyType, ValueType>*> s;
	s.push(root);

	while (!s.empty()) {
		Node<KeyType, ValueType>* curr = s.top();
		s.pop();

		if (curr != nullptr) {
			s.push(curr->childl);
			s.push(curr->childr);
			delete curr;
		}
	}
	root = nullptr;
}

template <typename KeyType, typename ValueType>
bool TreapBST<KeyType, ValueType>::empty()
{
	if (root == nullptr)
	{
		return true;
	}

	return false;
}


template <typename KeyType, typename ValueType>
ValueType TreapBST<KeyType, ValueType>::search(const KeyType& key, bool& found)
{
	NodePtr temp;
	temp = root;
	found = false;

	
	while (found == false)
	{
		if (temp == nullptr)
		{
			return ValueType();
		}

		if (temp->key == key)
		{
			found = true;
			return temp->data;
		}
		
		else if (temp->key < key)
		{
			temp = temp->childr;
		}
		else if (temp->key > key)
		{
			temp = temp->childl;
		}

	}

	return ValueType();
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::insert(
	const KeyType& key, const ValueType& value)
{
	
	bool f;

	ValueType v;
	v = search(key, f);
	if (f)
	{
		throw std::logic_error("Key already exists");
	}
	
	root = recursiveInsert(root, key, value);

}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::remove(const KeyType& key)
{
	if (empty())
	{
		throw std::logic_error("Tree is empty");
	}

	root = recursiveDelete(root, key);

}

template <typename KeyType, typename ValueType>
typename TreapBST<KeyType, ValueType>::NodePtr TreapBST<KeyType, ValueType>::recursiveInsert(NodePtr myRoot, const KeyType& myKey, const ValueType& myValue)
{
	if (myRoot == nullptr)
	{
		NodePtr node = new Node<KeyType, ValueType>(myKey, myValue);
		node->priority = rndgen();
		myRoot = node;
		return myRoot;
	}

	if (myKey <= myRoot->key)
	{
		myRoot->childl = recursiveInsert(myRoot->childl, myKey, myValue);

		if (myRoot->childl->priority > myRoot->priority)
		{
			myRoot = rotateRight(myRoot);
		}
	}

	else
	{
		myRoot->childr = recursiveInsert(myRoot->childr, myKey, myValue);

		if (myRoot->childr->priority > myRoot->priority)
		{
			myRoot = rotateLeft(myRoot);
		}
	}

	return myRoot;
}

template <typename KeyType, typename ValueType>
typename TreapBST<KeyType, ValueType>::NodePtr TreapBST<KeyType, ValueType>::rotateRight(NodePtr myRoot)
{
	NodePtr newRoot = myRoot->childl;
	NodePtr newParent = newRoot->childr;

	newRoot->childr = myRoot;
	myRoot->childl = newParent;

	return newRoot;
}

template <typename KeyType, typename ValueType>
typename TreapBST<KeyType, ValueType>::NodePtr TreapBST<KeyType, ValueType>::rotateLeft(NodePtr myRoot)
{
	NodePtr newRoot = myRoot->childr;
	NodePtr newParent = newRoot->childl;

	newRoot->childl = myRoot;
	myRoot->childr = newParent;

	return newRoot;
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::clone(Node<KeyType, ValueType>* rhs)
{
	Node<KeyType, ValueType>** lhs = &root;

	std::stack<stackvar<KeyType, ValueType>> s;

	stackvar<KeyType, ValueType> rootvar;
	rootvar.rhs = rhs;
	rootvar.lhs = lhs;
	s.push(rootvar);

	while (!s.empty()) {
		stackvar<KeyType, ValueType> currentvar = s.top();
		s.pop();

		Node<KeyType, ValueType>* curr_rhs = currentvar.rhs;
		Node<KeyType, ValueType>** curr_lhs = currentvar.lhs;

		if (curr_rhs == nullptr)
			continue;

		// allocate new node and copy contents
		Node<KeyType, ValueType>* temp = new Node<KeyType, ValueType>(curr_rhs->key, curr_rhs->data);
		*curr_lhs = temp;

		// push childl subtree
		currentvar.rhs = curr_rhs->childl;
		currentvar.lhs = &((*curr_lhs)->childl);
		s.push(currentvar);

		// push childr subtree
		currentvar.rhs = curr_rhs->childr;
		currentvar.lhs = &((*curr_lhs)->childr);
		s.push(currentvar);
	}
}

template <typename KeyType, typename ValueType>
typename TreapBST<KeyType, ValueType>::NodePtr TreapBST<KeyType, ValueType>::recursiveDelete(NodePtr myRoot, const KeyType& myKey)
{
	
	if (myKey < myRoot->key)
	{
		myRoot->childl = recursiveDelete(myRoot->childl, myKey);
	}

	else if (myKey > myRoot->key)
	{
		myRoot->childr = recursiveDelete(myRoot->childr, myKey);
	}

	else if (myRoot->childl == nullptr)
	{
		Node<KeyType, ValueType>* temp = myRoot->childr;
		delete(myRoot);
		myRoot = temp;
	}

	else if (myRoot->childr == nullptr)
	{
		Node<KeyType, ValueType>* temp = myRoot->childl;
		delete(myRoot);
		myRoot = temp;
	}

	else if (myRoot->childl->priority < myRoot->childr->priority)
	{
		myRoot = rotateLeft(myRoot);
		myRoot->childl = recursiveDelete(myRoot->childl, myKey);
	}

	else
	{
		myRoot = rotateRight(myRoot);
		myRoot->childr = recursiveDelete(myRoot->childr, myKey);
	}

	return myRoot;
}

