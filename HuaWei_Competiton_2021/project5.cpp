#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <math.h>

using namespace std;

#define MAX_SERVER_KIND 100
#define MAX_VM_KIND 1000
#define MAX_ASK 100000
#define MAX_DAYS 1000

struct Server_Kind
{
	string name;
	short core, memory, rent;
	int price;
	double vpratio;// The ratio of value and performance
	double cmratio;// The ratio of core and memory
	int subscript;
};

struct VM_Kind
{
	string name;
	short core, memory;
	bool doub;// 0--single 1--double
};

struct Server
{
	short server_kind;
	short core_A, core_B, memory_A, memory_B;
	bool usage = 0;// 0--unuse 1--use
	int subscript;
};

struct VM
{
	short vm_kind;
	int location;
	bool ABtag;// 0--A  1--B
	bool usage = 0;// 0--unuse 1--use
	int vm_id;
	int subscript;
};

struct Ratio
{
	double value;
	int position;
};

Server_Kind server_kind[MAX_SERVER_KIND];
VM_Kind vm_kind[MAX_VM_KIND];
int server_kind_num, vm_kind_num, days, ask_nums;
int yesterday = -1, today = -1;	//for the vm
int low = 0, high = 0;	//for the server

Server server[MAX_ASK];// save the status of server[i], i == id
VM vm[MAX_ASK];// save the status of vm[i]

//for the sort function
bool cmp_server(Server_Kind a, Server_Kind b)
{
	return a.vpratio < b.vpratio;
}

//for the sort function
bool cmp_ratio(Ratio a, Ratio b)
{
	return a.value < b.value;
}

bool cmp_double(VM a, VM b)
{
	return vm_kind[a.vm_kind].doub > vm_kind[b.vm_kind].doub;
}

bool cmp_cm(VM a, VM b)
{
	return (vm_kind[a.vm_kind].core + vm_kind[a.vm_kind].memory) > (vm_kind[b.vm_kind].core + vm_kind[b.vm_kind].memory);
}

bool cmp_sub(VM a, VM b)
{
	return a.subscript < b.subscript;
}

bool cmp_kind(Server a, Server b)
{
	return a.server_kind < b.server_kind;
}

void sortVM()
{
	int i = 0;
	sort(vm + yesterday + 1, vm + today + 1, cmp_double);
	for (i = yesterday + 1; i <= today; i++)
	{
		if ((vm_kind[vm[i].vm_kind].doub == 0) && (vm_kind[vm[i - 1].vm_kind].doub == 1))
			break;
	}
	sort(vm + yesterday + 1, vm + i, cmp_cm);
	sort(vm + i, vm + today + 1, cmp_cm);
}

int findVM(int number)
{
	int i = 0;
	for (i = 0; i <= today; i++)
	{
		if (vm[i].vm_id == number)
			break;
	}
	return i;
}

bool greedy(int server_id, short& CA, short& MA, short& CB, short& MB, int& Core, int& Memory)
{
	int tag = 0;
	for (int i = yesterday + 1; i <= today; i++)
		if (vm[i].usage == 0 && vm_kind[vm[i].vm_kind].doub == 1)//unlocated double
		{
			VM_Kind it = vm_kind[vm[i].vm_kind];
			if (it.core / 2 <= CA && it.memory / 2 <= MA && it.core / 2 <= CB && it.memory / 2 <= MB)
			{
				tag = 1;
				vm[i].usage = 1;
				vm[i].location = server_id;
				server[server_id].usage = 1;
				CA -= it.core / 2;
				MA -= it.memory / 2;
				CB -= it.core / 2;
				MB -= it.memory / 2;
				Core -= it.core;
				Memory -= it.memory;
			}
		}

	for (int i = yesterday + 1; i <= today; i++)
		if (vm[i].usage == 0 && vm_kind[vm[i].vm_kind].doub == 0)//unlocated single
		{
			VM_Kind it = vm_kind[vm[i].vm_kind];
			if (it.core <= CA && it.memory <= MA && it.core <= CB && it.memory <= MB)//can be located in A or B
			{
				tag = 1;
				vm[i].usage = 1;
				vm[i].location = server_id;
				server[server_id].usage = 1;
				if (CA + MA >= CB + MB)//the rest of A is more
				{
					CA -= it.core;
					MA -= it.memory;
					Core -= it.core;
					Memory -= it.memory;
					vm[i].ABtag = 0;
				}
				else//the rest of B is more
				{
					CB -= it.core;
					MB -= it.memory;
					Core -= it.core;
					Memory -= it.memory;
					vm[i].ABtag = 1;
				}
			}
			else
				if (it.core <= CA && it.memory <= MA)//only can be located in A
				{
					tag = 1;
					vm[i].usage = 1;
					vm[i].location = server_id;
					vm[i].ABtag = 0;
					server[server_id].usage = 1;
					CA -= it.core;
					MA -= it.memory;
					Core -= it.core;
					Memory -= it.memory;
				}
				else
					if (it.core <= CB && it.memory <= MB)//only can be located in B
					{
						tag = 1;
						vm[i].usage = 1;
						vm[i].location = server_id;
						vm[i].ABtag = 1;
						server[server_id].usage = 1;
						CB -= it.core;
						MB -= it.memory;
						Core -= it.core;
						Memory -= it.memory;
					}
		}
	return tag;
}


