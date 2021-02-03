#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <queue>
#include <vector>

using namespace std;

//Huffman压缩树结点
struct Huffman
{
	unsigned int weight;
	unsigned int parent, lchild, rchild;
	int position;
};

//解压树结点
struct DecodeNode
{
	bool isLeaf;
	unsigned char word;
	DecodeNode* lchild;
	DecodeNode* rchild;
};

//计算编码中各字符的权值
void GetWeight(unsigned int weight[], int& number,string infile)
{
	ifstream ifs(infile,ifstream::binary);
	istreambuf_iterator<char> isb(ifs), end;
	unsigned char word;
	if (ifs)
	{
		while (isb != end)
		{
			word = *isb++;
			if (weight[word] == 0)
				++number;
			++weight[word];
		}
	}
	else
		cerr << "Couldn't open infile!";
}

//在Huffman树的1~range中寻找权值最小的两个结点s1，s2
//void Select(Huffman* HTree, int range, int& s1, int& s2)
//{
//	int i = 1;
//	s1 = 0;
//	s2 = 0;
//	for (i=1; i <= range; ++i)
//	{
//		if ((HTree[i].parent == 0) && (HTree[i].weight <= HTree[s1].weight))
//			s1 = i;
//	}
//	for (i=1; i <= range; ++i)
//	{
//		if ((HTree[i].parent == 0) && (HTree[i].weight <= HTree[s2].weight) && (i != s1))
//			s2 = i;
//	}
//}

//进行Huffman编码
void HuffmanCoding(string infile,unsigned int weight[256],string code[], bool save, string name)
{
	int i = 0, j = 0;
	int cdlen = 0;
	int number = 0;
	char cd[200] = { '\0' };
	priority_queue<int,vector<int>,greater<int> > queue;
	Huffman* ptr;
	GetWeight(weight, number, infile);
	if (save)
	{
		for (i = 0; i < name.size(); i++)
		{
			if (weight[name[i]] == 0)
				number++;
			++weight[name[i]];
		}
	}
	Huffman* HTree = new Huffman[2 * number];
	//构建Huffman树
	for (ptr = HTree + 1, i = 1; i <= number; ++ptr, ++i)
	{
		while (weight[j] == 0)
			++j;
		*ptr = { weight[j],0,0,0,j };
		queue.push(weight[j]);
		++j;
	}
	for (; i <= 2 * number - 1; ++ptr, ++i)
		*ptr = { 0,0,0,0,-1 };
	//for (i = number + 1; i <= 2 * number - 1; ++i)
	//{
	//	int s1, s2;
	//	Select(HTree, i - 1, s1, s2);
	//	HTree[s1].parent = i;
	//	HTree[s2].parent = i;
	//	HTree[i].lchild = s1;
	//	HTree[i].rchild = s2;
	//	HTree[i].weight = HTree[s1].weight + HTree[s2].weight;
	//}
	for (i = number + 1; i <= 2 * number - 1; ++i)
	{
		int s1, s2;
		int weight1, weight2;
		weight1 = queue.top();
		queue.pop();
		weight2 = queue.top();
		queue.pop();
		for (j = 1; j < i; j++)
		{
			if (HTree[j].parent == 0 && HTree[j].weight == weight1)
			{
				s1 = j;
			}
		}
		for (j = 1; j < i; j++)
		{
			if (HTree[j].parent == 0 && HTree[j].weight == weight2 && j != s1)
			{
				s2 = j;
			}
		}
		HTree[s1].parent = i;
		HTree[s2].parent = i;
		HTree[i].lchild = s1;
		HTree[i].rchild = s2;
		HTree[i].weight = HTree[s1].weight + HTree[s2].weight;
		queue.push(HTree[i].weight);
	}
	j = 2 * number - 1;
	for (i = 1; i <= j; ++i)
		HTree[i].weight = 0;
	//根据得到的Huffman树，进行Huffman编码
	while (j)
	{
		if (HTree[j].weight == 0)
		{
			HTree[j].weight = 1;
			if (HTree[j].lchild != 0)
			{
				j = HTree[j].lchild;
				cd[cdlen++] = '0';
			}
			else if (HTree[j].rchild == 0)
			{
				cd[cdlen] = '\0';
				code[HTree[j].position] = cd;
			}
		}
		else if (HTree[j].weight == 1)
		{
			HTree[j].weight = 2;
			if (HTree[j].rchild != 0)
			{
				j = HTree[j].rchild;
				cd[cdlen++] = '1';
			}
		}
		else
		{
			HTree[j].weight = 0;
			j = HTree[j].parent;
			--cdlen;
		}
	}
}

