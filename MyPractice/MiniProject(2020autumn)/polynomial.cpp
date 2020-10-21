#include <iostream>

using namespace std;

//多项式的结点
struct chainNode
{
	float coef;
	int index;
	chainNode* next;

	chainNode() {};
	chainNode(const float& element, int index) : coef(element), index(index) {}
	chainNode(const float& element, int index, chainNode* next) : coef(element), index(index), next(next) {}
};

//多项式类
class polynomial
{
public:
	polynomial();
	polynomial(const polynomial&);
	~polynomial();

	bool empty() const { return headNode->coef == 0; }	//检查多项式是否为空
	int length() const { return headNode->coef; }	//得到多项式的项数
	void clear();	//将多项式清空
	void copy(polynomial);	//将多项式清空，并将参数polynomial中的多项式复制到该多项式中
	void addPoly(polynomial);	//多项式加法
	void subPoly(polynomial);	//多项式减法
	void mulPoly(polynomial);	//多项式乘法
	void derivative();	//多项式求导
	void output() const;	//输出多项式
	void insert(float, int);	//向多项式中插入一项

private:
	void mulSingle(float, int);	//将多项式中的每一项乘以一个指定项
	chainNode* headNode;
};

//储存多项式的链表
struct polyNode
{
	polynomial poly;
	polyNode* next;

	polyNode() {};
	polyNode(const polynomial& poly, polyNode* next) : poly(poly), next(next) {}
};

//向多项式中插入一项
void polynomial::insert(float elem, int index)
{
	chainNode* ptr = headNode;
	while (ptr->next != NULL && ptr->next->index > index)
		ptr = ptr->next;
	if (ptr->next == NULL)
	{
		chainNode* newNode = new chainNode(elem, index, NULL);
		ptr->next = newNode;
		++headNode->coef;
	}
	else if (ptr->next->index == index)
	{
		ptr->next->coef += elem;
	}
	else if (ptr->next->index < index)
	{
		chainNode* priorNode = headNode;
		while (priorNode->next != ptr->next)
			priorNode =priorNode->next;
		chainNode* newNode = new chainNode(elem, index, ptr->next);
		priorNode->next = newNode;
		++headNode->coef;
	}
}

//将多项式中的每一项乘以一个指定项
void polynomial::mulSingle(float elem, int index)
{
	chainNode* ptr = headNode->next;
	while (ptr != NULL)
	{
		if (elem != 0)
		{
			ptr->coef *= elem;
			ptr->index += index;
		}
		ptr = ptr->next;
	}
}

polynomial::polynomial()
{
	headNode = new chainNode(0, -1, NULL);
}

polynomial::polynomial(const polynomial& polynomial)
{
	headNode = new chainNode(polynomial.headNode->coef, polynomial.headNode->index);
	chainNode* curNode = polynomial.headNode;
	chainNode* tarNode = headNode;
	curNode = curNode->next;
	while (curNode != NULL)
	{
		tarNode->next = new chainNode(curNode->coef, curNode->index);
		tarNode = tarNode->next;
		curNode = curNode->next;
	}
	tarNode->next = NULL;
}

polynomial::~polynomial()
{
	while (headNode != NULL)
	{
		chainNode* nextNode = headNode->next;
		delete headNode;
		headNode = nextNode;
	}
}

//将多项式清空
void polynomial::clear()
{
	chainNode* ptr = headNode->next;
	while (ptr != NULL)
	{
		chainNode* deleteNode = ptr;
		ptr = ptr->next;
		delete deleteNode;
	}
	headNode->coef = 0;
	headNode->index = -1;
	headNode->next = NULL;
}

//将多项式清空，并将参数polynomial中的多项式复制到该多项式中
void polynomial::copy(polynomial p)
{
	clear();
	headNode = new chainNode(p.headNode->coef, p.headNode->index);
	chainNode* curNode = p.headNode;
	chainNode* tarNode = headNode;
	curNode = curNode->next;
	while (curNode != NULL)
	{
		tarNode->next = new chainNode(curNode->coef, curNode->index);
		tarNode = tarNode->next;
		curNode = curNode->next;
	}
	tarNode->next = NULL;
}

