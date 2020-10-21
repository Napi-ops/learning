#include <iostream>
#include <stdexcept>

using namespace std;
using Elemtype = int;

class queue
{
public:
	queue(int initsize);
	queue(const queue&);
	~queue() { delete[] elem; }

	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& front() const { return elem[(theFront + 1) % listsize]; }
	Elemtype& back() const { return elem[theBack]; }
	void pop();
	void push(const Elemtype&);

private:
	void addListSize(int, int);
	Elemtype* elem;
	int theFront;
	int theBack;
	int length;
	int listsize;
};

void queue::addListSize(int oldsize, int newsize)
{
	if (newsize < 0)
	{
		throw invalid_argument("The new size must be > 0!");
	}
	Elemtype* temp = new Elemtype[newsize];
	if (theFront <= theBack)
	{
		for (int i = theFront + 1; i != theBack + 1; ++i)
			temp[i] = elem[i];
	}
	else
	{
		for (int i = theFront + 1; i != listsize; ++i)
			temp[newsize - oldsize + i] = elem[i];
		for (int i = 0; i != theBack + 1; ++i)
			temp[i] = elem[i];
		theFront += newsize - oldsize;
	}
	delete[] elem;
	elem = temp;
	listsize *= 2;
}

queue::queue(int initsize)
{
	if (initsize < 1)
	{
		throw range_error("The size of the list must be a positive number!");
	}
	if (!(elem = new Elemtype[initsize]))
	{
		throw runtime_error("Failed to allocate memory :(");
	}
	theFront = theBack = 0;
	length = 0;
	listsize = initsize;
}

queue::queue(const queue& vector)
{
	theFront = vector.theFront;
	theBack = vector.theBack;
	length = vector.length;
	listsize = vector.listsize;
	elem = new Elemtype[listsize];
	for (int i = 0; i != length; ++i)
	{
		elem[i] = vector.elem[i];
	}
}

void queue::pop()
{
	if (length == 0)
		throw range_error("This queue is already empty!");
	theFront = (theFront + 1) % listsize;
	elem[theFront].~Elemtype();
	--length;
}

void queue::push(const Elemtype& element)
{
	if (length + 1 >= listsize)
		addListSize(listsize, 2 * listsize);
	theBack = (theBack + 1) % listsize;
	elem[theBack] = element;
	++length;
}