bool handsome(int l, int h, int& Core, int& Memory)
{
	for (int i = l; i < h; i++)
	{
		greedy(i, server[i].core_A, server[i].memory_A, server[i].core_B, server[i].memory_B, Core, Memory);

	}
	for (int j = yesterday + 1; j <= today; j++)
	{
		if (vm[j].usage == 0)
			return false;
	}
	return true;
}

void GetServer(int Core, int Memory,int day)
{
	Server_Kind server_kind_beta[MAX_SERVER_KIND];
	for (int i = 0; i < server_kind_num; i++)
	{
		server_kind_beta[i] = server_kind[i];
	}
	for (int i = 0; i < server_kind_num; i++)
	{
		server_kind_beta[i].vpratio = server_kind[i].vpratio + ((days - day) * server_kind[i].rent) / ((double)server_kind[i].core + (double)server_kind[i].memory);
	}
	sort(server_kind_beta, server_kind_beta + server_kind_num, cmp_server);
	while (Core > 0 || Memory > 0)
	{
		int tag = 0;
		int VMcore = 0, VMmemory = 0;
		for (int i = yesterday + 1; i <= today; i++)
		{
			if (vm[i].usage == 0)
			{
				VMcore += vm_kind[vm[i].vm_kind].core;
				VMmemory += vm_kind[vm[i].vm_kind].memory;
				++tag;
			}
			if (tag == 15)
				break;
		}
		double cmratio = (double)VMcore / (double)VMmemory;
		Ratio ratio[MAX_SERVER_KIND] = { 0 };
		for (int i = 0; i < server_kind_num; i++)
		{
			ratio[i].value = fabs(server_kind_beta[i].cmratio - cmratio);
			ratio[i].position = i;
		}
		sort(ratio, ratio + server_kind_num, cmp_ratio);
		for (int i = 0; i < server_kind_num; i++)
		{
			ratio[i].value = 0.52*(i+1) + 0.48*(ratio[i].position+1);
		}
		sort(ratio, ratio + server_kind_num, cmp_ratio);
		for (int i = 0; i < server_kind_num; i++)
		{
			int pos_core = server_kind[server_kind_beta[ratio[i].position].subscript].core;
			int pos_mem = server_kind[server_kind_beta[ratio[i].position].subscript].memory;
			server[high].server_kind = server_kind_beta[ratio[i].position].subscript;
			server[high].core_A = pos_core / 2;
			server[high].core_B = pos_core / 2;
			server[high].memory_A = pos_mem / 2;
			server[high].memory_B = pos_mem / 2;
			server[high].subscript = high;
			if (greedy(high, server[high].core_A, server[high].memory_A, server[high].core_B, server[high].memory_B, Core, Memory))
			{
				++high;
				break;
			}
		}
	}
	return;
}