//多项式加法
void polynomial::addPoly(polynomial p)
{
	chainNode* ptr = p.headNode->next;
	while (ptr != NULL)
	{
		insert(ptr->coef, ptr->index);
		ptr = ptr->next;
	}
}

//多项式减法
void polynomial::subPoly(polynomial p)
{
	chainNode* ptr = p.headNode->next;
	while (ptr != NULL)
	{
		insert(-ptr->coef, ptr->index);
		ptr = ptr->next;
	}
}

//多项式乘法
void polynomial::mulPoly(polynomial p)
{
	chainNode* ptr = headNode->next;
	polynomial copy;
	while (ptr != NULL)
	{
		polynomial temp(p);
		temp.mulSingle(ptr->coef, ptr->index);
		copy.addPoly(temp);
		ptr = ptr->next;
	}
	clear();
	addPoly(copy);
}

//多项式求导
void polynomial::derivative()
{
	chainNode* ptr = headNode->next;
	while (ptr != NULL)
	{
		if (ptr->coef == 0)
		{
			ptr = ptr->next;
			continue;
		}
		if (ptr->index != 0)
		{
			ptr->coef *= ptr->index;
			--ptr->index;
			ptr = ptr->next;
		}
		else if (ptr->index == 0)
		{
			chainNode* priorNode = headNode;
			while (priorNode->next != ptr)
				priorNode = priorNode->next;
			priorNode->next = NULL;
			delete ptr;
			break;
		}
	}
}

//输出多项式
void polynomial::output() const
{
	int tag = 1;
	chainNode* ptr = headNode->next;
	while (ptr != NULL)
	{
		if (ptr->coef == 0)
		{
			ptr = ptr->next;
			continue;
		}
		else
		{
			if (ptr->coef > 0)
			{
				if (ptr != headNode->next)
					cout << '+';
				if (ptr->coef == 1 && ptr->index == 0)
					cout << '1';
				if (ptr->coef != 1)
					cout << ptr->coef;
				if (ptr->index != 0)
				{
					if (ptr->index != 1)
						cout << 'x' << '^' << ptr->index;
					else
						cout << 'x';
				}
			}
			else if (ptr->coef < 0)
			{
				if (ptr->coef == -1 && ptr->index == 0)
					cout << '-' << '1';
				if (ptr->coef != -1)
					cout << ptr->coef;
				else
					cout << '-';
				if (ptr->index != 0)
				{
					if (ptr->index != 1)
						cout << 'x' << '^' << ptr->index;
					else
						cout << 'x';
				}
			}
			tag = 0;
			ptr = ptr->next;
		}
	}
	if (tag == 1)
		cout << '0';
	cout << endl;
}

//检查给定的序号在储存多项式的数组中是否越界
bool checkIndex(int index,polyNode* firstNode)
{
	int length = 0;
	polyNode* ptr = firstNode;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		length++;
	}
	if (index <= length - 1)
		return 1;
	else
		return 0;
}

