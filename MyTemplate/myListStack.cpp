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

class stack
{
public:
	stack(int initsize = 10);
	stack(const stack&);
	~stack();

	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& top() const { return firstNode->elem; }
	void pop();
	void push(const Elemtype& element);

private:
	chainNode* firstNode;
	int length;
};

stack::stack(int initsize)
{
	firstNode = NULL;
	length = 0;
}

stack::stack(const stack& list)
{
	length = list.length;
	if (length == 0)
	{
		firstNode = NULL;
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
		tarNode->next = NULL;
	}
}

stack::~stack()
{
	while (firstNode != NULL)
	{
		chainNode* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;
	}
}

void stack::pop()
{
	if (length == 0)
		throw range_error("This stack is already empty!");
	chainNode* delNode = firstNode;
	firstNode = firstNode->next;
	delete delNode;
	--length;
}

void stack::push(const Elemtype& elem)
{
	firstNode = new chainNode(elem, firstNode);
	++length;
}