//得到Huffman编码，进行Huffman压缩
bool HuffmanCompress(string infile, string outfile,bool save,string name)
{
	int i = 0, j = 0;
	int counter = 7;
	unsigned char writecode = 0;
	unsigned char word;
	unsigned int weight[300] = { 0 };
	ifstream ifs(infile,ifstream::binary);
	ofstream ofs(outfile,ofstream::binary);
	istreambuf_iterator<char> isb(ifs), end;
	string code[300];
	string huffcode;
	HuffmanCoding(infile, weight, code, save, name);
	for (i = 0; i <= 255; i++)
	{
		if (weight[i] != 0)
			ofs << i << " " << code[i] << " ";
	}
	ofs << endl;
	if (save)
	{
		for (i = 0; i < name.size(); ++i)
		{
			word = name[i];
			ofs << code[word];
		}
		ofs << '#';
	}
	if (ifs)
	{
		while (isb != end)
		{
			word = *isb++;
			huffcode = code[word];
			for (i = 0; i < huffcode.size(); i++)
			{
				counter--;
				if (counter < 0)
				{
					counter = 7;
					writecode = 0;
				}
			}
		}
	}
	else
		cerr << "can't open file!";
	ifs.close();
	ifs.open(infile, ifstream::binary);
	istreambuf_iterator<char> isb2(ifs);
	word = counter + 48;
	ofs << word;
	counter = 7;
	writecode = 0;
	while (isb2 != end)
	{
		word = *isb2++;
		huffcode = code[word];
		for (i = 0; i < huffcode.size(); i++)
		{
			if (huffcode[i] == '1')
			{
				writecode += pow(2, counter);
			}
			counter--;
			if (counter < 0)
			{
				counter = 7;
				ofs << writecode;
				writecode = 0;
			}
		}
	}
	if(counter!=7)
		ofs << writecode;
	return true;
}

