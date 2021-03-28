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
#define PERCENT_BOUND 0.1
#define PERCENT_CHECK 0.3

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
	double cmratio;
	bool doub;// 0--single 1--double
};

struct Server
{
	short server_kind;
	short core_A, core_B, memory_A, memory_B;
	double cmratio;
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
	bool done = 0;//0--undo 1--do
};

struct Ratio
{
	double value;
	int position;
};

struct Mig_Move
{
	int vm_subscript;
	int server_subcript;
	bool ABtag;
};

struct Instruction
{
	int vm_id;
	bool operation;// 0--add 1--del
	bool done = 0;// 0--undo 1--do
};

Server_Kind server_kind[MAX_SERVER_KIND];
VM_Kind vm_kind[MAX_VM_KIND];
int server_kind_num, vm_kind_num, days, ask_nums, vm_sum = 0;;
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

bool cmp_Ssub(Server a, Server b)
{
	return a.subscript < b.subscript;
}

bool cmp_kind(Server a, Server b)
{
	return a.server_kind < b.server_kind;
}

bool cmp_cmrat(Server a, Server b)
{
	return a.cmratio < b.cmratio;
}

bool cmp_Cm(int a, int b)
{
	return vm_kind[vm[a].vm_kind].cmratio < vm_kind[vm[b].vm_kind].cmratio;
}

bool cmp_cM(int a, int b)
{
	return vm_kind[vm[a].vm_kind].cmratio > vm_kind[vm[b].vm_kind].cmratio;
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


bool handsome(int l, int h, int& Core, int& Memory, Instruction* ins)
{
	for (int i = l; i < h; i++)
	{
		greedy(i, server[i].core_A, server[i].memory_A, server[i].core_B, server[i].memory_B, Core, Memory);
	}
	for (int j = yesterday + 1; j <= today; j++)
	{
		if (vm[j].done == 0 && vm[j].usage == 1)
		{
			for (int k = 0; k < ask_nums; k++)
			{
				if (vm[j].vm_id == ins[k].vm_id)
				{
					ins[k].done = 1;
					vm[j].done = 1;
					break;
				}
			}
		}
	}
	for (int j = yesterday + 1; j <= today; j++)
	{
		if (vm[j].usage == 0 && vm[j].done == 0)
			return false;
	}
	return true;
}

void GetServer(int Core, int Memory, int day, Instruction* ins, int down)
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
			if (vm[i].usage == 0 && vm[i].done == 0)
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
			ratio[i].value = i + ratio[i].position;
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
				if ((tag >= 12) && (((server[high].core_A >= 0.2 * pos_core / 2) || (server[high].core_B >= 0.2 * pos_core / 2))
					&& ((server[high].memory_A >= 0.2 * pos_mem / 2) || (server[high].memory_B >= 0.2 * pos_mem / 2))))
				{
					for (int j = yesterday + 1; j <= today; j++)
					{
						if (vm[j].location == high && vm[j].usage == 1)
						{
							Core += vm_kind[vm[j].vm_kind].core;
							Memory += vm_kind[vm[j].vm_kind].memory;
							vm[j].usage = 0;
						}
					}
				}
				else
				{
					++high;
					for (int j = yesterday + 1; j <= today; j++)
					{
						if (vm[j].done == 0 && vm[j].usage == 1)
						{
							for (int k = 0; k < ask_nums; k++)
							{
								if (vm[j].vm_id == ins[k].vm_id)
								{
									ins[k].done = 1;
									vm[j].done = 1;
									break;
								}
							}
						}
					}
					for (int j = down; j < ask_nums; j++)
					{
						if (ins[j].operation == 1 && ins[j].done == 0)
						{
							ins[j].done = 1;
							down = j + 1;
							for (int l = 0; l <= today; l++)
							{
								if (vm[l].vm_id == ins[j].vm_id && vm[l].usage == true)
								{
									vm[l].usage = false;
									vm[l].done = 1;
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
									greedy(vm[l].location, server[vm[l].location].core_A, server[vm[l].location].memory_A, server[vm[l].location].core_B, server[vm[l].location].memory_B, Core, Memory);
								}
							}
						}
						else if (ins[j].operation == 0 && ins[j].done == 0)
						{
							break;
						}
					}
					break;
				}
			}
		}
	}
	return;
}

void vm_in_server(int subscript, int* vm_sub, int& len)
{
	len = 0;
	for (int i = 0; i <= yesterday; i++)
		if (vm[i].location == subscript && vm[i].usage == true) vm_sub[len++] = i;
}

void check_empty(int subscript)
{
	int kind = server[subscript].server_kind;
	if (server[subscript].core_A == server_kind[kind].core / 2 && server[subscript].core_B == server_kind[kind].core / 2 &&
		server[subscript].memory_A == server_kind[kind].memory / 2 && server[subscript].memory_B == server_kind[kind].memory / 2)
		server[subscript].usage = 0;
}

