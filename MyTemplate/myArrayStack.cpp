#include <iostream>
#include <stdexcept>

using namespace std;
using Elemtype = int;

class stack
{
public:
	stack(int initsize = 10);
	stack(const stack&);
	~stack() { delete[] elem; }

	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& top() { return elem[length - 1]; }
	void pop();
	void push(const Elemtype&);

private:
	Elemtype* elem;
	int length;
	int listsize;
};

void addListSize(Elemtype* elem, int oldsize, int newsize)
{
	if (newsize < 0)
	{
		throw invalid_argument("The new size must be > 0!");
	}
	Elemtype* temp = new Elemtype[newsize];
	for (int i = 0; i != oldsize; ++i)
	{
		temp[i] = elem[i];
	}
	delete[] elem;
	elem = temp;
}

stack::stack(int initsize)
{
	if (initsize < 1)
	{
		throw range_error("The size of the list must be a positive number!");
	}
	if (!(elem = new Elemtype[initsize]))
	{
		throw runtime_error("Failed to allocate memory :(");
	}
	length = 0;
	listsize = initsize;
}

stack::stack(const stack& vector)
{
	length = vector.length;
	listsize = vector.listsize;
	elem = new Elemtype[listsize];
	for (int i = 0; i != length; ++i)
	{
		elem[i] = vector.elem[i];
	}
}

void stack::pop()
{
	if (length == 0)
		throw range_error("This stack is already empty!");
	elem[length - 1].~Elemtype();
	--length;
}

void stack::push(const Elemtype& element)
{
	if (length >= listsize)
		addListSize(elem, listsize, listsize * 2);
	elem[length++] = element;
}
