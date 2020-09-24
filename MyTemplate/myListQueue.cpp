#include <iostream>
#include <stdexcept>

using namespace std;
using Elemtype = int;

struct chainNode
{
	Elemtype elem;
	chainNode* next;

	chainNode() {};
	chainNode(const Elemtype& element) : elem(element) {}
	chainNode(const Elemtype& element, chainNode* next) : elem(element), next(next) {}
};

class queue
{
public:
	queue(int initsize = 10);
	queue(const queue&);
	~queue();

	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& front() const { return firstNode->elem; }
	Elemtype& back() const { return lastNode->elem; }
	void pop();
	void push(const Elemtype&);

private:
	chainNode* firstNode;
	chainNode* lastNode;
	int length;
};

queue::queue(int initsize)
{
	firstNode = NULL;
	lastNode = NULL;
	length = 0;
}

queue::queue(const queue& list)
{
	length = list.length;
	if (length == 0)
	{
		firstNode = NULL;
		lastNode = NULL;
	}
	else
	{
		firstNode = new chainNode(list.firstNode->elem);
		chainNode* curNode = list.firstNode;
		chainNode* tarNode = firstNode;
		curNode = curNode->next;
		while (curNode != NULL)
		{
			tarNode->next = new chainNode(curNode->elem);
			tarNode = tarNode->next;
			curNode = curNode->next;
		}
		lastNode = tarNode;
		lastNode->next = NULL;
	}
}

queue::~queue()
{
	while (firstNode != NULL)
	{
		chainNode* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;
	}
}

void queue::pop()
{
	if (length == 0)
		throw range_error("This queue is already empty!");
	chainNode* delNode = firstNode;
	firstNode = firstNode->next;
	delete delNode;
	--length;
}

void queue::push(const Elemtype& element)
{
	if (length == 0)
	{
		firstNode = new chainNode(element, NULL);
		lastNode = firstNode;
	}
	else
	{
		lastNode->next = new chainNode(element, NULL);
		lastNode = lastNode->next;
	}
	++ length;
}