//进行Huffman解压
bool HuffmanDecode(string infile, string outfile,string name)
{
	int i = 0, j = 0, k = 0;
	int tag = 0;
	int flag = 2;
	int count = 0;
	int number = 0;
	int position = 0;
	int addnumber = 0;
	int counter = 0;
	int int_code[8] = { 0 };
	char ch_addnumber;
	unsigned char ch;
	string word;
	string code[300];
	char path[100] = { '\0' };
	ifstream ifs(infile);
	ofstream ofs;
	DecodeNode* root;
	DecodeNode* ptr;
	DecodeNode* temp;
	istreambuf_iterator<char> isb(ifs), end;
	while (ifs >> position >> word && number<=255)
	{
		if (word[0] != '0' && word[0] != '1')
			break;
		++number;
		code[position] = word;
	}
	//构建解压树
	root = new DecodeNode{false,0,NULL,NULL };
	for (i = 1; i <= number; i++)
	{
		ptr = root;
		while (code[j] == "")
			j++;
		for (k = 0; k < code[j].size(); k++)
		{
			if (code[j][k] == '0')
			{
				if(ptr->lchild==NULL)
				{
					temp = new DecodeNode{false,0,NULL,NULL };
					ptr->lchild = temp;
					if (k == code[j].size() - 1)
					{
						temp->isLeaf = true;
						temp->word = j;
					}
				}
				ptr = ptr->lchild;
			}
			else if (code[j][k] == '1')
			{
				if(ptr->rchild==NULL)
				{
					temp = new DecodeNode{ false,0,NULL,NULL };
					ptr->rchild = temp;
					if (k == code[j].size() - 1)
					{
						temp->isLeaf = true;
						temp->word = j;
					}
				}
				ptr = ptr->rchild;
			}
		}
		j++;
	}
	//解码文件正文
	ptr = root;
	ifs.close();
	ifs.open(infile,ifstream::binary);
	for (int i = 0; i != number; i++)
		ifs >> position >> word;
	while (ifs>>ch && count < 100)
	{
		if (ch == '0')
		{
			ptr = ptr->lchild;
			if (ptr->isLeaf == true)
			{
				path[count++] = ptr->word;
				ptr = root;
			}
			if(flag!=0)
				flag = 1;
		}
		else if (ch == '1')
		{
			ptr = ptr->rchild;
			if (ptr->isLeaf == true)
			{
				path[count++] = ptr->word;
				ptr = root;
			}
			if(flag!=0)
				flag = 1;
		}
		else if (ch == '#' && flag == 1)
		{
			tag = 1;
			name = path;
			break;
		}
		else
			flag = 0;
	}  
	outfile = outfile + name;
	ofs.open(outfile, ofstream::binary);
	ptr = root;
	ifs.close();
	ifs.open(infile, ifstream::binary);
	for (int i = 0; i != number; i++)
		ifs >> position >> word;
	if (tag == 1)
	{
		while (ifs >> ch)
		{
			if (ch == '#')
				break;
		}
	}
	ifs >> ch_addnumber;
	addnumber = ch_addnumber - 48;
	i = 0;
	j = 0;
	while (isb!=end)
	{
		ch = *isb++;
		if (isb == end)
		{
			j = addnumber + 1;
		}
		for (k = 7; k >= 0; k--)
		{
			if (ch != 0)
			{
				int_code[k] = ch % 2;
				ch = ch / 2;
			}
			else if (ch == 0)
			{
				int_code[k] = 0;
			}
		}
		for (k = 0; k < 8-j; k++)
		{
			if (int_code[k] == 0)
			{
				ptr = ptr->lchild;
				if (ptr->isLeaf == true)
				{
					ofs << ptr->word;
					ptr = root;
				}
			}
			else if (int_code[k] == 1)
			{
				ptr = ptr->rchild;
				if (ptr->isLeaf == true)
				{
					ofs << ptr->word;
					ptr = root;
				}
			}
		}
	}
	return true;
}

int main(int argc,char* argv[])
{
	int i = 0, j = 0;
	string infile, outfile, name,name2;
	char path[20] = { '\0' };
	if (argc == 2)
	{
		name = argv[1];
		infile = "D:\\huffman\\" + name;
		outfile = "D:\\huffman\\" + name + ".huff";
		HuffmanCompress(infile, outfile, false, name);
	}
	else if (argc == 3)
	{
		name = argv[1];
		if (name == "-z")
		{
			name = argv[2];
			infile = "D:\\huffman\\" + name;


			outfile = "D:\\huffman\\" + name + ".huff";
			HuffmanCompress(infile, outfile, false, name);
		}
		else if (name == "-u")
		{
			name = argv[2];
			for (i = name.size() - 5; i < name.size(); i++)
			{
				path[j++] = name[i];
			}
			name2 = path;
			if (name2 != ".huff")
			{
				cout << "This is not a .huff file!" << endl;
			}
			else
			{
				infile = "D:\\huffman\\" + name;
				for (int i = 0; i < 20; i++)
					path[i] = '\0';
				for (int i = 0; i < name.size()-5; i++)
					path[i] = name[i];
				name = path;
				outfile = "D:\\huffman\\";
				HuffmanDecode(infile, outfile, name);
			}
		}
	}
	else if (argc == 4)
	{
		name = argv[1];
		if (name == "-zr")
		{
			name = argv[2];
			name2 = argv[3];
			infile = "D:\\huffman\\" + name;
			outfile= "D:\\huffman\\" + name2;
			HuffmanCompress(infile, outfile, true, name);
		}
		else if (name == "-ur")
		{
			name = argv[2];
			for (i = name.size() - 5; i < name.size(); i++)
			{
				path[j++] = name[i];
			}
			name2 = path;
			if (name2 != ".huff")
			{
				cout << "This is not a .huff file!" << endl;
			}
			else
			{
				name2 = argv[3];
				infile = "D:\\huffman\\" + name;
				outfile = "D:\\huffman\\";
				HuffmanDecode(infile, outfile, name2);
			}
		}
	}
}