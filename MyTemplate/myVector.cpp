#include <iostream>
#include <stdexcept>

using namespace std;
using Elemtype = int;

class myVector
{
public:
	myVector(int initsize);
	~myVector() { delete[] elem; }

	void clear();
	bool empty() const { return length == 0; }
	int size() const { return length; }
	Elemtype& get(int index) const;
	int indexOf(const Elemtype& element) const;
	void insert(int index, const Elemtype& element);
	void push_back(const Elemtype& element);
	void erase(const Elemtype& element);
	void erase(int index);
	void output() const;

private:
	void checkIndex(int index) const;
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

void myVector::checkIndex(int index) const
{
	if (index<0 || index>=length)
	{
		throw out_of_range("This index is out of range");
	}
}

myVector::myVector(int initsize)
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

void myVector::clear()
{
	for (int i = 0; i != length; ++i)
	{
		elem[i] = 0;
	}
}

Elemtype& myVector::get(int index) const
{
	checkIndex(index);
	return elem[index];
}

int myVector::indexOf(const Elemtype& element) const
{
	for (int i = 0; i != length; ++i)
	{
		if (elem[i] == element)
		{
			return i;
		}
	}
	return -1;
}

void myVector::insert(int index, const Elemtype& element)
{
	if (index < 0 || index > length)
	{
		throw out_of_range("This index is out of range");
	}
	if (length >= listsize)
	{
		addListSize(elem, listsize, listsize * 2);
		listsize *= 2;
	}
	for (int i = length; i != index; --i)
	{
		elem[i] = elem[i - 1];
	}
	elem[index] = element;
	++length;
}

void myVector::erase(int index)
{
	checkIndex(index);
	for (int i = index; i != length-1; ++i)
	{
		elem[i] = elem[i + 1];
	}
	--length;
}

void myVector::erase(const Elemtype& element)
{
	int index = indexOf(element);
	checkIndex(index);
	for (int i = index; i != length - 1; ++i)
	{
		elem[i] = elem[i + 1];
	}
	--length;
}

void myVector::push_back(const Elemtype& element)
{
	insert(length, element);
}

void myVector::output() const
{
	for (int i = 0; i != length; ++i)
	{
		cout << elem[i] << endl;
	}
}