void get_ratio(int subscript)
{
	if (server[subscript].memory_A + server[subscript].memory_B != 0 && server[subscript].core_A + server[subscript].core_B != 0)
		server[subscript].cmratio = (double)(server[subscript].core_A + server[subscript].core_B) / (double)(server[subscript].memory_A + server[subscript].memory_B);
	else
		if (server[subscript].memory_A + server[subscript].memory_B == 0 && server[subscript].core_A + server[subscript].core_B == 0)
			server[subscript].cmratio = 0;//perfect placement
		else
			if (server[subscript].memory_A + server[subscript].memory_B == 0)
				server[subscript].cmratio = (double)(server[subscript].core_A + server[subscript].core_B);
			else
				server[subscript].cmratio = 1 / (double)(server[subscript].memory_A + server[subscript].memory_B);
}

bool adjust(int i, int l, int subscript)
{//i--source l--destination subscript--the vm waiting for migration
	VM_Kind it = vm_kind[vm[subscript].vm_kind];
	short& CA = server[l].core_A;
	short& MA = server[l].memory_A;
	short& CB = server[l].core_B;
	short& MB = server[l].memory_B;
	bool tag = 0, ABtag = vm[subscript].ABtag;

	if (it.doub == 1)//double
	{
		if (it.core / 2 <= CA && it.memory / 2 <= MA && it.core / 2 <= CB && it.memory / 2 <= MB)
		{
			tag = 1;
			vm[subscript].location = server[l].subscript;
			server[l].usage = 1;
			CA -= it.core / 2;
			MA -= it.memory / 2;
			CB -= it.core / 2;
			MB -= it.memory / 2;
		}
	}
	else//single
	{
		if (it.core <= CA && it.memory <= MA && it.core <= CB && it.memory <= MB)//can be located in A or B
		{
			vm[subscript].location = server[l].subscript;
			server[l].usage = 1;
			if (CA + MA >= CB + MB)//the rest of A is more
			{
				tag = 1;
				CA -= it.core;
				MA -= it.memory;
				vm[subscript].ABtag = 0;
			}
			else//the rest of B is more
			{
				tag = 1;
				CB -= it.core;
				MB -= it.memory;
				vm[subscript].ABtag = 1;
			}
		}
		else
			if (it.core <= CA && it.memory <= MA)//only can be located in A
			{
				tag = 1;
				vm[subscript].location = server[l].subscript;
				server[l].usage = 1;
				CA -= it.core;
				MA -= it.memory;
				vm[subscript].ABtag = 0;
			}
			else
				if (it.core <= CB && it.memory <= MB)//only can be located in B
				{
					tag = 1;
					vm[subscript].location = server[l].subscript;
					server[l].usage = 1;
					CB -= it.core;
					MB -= it.memory;
					vm[subscript].ABtag = 1;
				}
	}

	if (tag)//migrate successfully, rest addition
	{
		if (it.doub == 1)
		{
			server[i].core_A += it.core / 2;
			server[i].memory_A += it.memory / 2;
			server[i].core_B += it.core / 2;
			server[i].memory_B += it.memory / 2;
		}
		else
		{
			if (ABtag == 0)
			{
				server[i].core_A += it.core;
				server[i].memory_A += it.memory;
			}
			else
			{
				server[i].core_B += it.core;
				server[i].memory_B += it.memory;
			}
		}
		//recalculate cmratio of these two servers
		get_ratio(i);
		get_ratio(l);
	}
	return tag;
}

