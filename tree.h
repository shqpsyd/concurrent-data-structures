#pragma once

#include <cstdlib>
#include <algorithm>
#include <mutex>
#include <thread>
/// TODO: complete this implementation of a thread-safe (concurrent) AVL
///       tree of integers, implemented as a set of Node objects.
///       In addition, the API now allows for multiple insertions on each
///       operation.
///       Less than goes to the left. Greater than goes to the right.
class tree
{
//This is for testing purposes. Do not access your Node directly.
 public:
  struct Node
  {
   public:
	int value;
	Node* left;
	Node* right;
	int height;
	explicit Node(int v) : value(v), height(1), left(nullptr), right(nullptr) { }
  };

  Node* root;

  int height(Node *root) {
	  if (root == nullptr)
		  return 0;
	  return root->height;
  }

  int NumberOfNodes(Node *root) const {
	  if (!root)
		  return 0;
	  return NumberOfNodes(root->left) + NumberOfNodes(root->right) + 1;
  }

  // Get Balance factor of node N
  int getBalance(Node *root)
  {
	  if (root == nullptr)
		  return 0;
	  return height(root->left) - height(root->right);
  }

  Node* leftRotate(Node *root) {
	  Node *newRoot = root->right;
	  root->right = newRoot->left;
	  newRoot->left = root;

	  root->height = std::max(height(root->left), height(root->right)) + 1;
	  newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
	  return newRoot;
  }

  Node* rightRotate(Node *root) {
	  Node *newRoot = root->left;
	  root->left = newRoot->right;
	  newRoot->right = root;

	  root->height = std::max(height(root->left), height(root->right)) + 1;
	  newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
	  return  newRoot;
  }

  Node* insert(Node *root, int v, bool &result) {
	  if (!root) {
		  ++size;
		  result = true;
		  return new Node(v);
	  }

	  if (root->value < v)
		  root->right = insert(root->right, v, result);
	  else if (root->value > v)
		  root->left = insert(root->left, v, result);
	  else
		  return root;

	  root->height = std::max(height(root->left), height(root->right)) + 1;

	  int balance = getBalance(root);

	  // single right rotate
	  if (balance > 1 && v < root->left->value)
		  return rightRotate(root);

	  // single left rotate
	  if (balance < -1 && v > root->right->value)
		  return leftRotate(root);

	  // left right rotate
	  if (balance > 1 && v > root->left->value) {
		  root->left = leftRotate(root->left);
		  return rightRotate(root);
	  }

	  // right left rotate
	  if (balance < -1 && v < root->right->value) {
		  root->right = rightRotate(root->right);
		  return leftRotate(root);
	  }

	  // directly return root if balanced
	  return root;
  }

  Node* inorderSuc(Node *root) {
	  root = root->right;
	  while (root->left)
		  root = root->left;
	  return root;
  }

  Node* remove(Node *root, int v, bool &result) {
	  if (root == nullptr)
		  return root;

	  if (v < root->value)
		  root->left = remove(root->left, v, result);
	  else if(v > root->value)
		  root->right = remove(root->right, v, result);
	  else
	  {
		  --size;
		  result = true;
		  // node with only one child or no child
		  if( (root->left == nullptr) || (root->right == nullptr) )
		  {
			  Node *temp = root->left ? root->left : root->right;
			  delete root;
			  // No child case
			  if (temp == nullptr)
			  {
				  return nullptr;
			  }
			  else // One child case
				  return temp;
		  }
		  else
		  {
			  // node with two children
			  Node* temp = inorderSuc(root);
			  root->value = temp->value;
			  ++size; // trick
			  root->right = remove(root->right, temp->value, result);
		  }
	  }

	  root->height = 1 + std::max(height(root->left), height(root->right));

	  int balance = getBalance(root);

	  // Left Left Case
	  if (balance > 1 && getBalance(root->left) >= 0)
		  return rightRotate(root);

	  // Left Right Case
	  if (balance > 1 && getBalance(root->left) < 0)
	  {
		  root->left =  leftRotate(root->left);
		  return rightRotate(root);
	  }

	  // Right Right Case
	  if (balance < -1 && getBalance(root->right) <= 0)
		  return leftRotate(root);

	  // Right Left Case
	  if (balance < -1 && getBalance(root->right) > 0)
	  {
		  root->right = rightRotate(root->right);
		  return leftRotate(root);
	  }

	  return root;
  }

  bool lookup(Node *root, int v) const {
	  if (!root)
		  return false;
	  if (root->value < v)
		  return lookup(root->right, v);
	  else if (root->value > v)
		  return lookup(root->left, v);
	  else
		  return true;
  }

  mutable std::mutex global_lock;

  size_t size = 0;

 public:
  explicit tree(int)
		  : root(nullptr)
  {}

  /// insert /num/ values from /data/ array into the tree, and return the
  /// success/failure of each insert in /results/ array.
  void insert(int* data, bool* results, int num)
  {
	  for (int i = 0; i < num; ++i) {
		  global_lock.lock();
		  root = insert(root, data[i], results[i]);
		  global_lock.unlock();
	  }
  }
  /// remove *data* from the list if it was present; return true if the data
  /// was removed successfully.
  void remove(int* data, bool* results, int num)
  {
	  for (int i = 0; i < num; ++i) {
		  global_lock.lock();
		  root = remove(root, data[i], results[i]);
		  global_lock.unlock();
	  }
  }
  /// return true if *data* is present in the list, false otherwise
  void lookup(int* data, bool* results, int num) const
  {
	  for (int i = 0; i < num; ++i) {
		  global_lock.lock();
		  results[i] = lookup(root, data[i]);
		  global_lock.unlock();
	  }
  }

  //The following are not tested by the given tester but are required for grading
  //No locks are required for these.

  //Total number of elements in the tree
  size_t getSize() const
  {
	  return size;
  }

  int getElement(size_t idx) const
  {
	  return getElement(idx + 1, root);
  }


  //These functions need to exist, they do not need to do anyting
  size_t getBucketSize() const
  {
	  return 0;
  }
  int getElement(size_t bucket, size_t idx) const
  {
	  return 0;
  }



 private:
  //Returns the idx'th element from the tree assuming an inorder traversal.
  int getElement(size_t k, Node* root) const
  {
	  int leftNodes = NumberOfNodes(root->left);
	  if (leftNodes >= k) {
		  return getElement(k, root->left);
	  }
	  else if (leftNodes == k - 1)
		  return root->value;
	  else
		  return getElement(k - leftNodes - 1, root->right);
  }
};
