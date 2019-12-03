#pragma once
#include <cassert>

class Resource;

struct node
{
    Resource* res;
    node* next;
};

class Pool
{
    node* start;

    Pool(size_t size)
    {
        node* pool = new node[size];

        for (int i = 0; i < size - 1; i++)
        {
            pool[i].next = &pool[i + 1];
        }
        pool[size - 1].next = NULL;

        start = &pool[0];
    }

    ~Pool()
    {
    }

    void push(node* Node)
    {
        node* temp = start;
        start = Node;
        Node->next = temp;
    }

    node* pop()
    {
        assert(start != nullptr);
        node* value = start;
        start = start->next;
        return value;
    }
};

class Queue
{
    int rear;
    int front;
    Resource** arr;
    int MAXSIZE;

public:
    Queue()
    {
        front = rear = -1;
        MAXSIZE = 1000;
        arr = new Resource*[MAXSIZE];
    }

    ~Queue()
    {
    }

    void push(Resource* res)
    {
        if (front == -1)
        {
            front = 0;
        }
        rear = (rear + 1) % MAXSIZE;  // going round and round concept
        // inserting the element
        arr[rear] = res;
    }

    Resource* pop()
    {
        Resource* res = arr[front];
        if (front == rear)
        {
            // only one element in queue, reset queue after removal
            front = -1;
            rear = -1;
        }
        else
        {
            front = (front + 1) % MAXSIZE;
        }
        return res;
    }

    bool empty()
    {
        return (front == -1);
    }

    void swap(Queue& queue)
    {
        int temp1 = queue.rear;
        int temp2 = queue.front;
        Resource** temp3 = queue.arr;
        int temp4 = queue.MAXSIZE;

        queue.rear = rear;
        queue.front = front;
        queue.arr = arr;
        queue.MAXSIZE = MAXSIZE;

        rear = temp1;
        front = temp2;
        arr = temp3;
        MAXSIZE = temp4;
    }
};