int main()
{
	polyNode* firstNode = NULL;
	while (1)
	{
		int operation = 0;	//用于得到操作指令
		//主菜单显示界面
		cout << "**********OPERATIONS**********" << endl;
		cout << "*****1.	menu		 *****" << endl;
		cout << "*****2.	create		 *****" << endl;
		cout << "*****3.	display		 *****" << endl;
		cout << "*****4.	display all	 *****" << endl;
		cout << "*****5.	add		 *****" << endl;
		cout << "*****6.	sub		 *****" << endl;
		cout << "*****7.	delete		 *****" << endl;
		cout << "*****8.	mul		 *****" << endl;
		cout << "*****9.	derivative	 *****" << endl;
		cout << "*****10.quit		 *****" << endl;
		cout << "**********OPERATIONS**********" << endl;
		cout << '#' << "Operation:";
		scanf_s("%d", &operation);
		getchar();
		if (operation == 1)
		{
			continue;
		}
		//创建多项式
		else if (operation == 2)
		{
			cout << "Enter a polynomial:";
			polynomial temp;
			int num = 0;
			float coef = 0;
			int index = 0;
			scanf_s("%d", &num);
			getchar();
			for (int i = 0; i != num; ++i)
			{
				scanf_s("%f%d", &coef, &index);

				temp.insert(coef, index);
			}
			getchar();
			polyNode* ptr = firstNode;
			if (firstNode == NULL)
			{
				firstNode = new polyNode(temp, NULL);
			}
			else
			{
				while (ptr->next != NULL)
					ptr = ptr->next;
				ptr->next = new polyNode(temp, NULL);
			}
		}
		else if (operation == 3 || operation == 7 || operation == 9)
		{
			cout << "Enter the index of the polynomial:";
			polyNode* ptr = firstNode;
			int index = 0;
			scanf_s("%d", &index);
			getchar();
			--index;
			//检查越界
			if (!checkIndex(index, firstNode))
			{
				cout << "The index is out of range!" << endl;
				continue;
			}
			for (int i = 0; i != index; ++i)
				ptr = ptr->next;
			//输出指定序号位置的多项式
			if (operation == 3)
			{
				cout << "poly" << " " << index + 1 << ":";
				ptr->poly.output();
			}
			//删除指定序号位置的多项式
			else if (operation == 7)
			{
				if (ptr == firstNode)
				{
					firstNode = ptr->next;
					delete ptr;
				}
				else
				{
					polyNode* priorNode = firstNode;
					while (priorNode->next != ptr)
						priorNode = priorNode->next;
					priorNode->next = ptr->next;
					delete ptr;
				}
			}
			//多项式求导
			else if (operation == 9)
			{
				polynomial temp(ptr->poly);
				temp.derivative();
				while (ptr->next != NULL)
					ptr = ptr->next;
				ptr->next = new polyNode(temp, NULL);
				cout << "Result:";
				temp.output();
			}
		}
		//输出储存的所有多项式
		else if (operation == 4)
		{
			if (firstNode == NULL)
			{
				cout << "There is no polynomial!" << endl;
				continue;
			}
			int i = 1;
			polyNode* ptr = firstNode;
			while (ptr != NULL)
			{
				cout << "poly" << " " << i++ << ":";
				ptr->poly.output();
				ptr = ptr->next;
			}
		}
		else if (operation == 5 || operation == 6 || operation == 8)
		{
			if (firstNode==NULL)
			{
				cout << "You haven't input any polynomials!" << endl;
				continue;
			}
			cout << "Enter the index of the two polynomials:";
			int p1 = 0, p2 = 0;
			scanf_s("%d%d", &p1, &p2);
			--p1;
			--p2;
			//检查越界
			if (!checkIndex(p1, firstNode))
			{
				cout << "The index of polynomial1 is out of range!"<<endl;
				continue;
			}
			if (!checkIndex(p2, firstNode))
			{
				cout << "The index of polynomial2 is out of range!" << endl;
				continue;
			}
			polyNode* ptr = firstNode, * ptr1 = firstNode, * ptr2 = firstNode;
			while (ptr->next != NULL)
				ptr = ptr->next;
			for (int i = 0; i != p1; ++i)
				ptr1 = ptr1->next;
			for (int i = 0; i != p2; ++i)
				ptr2 = ptr2->next;
			polynomial temp(ptr1->poly);
			//多项式加法
			if (operation == 5)
				temp.addPoly(ptr2->poly);
			//多项式减法
			else if (operation == 6)
				temp.subPoly(ptr2->poly);
			//多项式乘法
			else if (operation == 8)
				temp.mulPoly(ptr2->poly);
			ptr->next = new polyNode(temp, NULL);
			cout << "Result:";
			temp.output();
		}
		//退出程序
		else if (operation == 10)
			break;
		//输入其他指令则报错
		else
			cout << "Error operation code!" << endl;
	}
}