//deal the input
int main()
{
	int temp_int;
	string temp_string;
	string server_input;

	//input the server kind
	cin >> server_kind_num;
	getline(cin, server_input);
	for (int i = 0; i < server_kind_num; i++)
	{
		getline(cin, server_input);
		istringstream line(server_input);
		line >> temp_string;
		server_kind[i].name = temp_string.substr(1, temp_string.size() - 2);
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		server_kind[i].core = temp_int;
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		server_kind[i].memory = temp_int;
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		server_kind[i].price = temp_int;
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		server_kind[i].rent = temp_int;
		server_kind[i].vpratio = (double)server_kind[i].price / ((double)server_kind[i].core + (double)server_kind[i].memory);
		server_kind[i].cmratio = (double)server_kind[i].core / (double)server_kind[i].memory;
		server_kind[i].subscript = i;
	}
	//sort(server_kind, server_kind + server_kind_num, cmp_server);

	//input the vm kind
	cin >> vm_kind_num;
	getline(cin, server_input);
	for (int i = 0; i < vm_kind_num; i++)
	{
		getline(cin, server_input);
		istringstream line(server_input);
		line >> temp_string;
		vm_kind[i].name = temp_string.substr(1, temp_string.size() - 2);
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		vm_kind[i].core = temp_int;
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		vm_kind[i].memory = temp_int;
		line >> temp_string;
		temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
		vm_kind[i].doub = temp_int;
	}

	//input the vms that needs to be dealed in one day 
	cin >> days;
	for (int i = 0; i < days; i++)
	{
		//read the data
		int totalC = 0, totalM = 0;
		int delvm[MAX_ASK] = { 0 };
		int del = 0;
		cin >> ask_nums;
		getline(cin, server_input);
		for (int j = 0; j < ask_nums; j++)
		{
			getline(cin, server_input);
			istringstream line(server_input);
			line >> temp_string;
			string operation = temp_string.substr(1, temp_string.size() - 2);
			if (operation == "add")
			{
				line >> temp_string;
				string vm_kind_name = temp_string.substr(0, temp_string.size() - 1);
				for (int k = 0; k < vm_kind_num; k++)
				{
					if (vm_kind[k].name == vm_kind_name)
					{
						vm[++today].vm_kind = k;
						totalC += vm_kind[k].core;
						totalM += vm_kind[k].memory;
						break;
					}
				}
			}
			line >> temp_string;
			temp_int = stoi(temp_string.substr(0, temp_string.size() - 1));
			if (operation == "add")
			{
				vm[today].vm_id = temp_int;
				vm[today].subscript = today;
			}
			else if (operation == "del")
				delvm[del++] = temp_int;
		}

		//deal the request
		//add server
		low = high;
		sortVM();
		if (!handsome(0, low, totalC, totalM))
		{
			//for (int i = yesterday + 1; i <= today; i++)
			//{
			//	if (vm[i].usage == false)
			//		cout << i << " ";
			//}
			//cout << endl;
			GetServer(totalC, totalM, i);
		}
		sort(vm + yesterday + 1, vm + today + 1, cmp_sub);
		sort(server + low, server + high, cmp_kind);
		int update[MAX_ASK] = { 0 };
		for (int k = 0; k < high; k++)
		{
			update[server[k].subscript] = k;
		}
		for (int k = yesterday + 1; k <= today; k++)
		{
			vm[k].location = update[vm[k].location];
		}
		for (int k = low; k < high; k++)
		{
			server[k].subscript = k;
		}

		//delete VM
		for (int j = 0; j < del; j++)
		{
			for (int l = 0; l <= today; l++)
			{
				if (vm[l].vm_id == delvm[j] && vm[l].usage == true)
				{
					vm[l].usage = false;
					int delcore = vm_kind[vm[l].vm_kind].core;
					int delmem = vm_kind[vm[l].vm_kind].memory;
					if (vm_kind[vm[l].vm_kind].doub == 0)
					{
						if (vm[l].ABtag == 0)
						{
							server[vm[l].location].core_A += delcore;
							server[vm[l].location].memory_A += delmem;
						}
						else
						{
							server[vm[l].location].core_B += delcore;
							server[vm[l].location].memory_B += delmem;
						}
					}
					else
					{
						server[vm[l].location].core_A += delcore / 2;
						server[vm[l].location].core_B += delcore / 2;
						server[vm[l].location].memory_A += delmem / 2;
						server[vm[l].location].memory_B += delmem / 2;
					}
					int kind = server[vm[l].location].server_kind;
					if (server[vm[l].location].core_A == server_kind[kind].core / 2 && server[vm[l].location].core_B == server_kind[kind].core / 2 &&
						server[vm[l].location].memory_A == server_kind[kind].memory / 2 && server[vm[l].location].memory_B == server_kind[kind].memory / 2)
						server[vm[l].location].usage = 0;
				}
			}
		}

		//output the result
		int kind_num = 0;
		int purchase[MAX_SERVER_KIND] = { 0 };
		for (int j = low; j < high; j++)
		{
			if (purchase[server[j].server_kind] == 0)//haven't been purchased ever today
				kind_num++;
			purchase[server[j].server_kind]++;
		}

		cout << "(purchase, " << kind_num << ")\n";

		for (int j = 0; j < MAX_SERVER_KIND; j++)
			if (purchase[j] > 0)
				cout << '(' << server_kind[j].name << ", " << purchase[j] << ")\n";

		//out put migration(unfinished)
		cout << "(migration, 0)" << endl;

		for (int j = yesterday + 1; j <= today; j++)
			if (vm_kind[vm[j].vm_kind].doub == 0)//single
				cout << '(' << vm[j].location << ", " << (char)(vm[j].ABtag + 'A') << ")\n";
			else//double
				cout << '(' << vm[j].location << ")\n";

		yesterday = today;
	}
}



