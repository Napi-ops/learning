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

class list
{
public:
	list(int initsize = 10);
	list(const list&);
	~list();

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

void list::checkIndex(int index) const
{
	if (index < 0 || index >= length)
	{
		throw out_of_range("This index is out of range");
	}
}

list::list(int initsize)
{
	firstNode = NULL;
	length = 0;
}

list::list(const list& list)
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

list::~list()
{
	while (firstNode != NULL)
	{
		chainNode* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;
	}
}

void list::clear()
{
	chainNode* curNode = firstNode;
	while (curNode != NULL)
	{
		curNode->elem.~Elemtype();
		curNode = curNode->next;
	}
}

Elemtype& list::get(int index) const
{
	checkIndex(index);
	chainNode* curNode = firstNode;
	for (int i = 0; i != index; ++i)
	{
		curNode = curNode->next;
	}
	return curNode->elem;
}

int list::indexOf(const Elemtype& element) const
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

void list::insert(int index, const Elemtype& element)
{
	if (index < 0 || index > length)
	{
		throw out_of_range("This index is out of range");
	}
	chainNode* curNode = firstNode;
	if (index == 0)
	{
		firstNode = new chainNode(element, firstNode);
	}
	else
	{
		for (int i = 0; i != index - 1; ++i)
		{
			curNode = curNode->next;
		}
		curNode->next = new chainNode(element, curNode->next);
	}
	++length;
}

void list::push_back(const Elemtype& element)
{
	insert(length, element);
}

void list::erase(int index)
{
	chainNode* curNode = firstNode;
	chainNode* deleteNode;
	if (index == 0)
	{
		deleteNode = firstNode;
		firstNode = firstNode->next;
	}
	else
	{
		for (int i = 0; i != index - 1; ++i)
		{
			curNode = curNode->next;
		}
		deleteNode = curNode->next;
		curNode->next = deleteNode->next;
	}
	--length;
	delete deleteNode;
}

void list::output() const
{
	chainNode* curNode = firstNode;
	while (curNode != NULL)
	{
		cout << curNode->elem << endl;
		curNode = curNode->next;
	}
}

