#pragma once

/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation.
///       In addition, the API now allows for multiple keys on each
///       operation.
#include <iostream>
#include<mutex>
using namespace std;

class shash2
{

  struct Node {
    int value;
    Node *next;
    Node(int v) : value(v), next(nullptr) { }
  };

  mutable std::vector<std::mutex> locks;
  std::vector<Node*> sentinels;
  std::vector<int> sizes;
  int bucks;

  bool insert(int idx, int key)
  {
    std::lock_guard<std::mutex> lock_guard(locks[idx]);
    Node *p = sentinels[idx];
    while (p->next) {
      if (p->next->value < key) {
        p = p->next;
      }
      else if (p->next->value == key) {
        return false;
      }
      else {
        break;
      }
    }

    sizes[idx]++;
    Node *new_node = new Node(key);
    new_node->next = p->next;
    p->next = new_node;
    return true;
  }

  bool remove(int idx, int key)
  {
    std::lock_guard<std::mutex> lock_guard(locks[idx]);
    Node *p = sentinels[idx];
    while (p->next) {
      if (p->next->value < key) {
        p = p->next;
      }
      else if (p->next->value >= key) {
        break;
      }
    }

    if (p->next && p->next->value == key) {
      sizes[idx]--;
      p->next = p->next->next;
      return true;
    }

    return false;
  }

  bool lookup(int idx, int key) const
  {
    if (idx >= bucks) {
      cout << "here " << endl;
    }
    std::lock_guard<std::mutex> lock_guard(locks[idx]);
    Node *p = sentinels[idx]->next;
    while (p && p->value < key)
      p = p->next;

    return p ? p->value == key : false;
  }

 public:
	shash2(unsigned _buckets): bucks(_buckets), locks(_buckets), sizes(_buckets, 0)
    {
      for (int i = 0; i < bucks; ++i) {
        sentinels.push_back(new Node(-1));
      }
    }

	/// insert /num/ values from /keys/ array into the hash, and return the
	/// success/failure of each insert in /results/ array.
	void insert(int* keys, bool* results, int num)
	{
      for (int i = 0; i < num; ++i) {
        int idx = keys[i] % bucks;
        results[i] = insert(idx, keys[i]);
      }
    }
	/// remove *key* from the list if it was present; return true if the key
	/// was removed successfully.
	void remove(int* keys, bool* results, int num)
	{
      for (int i = 0; i < num; ++i) {
        int idx = keys[i] % bucks;
        results[i] = remove(idx, keys[i]);
      }
    }
	/// return true if *key* is present in the list, false otherwise
	void lookup(int* keys, bool* results, int num) const
	{
      for (int i = 0; i < num; ++i) {
        int idx = keys[i] % bucks;
        results[i] = lookup(idx, keys[i]);
      }
    }

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return bucks;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		return sizes[bucket];
	}
	int getElement(size_t bucket, size_t idx) const
	{
        if (idx >= sizes[bucket])
          return -1;
        Node *p = sentinels[bucket]->next;
        while (idx-- > 0) {
          p = p->next;
        }
        return p->value;
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};