void migration(int& mig_num, Mig_Move* mig_move, int MAX_mig)
{
	bool good[MAX_ASK / 10] = { 0 };
	for (int i = 0; i < high; i++)
		get_ratio(i);

	sort(server, server + high, cmp_cmrat);

	for (int i = 0; i < high; i++)
		if (server[i].cmratio == 0)
			good[i] = 1;//perfect don't needs adjustment
		else
			break;

	int i = 0, j = high - 1;
	double boundary_low = server[(int)(high * PERCENT_BOUND)].cmratio;
	double boundary_high = server[(int)(high * (1 - PERCENT_BOUND))].cmratio;
	while (mig_num < MAX_mig)
	{
		while (server[i].usage == 0 || good[i]) i++;
		while (server[j].usage == 0 || good[j]) j--;

		if (i > (int)(high * PERCENT_CHECK) && j < (int)(high * (1 - PERCENT_CHECK))) break;

		if (i <= (int)(high * PERCENT_CHECK))
			if (server[i].cmratio * server[j].cmratio <= 1 || (server[i].cmratio * server[j].cmratio > 1 && j < (int)(high * (1 - PERCENT_CHECK))))
			{
				int vm_series[MAX_ASK / 100], len = 0;
				vm_in_server(server[i].subscript, vm_series, len);
				sort(vm_series, vm_series + len, cmp_Cm);//first one has bigger core and smaller memory

				for (int k = 0; k < len; k++)
				{
					if (mig_num == MAX_mig) break;
					if (vm_kind[vm[vm_series[k]].vm_kind].cmratio < server[i].cmratio) break;
					if (!good[i])
						for (int l = (int)(high * (1 - PERCENT_CHECK)); l <= high - 1; l++)
						{
							//if (vm_kind[vm[vm_series[k]].vm_kind].cmratio > server[l].cmratio) break;
							if (!good[l])//need adjustment
							{
								if (adjust(i, l, vm_series[k]))
								{
									mig_move[mig_num++] = { vm_series[k],server[l].subscript,vm[vm_series[k]].ABtag };
									if (server[i].cmratio > boundary_low) good[i] = true;//have been adjusted well
									check_empty(i);
									if (server[l].cmratio < boundary_high && l>(int)(high * (1 - PERCENT_BOUND))) good[l] = true;//have been adjusted well
									break;
								}
							}
						}
				}
				i++;
			}

		if (j >= (int)(high * (1 - PERCENT_CHECK)))
			if (server[i].cmratio * server[j].cmratio > 1 || (server[i].cmratio * server[j].cmratio <= 1 && i > (int)(high * PERCENT_CHECK)))
			{
				int vm_series[MAX_ASK / 100], len = 0;
				vm_in_server(server[j].subscript, vm_series, len);
				sort(vm_series, vm_series + len, cmp_cM);//first one has smaller core and bigger memory
				for (int k = 0; k < len; k++)
				{
					if (mig_num == MAX_mig) break;
					if (vm_kind[vm[vm_series[k]].vm_kind].cmratio > server[j].cmratio) break;
					if (!good[j])
						for (int l = (int)(high * PERCENT_CHECK); l >= 0; l--)
						{
							//if (vm_kind[vm[vm_series[k]].vm_kind].cmratio < server[l].cmratio) break;
							if (!good[l])//need adjustment
							{
								if (adjust(j, l, vm_series[k]))
								{
									mig_move[mig_num++] = { vm_series[k],server[l].subscript,vm[vm_series[k]].ABtag };
									if (server[j].cmratio < boundary_high) good[j] = true;//have been adjusted well
									check_empty(j);
									if (server[l].cmratio > boundary_low && l < (int)(high * PERCENT_BOUND)) good[l] = true;//have been adjusted well
									break;
								}
							}
						}
				}
				j--;
			}
	}
	sort(server, server + high, cmp_Ssub);
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

		vm_kind[i].cmratio = vm_kind[i].core / vm_kind[i].memory;
	}

	//input the vms that needs to be dealed in one day 
	cin >> days;
	for (int i = 0; i < days; i++)
	{
		//migration
		int mig_num = 0;
		Mig_Move mig_move[MAX_ASK * 5 / 1000];
		migration(mig_num, mig_move, vm_sum / 3000);

		//read the data
		int totalC = 0, totalM = 0;
		Instruction ins[MAX_ASK / 10];
		int down = 0;

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
				ins[j].operation = 0;
				ins[j].vm_id = temp_int;
			}
			else if (operation == "del")
			{
				ins[j].operation = 1;
				ins[j].vm_id = temp_int;
			}
		}

		//deal the request
		//add server
		low = high;
		sortVM();
		if (!handsome(0, low, totalC, totalM, ins))
		{
			GetServer(totalC, totalM, i, ins, down);
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
		for (int j = 0; j < high; j++)
			server[j].cmratio = (double)(server[j].core_A + server[j].core_B) / (double)(server[j].memory_A + server[j].memory_B);
		cout << "(migration, " << mig_num << ")\n";
		for (int j = 0; j < mig_num; j++)
			if (vm_kind[vm[mig_move[j].vm_subscript].vm_kind].doub == 0)//single
				cout << '(' << vm[mig_move[j].vm_subscript].vm_id << ", " << mig_move[j].server_subcript << ", " << (char)(mig_move[j].ABtag + 'A') << ")\n";
			else//double
				cout << '(' << vm[mig_move[j].vm_subscript].vm_id << ", " << mig_move[j].server_subcript << ")\n";

		for (int j = yesterday + 1; j <= today; j++)
			if (vm_kind[vm[j].vm_kind].doub == 0)//single
				cout << '(' << vm[j].location << ", " << (char)(vm[j].ABtag + 'A') << ")\n";
			else//double
				cout << '(' << vm[j].location << ")\n";

		yesterday = today;
	}
}





