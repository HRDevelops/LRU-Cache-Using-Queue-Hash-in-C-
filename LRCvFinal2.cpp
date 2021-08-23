// LRCvFinal2.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include<queue>
#include <cstdlib>
#include <cmath>
#include <windows.h>

using namespace std;

// A Queue Node (Queue is implemented using Doubly Linked List)

typedef struct QNode

{

	QNode *prev, *next;

	unsigned valueN;

} QNode;

// A Queue (A FIFO collection of Queue Nodes)

typedef struct Queue

{

	unsigned count;

	unsigned numberOfFrames;

	QNode *front, *rear;

} Queue;

// A hash (Collection of pointers to Queue Nodes)

typedef struct Hash

{

	int capacity;

	QNode **array;

} Hash;

// A utility function to create a new Queue Node.

QNode* newQNode(unsigned valueN)

{

	QNode* temp = new QNode;

	temp->valueN = valueN;

	temp->prev = temp->next = NULL;

	return temp;

}

// A utility function to create an empty Queue.

Queue* createQueue(int numberOfFrames)

{

	Queue* queue = new Queue;

	queue->count = 0;

	queue->front = queue->rear = NULL;

	queue->numberOfFrames = numberOfFrames;

	return queue;

}

// A utility function to create an empty Hash of given capacity

Hash* createHash(int capacity)

{

	Hash* hash = new Hash;

	hash->capacity = capacity;

	hash->array = new QNode*[hash->capacity];

	int i;

	for (i = 0; i < hash->capacity; ++i)

		hash->array[i] = NULL;

	return hash;

}

// A function to check if there is slot available in memory

int AreAllFramesFull(Queue* queue)

{

	return queue->count == queue->numberOfFrames;

}

// A utility function to check if queue is empty

int isQueueEmpty(Queue* queue)

{

	return queue->rear == NULL;

}

// A utility function to delete a frame from queue

void deQueue(Queue* queue)

{

	if (isQueueEmpty(queue))

		return;

	if (queue->front == queue->rear)

		queue->front = NULL;

	QNode* temp = queue->rear;

	queue->rear = queue->rear->prev;

	if (queue->rear)

		queue->rear->next = NULL;

	free(temp);

	queue->count--;

}

// A function to add a page with given 'valueN' to both queue and hash

void Enqueue(Queue* queue, Hash* hash, unsigned valueN)

{

	if (AreAllFramesFull(queue))

	{

		hash->array[queue->rear->valueN] = NULL;

		deQueue(queue);

	}

	QNode* temp = newQNode(valueN);

	temp->next = queue->front;

	if (isQueueEmpty(queue))

		queue->rear = queue->front = temp;

	else

	{

		queue->front->prev = temp;

		queue->front = temp;

	}

	hash->array[valueN] = temp;

	queue->count++;

}

// This function is called when a page with given 'valueN' is referenced from cache (or memory).

void CacheValue(Queue* queue, Hash* hash, unsigned valueN)

{

	QNode* val = hash->array[valueN];

	if (val == NULL)

		Enqueue(queue, hash, valueN);

	else if (val != queue->front)

	{

		val->prev->next = val->next;

		if (val->next)

			val->next->prev = val->prev;

		if (val == queue->rear)

		{

			queue->rear = val->prev;

			queue->rear->next = NULL;

		}

		val->next = queue->front;

		val->prev = NULL;

		val->next->prev = val;

		queue->front = val;

	}

}

// Main

int main()

{
	system("Color 0A");
	Queue* q = createQueue(4);

	Hash* hash = createHash(10);

	CacheValue(q, hash, 1);

	CacheValue(q, hash, 2);

	CacheValue(q, hash, 3);

	CacheValue(q, hash, 1);

	CacheValue(q, hash, 4);

	CacheValue(q, hash, 5);

	cout << "------------------Values In Cache Memory------------------";

	cout<<"\n\t\t" << q->front->valueN << " \n";

	cout << "\t\t" << q->front->next->valueN << "\n";

	cout << "\t\t" << q->front->next->next->valueN << "\n";

	cout << "\t\t" << q->front->next->next->next->valueN << "\n";

	return 0;

}