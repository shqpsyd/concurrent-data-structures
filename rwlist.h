#pragma once

/// TODO: complete this implementation of a thread-safe (concurrent) sorted
/// linked list of integers, which should use readers/writer locking.

#include <pthread.h>

static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

class rwlist
{
  /// a Node consists of a value and a pointer to another node
  struct Node{
    int value;
    Node *next;
    Node(int v) : value(v), next(nullptr) { }
  };


  /// The head of the list is referenced by this pointer
  Node* sentinel;

  //size
  int size = 0;

 public:
  rwlist(int)
      : sentinel(new Node(0))
  { }

  /// insert *key* into the linked list if it doesn't already exist; return
  /// true if the key was added successfully.
  bool insert(int key)
  {
    pthread_rwlock_wrlock(&rwlock);
    Node *new_node = new Node(key);
    Node *p = sentinel;
    while (p->next && p->next->value < key) {
      p = p->next;
    }

    if (p->next && p->next->value == key) {
      pthread_rwlock_unlock(&rwlock);
      return false;
    }

    ++size;
    new_node->next = p->next;
    p->next = new_node;
    pthread_rwlock_unlock(&rwlock);
    return true;
  }
  /// remove *key* from the list if it was present; return true if the key
  /// was removed successfully.
  bool remove(int key)
  {
    pthread_rwlock_wrlock(&rwlock);
    Node *p = sentinel;
    while (p->next && p->next->value < key) {
      p = p->next;
    }

    if (p->next && p->next->value == key) {
      --size;
      p->next = p->next->next;
      pthread_rwlock_unlock(&rwlock);
      return true;
    }

    pthread_rwlock_unlock(&rwlock);
    return false;
  }
  /// return true if *key* is present in the list, false otherwise
  bool lookup(int key) const
  {
    pthread_rwlock_rdlock(&rwlock);
    Node *p = sentinel;
    while (p->next && p->next->value < key)
      p = p->next;

    if (p->next && p->next->value == key) {
      pthread_rwlock_unlock(&rwlock);
      return true;
    }

    pthread_rwlock_unlock(&rwlock);
    return false;
  }


  //The following are not tested by the given tester but are required for grading
  //No locks are required for these.
  size_t getSize() const
  {
    return size;
  }
  int getElement(size_t idx) const
  {
    if (idx >= size)
      return -1;
    Node *p = sentinel->next;
    while (idx-- > 0) {
      p = p->next;
    }
    return p->value;
  }


  //These functions just need to exist, they do not need to do anything
  size_t getBucketSize(size_t bucket) const
  {
    return 0;
  }
  int getElement(size_t bucket, size_t idx) const
  {
    return 0;
  }
};
