#pragma once
#include <mutex>
#include <vector>
/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation

class shash
{
	struct Node{
		int value;
		Node* next;
		Node(int v):value(v),next(nullptr){}
	}; 
	struct sentinel{
		Node* head;
		mutable std::mutex lock;
	};
	unsigned _buckets;
	std::vector<sentinel> buckets;
public:
	shash(unsigned _buckets):_buckets(_buckets)
	{		
		for(int i = 0; i < _buckets; i++) {
			
			buckets[i].head = new Node(0);			
			
		}
	}
	

	/// insert *key* into the appropriate linked list if it doesn't already
	/// exist; return true if the key was added successfully.
	bool insert(int key)
	{
		sentinel* s  = &buckets[key%_buckets];
		std::lock_guard<std::mutex> lck(s->lock);
		Node* p = s->head;	
		while(p->next && p->next->value < key){
			p = p->next;
		}
		if(p->next && p->next->value == key){
			return false;
		}
		Node* node = new Node(key);
		node->next = p->next;
		p->next = node;
		return true;
		
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		sentinel* s  = &buckets[key%_buckets];
		std::lock_guard<std::mutex> lck(s->lock);
		Node* p = s->head;
		while(p->next && p->next->value < key) {
			p = p->next;
		}		
		if(p->next && p->next->value == key){
			Node *temp = p->next;
			p->next = p->next->next;
			delete temp;
			return true;
		}
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key) const
	{
		const sentinel* s  = &buckets[key%_buckets];
		std::lock_guard<std::mutex> lck(s->lock);
		Node* p = s->head;
		
		while(p->next && p->next->value < key){
			p = p->next;
		}
		if(p->next && p->next->value == key){
			return true;
		}
		return false;
		

	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return _buckets;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		int size = 0;
		const sentinel* s  = &buckets[bucket];
		Node* p = s->head;
		while(p->next){
				size++;
				p = p->next;
			}
		return size;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		const sentinel* s  = &buckets[bucket];
		Node* p = s->head;
		while(idx-- > 0){
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
