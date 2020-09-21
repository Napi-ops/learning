#include <iostream>
#include <stdexcept>

using namespace std;
using Elemtype = int;

struct chainNode
{
	Elemtype elem;
	chainNode* prior;
	chainNode* next;

	chainNode() {};
	chainNode(const Elemtype& element) : elem(element) {}
	chainNode(const Elemtype& element, chainNode* prior, chainNode* next) : elem(element), prior(prior), next(next) {}
};

class myList_Dul
{
public:
	myList_Dul(int initsize = 10);
	myList_Dul(const myList_Dul&);
	~myList_Dul();

	void clear();
	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& get(int index) const;
	int indexOf(const Elemtype& element) const;
	void insert(int index, const Elemtype& element);
	void push_back(const Elemtype& element);
	void erase(int index);
	void output() const;

private:
	void checkIndex(int index) const;
	chainNode* firstNode;
	int length;
};

void myList_Dul::checkIndex(int index) const
{
	if (index < 0 || index >= length)
	{
		throw out_of_range("This index is out of range");
	}
}

myList_Dul::myList_Dul(int initsize)
{
	firstNode = NULL;
	length = 0;
}

myList_Dul::myList_Dul(const myList_Dul& list)
{
	length = list.length;
	if (length == 0)
	{
		firstNode = NULL;
	}
	else
	{
		firstNode = new chainNode(list.firstNode->elem);
		firstNode->prior = NULL;
		chainNode* curNode = list.firstNode;
		chainNode* tarNode = firstNode;
		curNode = curNode->next;
		while (curNode != NULL)
		{
			tarNode->next = new chainNode(curNode->elem);
			tarNode->next->prior = tarNode;
			tarNode = tarNode->next;
			curNode = curNode->next;
		}
		tarNode->next = NULL;
	}
}

myList_Dul::~myList_Dul()
{
	while (firstNode != NULL)
	{
		chainNode* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;
	}
}

void myList_Dul::clear()
{
	chainNode* curNode = firstNode;
	while (curNode != NULL)
	{
		curNode->elem = 0;
		curNode = curNode->next;
	}
}

Elemtype& myList_Dul::get(int index) const
{
	checkIndex(index);
	chainNode* curNode = firstNode;
	for (int i = 0; i != index; ++i)
	{
		curNode = curNode->next;
	}
	return curNode->elem;
}

int myList_Dul::indexOf(const Elemtype& element) const
{
	chainNode* curNode = firstNode;
	for (int i = 0; i != length; ++i)
	{
		if (curNode->elem == element)
			return i;
		curNode = curNode->next;
	}
	if (curNode == NULL)
		return -1;
}

void myList_Dul::insert(int index, const Elemtype& element)
{
	if (index < 0 || index > length)
	{
		throw out_of_range("This index is out of range");
	}
	chainNode* curNode = firstNode;
	if (index == 0)
	{
		firstNode = new chainNode(element, NULL, firstNode);
		if (length != 0)
		{
			firstNode->next->prior = firstNode;
		}
	}
	else
	{
		for (int i = 0; i != index - 1; ++i)
		{
			curNode = curNode->next;
		}
		curNode->next = new chainNode(element, curNode, curNode->next);
		if (index != length)
		{
			curNode->next->next->prior = curNode->next;
		}
	}
	++length;
}

void myList_Dul::push_back(const Elemtype& element)
{
	insert(length, element);
}

void myList_Dul::erase(int index)
{
	chainNode* curNode = firstNode;
	chainNode* deleteNode;
	if (index == 0)
	{
		deleteNode = firstNode;
		firstNode = firstNode->next;
		firstNode->prior = NULL;
	}
	else
	{
		for (int i = 0; i != index - 1; ++i)
		{
			curNode = curNode->next;
		}
		deleteNode = curNode->next;
		curNode->next = deleteNode->next;
		deleteNode->next->prior = curNode;
	}
	--length;
	delete deleteNode;
}

void myList_Dul::output() const
{
	chainNode* curNode = firstNode;
	while (curNode != NULL)
	{
		cout << curNode->elem << endl;
		curNode = curNode->next;
	}
}

