#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Command
{
	int address;
	string opcode;
	string operand;
};

struct Symbol_Table
{
	int address;
	string symbol;
};

string Ox_to_Binary(char ox)
{
	if (ox == '0')
		return "0000";
	else if (ox == '1')
		return "0001";
	else if (ox == '2')
		return "0010";
	else if (ox == '3')
		return "0011";
	else if (ox == '4')
		return "0100";
	else if (ox == '5')
		return "0101";
	else if (ox == '6')
		return "0110";
	else if (ox == '7')
		return "0111";
	else if (ox == '8')
		return "1000";
	else if (ox == '9')
		return "1001";
	else if (ox == 'A')
		return "1010";
	else if (ox == 'B')
		return "1011";
	else if (ox == 'C')
		return "1100";
	else if (ox == 'D')
		return "1101";
	else if (ox == 'E')
		return "1110";
	else if (ox == 'F')
		return "1111";
	else
		return "";
}

string Decimal_to_Binary(int decimal,int number,bool pc)
{
	int offset_max = pow(2, number - 1) - 1;
	int offset_min = (-1) * pow(2, number - 1);
	string result = "";
	if(pc)
		decimal = decimal - 1;
	if (decimal >= offset_min && decimal <= offset_max)
	{
		if (decimal > 0)
		{
			while (decimal != 0)
			{
				if (result.size() != 0)
				{
					result.insert(0,to_string(decimal % 2));
					decimal = decimal / 2;
				}
				else
				{
					result += to_string(decimal % 2);
					decimal = decimal / 2;
				}
			}
			while (result.size() < number)
				result.insert(0, "0");
			return result;
		}
		else if (decimal == 0)
		{
			for (int i = 0; i < number; i++)
				result += "0";
			return result;
		}
		else
		{
			int abs_decimal = abs(decimal);
			while (abs_decimal != 0)
			{
				if (result.size() != 0)
				{
					result.insert(0, to_string(abs_decimal % 2));
					abs_decimal = abs_decimal / 2;
				}
				else
				{
					result += to_string(abs_decimal % 2);
					abs_decimal = abs_decimal / 2;
				}
			}
			while (result.size() < number)
				result.insert(0, "0");
			for (int i = 0; i < result.size(); i++)
			{
				if (result[i] == '0')
					result[i] = '1';
				else if (result[i] == '1')
				{
					result[i] = '0';
				}
			}
			int tag = 1;
			for (int i = result.size()-1; i >=0; i--)
			{
				int num = result[i] - 48;
				if (num + tag == 2)
				{
					result[i] = '0';
					tag = 1;
				}
				else
				{
					if (num == 0)
						result[i] = '1';
					break;
				}
			}
			return result;
		}
	}
	return "";
}

string reg_pos(string reg)
{
	if (reg[0] == 'R' && reg.size() == 2)
	{
		if (reg[1] == '0')
			return "000";
		else if (reg[1] == '1')
			return "001";
		else if (reg[1] == '2')
			return "010";
		else if (reg[1] == '3')
			return "011";
		else if (reg[1] == '4')
			return "100";
		else if (reg[1] == '5')
			return "101";
		else if (reg[1] == '6')
			return "110";
		else if (reg[1] == '7')
			return "111";
	}
	return "";
}

string judge_offset(string offset, int number,bool pc)
{
	string pure_number(offset, 1);
	if (offset[0] != '#')
		return "";
	for (int i = 0; i < pure_number.size(); ++i)
	{
		if (!isdigit(pure_number[0]) && pure_number[0] != '-')
			return "";
		else if (i != 0 && !isdigit(pure_number[i]))
			return "";
	}
	int offset_number = stoi(pure_number);
	string result = Decimal_to_Binary(offset_number, number,pc);
	if (result != "")
		return result;
	return "";
}

void assemble(string infile, string outfile)
{
	int LC = 0;
	string Start_Address;
	string line, word;
	ifstream ifs(infile);
	ofstream ofs(outfile);
	vector<Command> commands;
	vector<Symbol_Table> symbols;
	while (getline(ifs, line))
	{
		Command Orig_Command;
		Symbol_Table Orig_Symbol;
		if (line[0] != ';' && !line.empty())
		{
			istringstream record(line);
			record >> word;
			string Deal_BR(word, 0, 2);
			if (word == ".ORIG")
			{
				record >> word;
				string StartAddress(word, 1);
				Start_Address = word;
				LC = stoi(StartAddress);
			}
			else if (word == ".END")
			{
				break;
			}
			else if (word == "ADD" || word == "AND" || word == "TRAP" || word == "JMP"
				|| word == "JSR" || word == "JSRR" || word == "LD" || word == "LDI"
				|| word == "LDR" || word == "LEA" || word == "NOT" || word == "RET"
				|| word == "RTI" || word == "ST" || word == "STI" || word == "STR")
			{
				Orig_Command.address = LC++;
				Orig_Command.opcode = word;
				if (word!="RET"&&word!="RTI")
				{
					while (record >> word)
					{
						if (word[0] == ';')
							break;
						Orig_Command.operand += word;
					}
				}
				commands.push_back(Orig_Command);
			}
			else if (Deal_BR == "BR")
			{
				Orig_Command.address = LC++;
				Orig_Command.opcode = word;
				record >> word;
				Orig_Command.operand = word;
				commands.push_back(Orig_Command);
			}
			else
			{
				Orig_Symbol.address = LC;
				Orig_Symbol.symbol = word;
				symbols.push_back(Orig_Symbol);
				Orig_Command.address = LC;
				if (record >> word)
				{
					string Deal_BR_temp(word, 0, 2);
					if (word == "ADD" || word == "AND" || word == "TRAP" || word == "JMP"
						|| word == "JSR" || word == "JSRR" || word == "LD" || word == "LDI"
						|| word == "LDR" || word == "LEA" || word == "NOT" || word == "RET"
						|| word == "RTI" || word == "ST" || word == "STI" || word == "STR")
					{
						Orig_Command.opcode = word;
						if (word != "RTI" && word != "RET")
						{
							while (record >> word)
							{
								if (word[0] == ';')
									break;
								Orig_Command.operand += word;
							}
						}
						else if (!record)
						{
							record >> word;
							Orig_Command.operand = word;
						}
						commands.push_back(Orig_Command);
						++LC;
					}
					else if (Deal_BR_temp == "BR")
					{
						Orig_Command.opcode = word;
						record >> word;
						Orig_Command.operand = word;
						commands.push_back(Orig_Command);
						++LC;
					}
					else
					{
						Orig_Command.opcode = word;
						if (word == ".FILL")
						{
							record >> word;
							Orig_Command.operand = word;
							++LC;
						}
						else if (word == ".BLKW")
						{
							record >> word;
							Orig_Command.operand = word;
							string temp(word, 1);
							LC = LC + stoi(temp);
						}
						else if (word == ".STRINGZ")
						{
							int i = 0;
							int first, last;
							while (line[i] != '\"')
								i++;
							first = i;
							i++;
							while (line[i] != '\"')
								i++;
							last = i;
							string str(line, first, last - first + 1);
							Orig_Command.operand = str;
							int temp = Orig_Command.operand.size() - 1;
							LC = LC + temp;
						}
						else
							cout << "is a Error Command!" << endl;
						commands.push_back(Orig_Command);
					}
				}
				else
					cout << Orig_Symbol.symbol << " is a Error Commond!" << endl;
			}
		}
		else
			continue;
	}
	string Start_Add = "";
	if (word.size() != 5 || word[0] != 'x')
	{
		if (Ox_to_Binary(Start_Address[1]) != "" && Ox_to_Binary(Start_Address[2]) != "" &&
			Ox_to_Binary(Start_Address[3]) != "" && Ox_to_Binary(Start_Address[4]) != "")
		{
			Start_Add += Ox_to_Binary(Start_Address[1]);
			Start_Add += Ox_to_Binary(Start_Address[2]);
			Start_Add += Ox_to_Binary(Start_Address[3]);
			Start_Add += Ox_to_Binary(Start_Address[4]);
			ofs << Start_Add << endl;
		}
		else
			cout << "Start Address is Wrong!" << endl;
	}
	else
		cout << "Start Address is Wrong!" << endl;
	for (auto it = commands.begin(); it != commands.end(); ++it)
	{
		bool Error_Tag = false;
		string result = "";
		if (it->opcode == "ADD" || it->opcode == "AND")
		{
			if (it->opcode == "ADD")
				result += "0001";
			else if (it->opcode == "AND")
				result += "0101";
			word = it->operand;
			if (word.size() < 8 || word[2] != ',' || word[5] != ',')
				Error_Tag = true;
			else
			{
				string DR(word, 0, 2);
				string SR1(word, 3, 2);
				string SR2(word, 6);
				if (reg_pos(DR) != "" && reg_pos(SR1) != "")
				{
					result += reg_pos(DR);
					result += reg_pos(SR1);
				}
				else
					Error_Tag = true;
				if (reg_pos(SR2) != "")
				{
					result += "000";
					result += reg_pos(SR2);
					ofs << result << endl;
				}
				else if (judge_offset(SR2, 5,false) != "")
				{
					result += "1";
					result += judge_offset(SR2, 5,false);
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode == "LD" || it->opcode == "LDI" || it->opcode == "LEA" || it->opcode == "ST" || it->opcode == "STI")
		{
			if (it->opcode == "LD")
				result += "0010";
			else if (it->opcode == "LDI")
				result += "1010";
			else if (it->opcode == "LEA")
				result += "1110";
			else if (it->opcode == "ST")
				result += "0011";
			else if (it->opcode == "STI")
				result += "1011";
			word = it->operand;
			if (word.size() < 4 || word[2] != ',')
				Error_Tag = true;
			else
			{
				string reg(word, 0, 2);
				string label(word, 3);
				if (reg_pos(reg) != "")
				{
					result += reg_pos(reg);
				}
				else
					Error_Tag = true;
				int temp_offset = 0;
				int temp_tag = 0;
				for (auto ptr = symbols.begin(); ptr != symbols.end(); ++ptr)
				{
					if (ptr->symbol == label)
					{
						temp_offset = ptr->address - it->address;
						temp_tag = 1;
						break;
					}
				}
				if (temp_tag == 1)
				{
					string str_offset = Decimal_to_Binary(temp_offset, 9,true);
					if (str_offset != "")
					{
						result += str_offset;
						ofs << result << endl;
					}
					else
						Error_Tag = true;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode == "RET" || it->opcode == "RTI")
		{
			if (it->opcode == "RET")
				result = "1100000111000000";
			else if (it->opcode == "RTI")
				result = "1000000000000000";
			if (it->operand != "")
				Error_Tag = true;
			else
				ofs << result << endl;
		}
		else if (it->opcode == "NOT")
		{
			result += "1001";
			word = it->operand;
			if (word.size() != 5 || word[2] != ',')
				Error_Tag = true;
			else
			{
				string DR(word, 0, 2);
				string SR(word, 3, 2);
				if (reg_pos(DR) != "" && reg_pos(SR) != "")
				{
					result += reg_pos(DR);
					result += reg_pos(SR);
					result += "111111";
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode=="JMP"||it->opcode=="JSRR")
		{
			if (it->opcode == "JMP")
				result += "1100";
			else if (it->opcode == "JSRR")
				result += "0100";
			result += "000";
			word = it->operand;
			if (word.size() != 2 || reg_pos(word) == "")
				Error_Tag = true;
			else
			{
				result += reg_pos(word);
				result += "000000";
				ofs << result << endl;
			}
		}
		else if (it->opcode=="LDR"||it->opcode=="STR")
		{
			if (it->opcode == "LDR")
				result += "0110";
			else if (it->opcode == "STR")
				result += "0111";
			word = it->operand;
			if (word.size() < 8 || word[2] != ',' || word[5] != ',')
				Error_Tag = true;
			else
			{
				string DR(word, 0, 2);
				string SR1(word, 3, 2);
				string SR2(word, 6);
				if (reg_pos(DR) != "" && reg_pos(SR1) != "")
				{
					result += reg_pos(DR);
					result += reg_pos(SR1);
				}
				else
					Error_Tag = true;
				if (judge_offset(SR2, 6,true) != "")
				{
					result += judge_offset(SR2, 6, false);
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode == "JSR")
		{
			result += "01001";
			word = it->operand;
			int temp_offset = 0;
			int temp_tag = 0;
			for (auto ptr = symbols.begin(); ptr != symbols.end(); ++ptr)
			{
				if (ptr->symbol == word)
				{
					temp_offset = ptr->address - it->address;
					temp_tag = 1;
					break;
				}
			}
			if (temp_tag == 1)
			{
				string str_offset = Decimal_to_Binary(temp_offset, 11,true);
				if (str_offset != "")
				{
					result += str_offset;
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
			else
				Error_Tag = true;
		}
		else if (it->opcode == "TRAP")
		{
			result += "11110000";
			word = it->operand;
			if (word.size() != 3 || word[0] != 'x')
				Error_Tag = true;
			else
			{
				if (Ox_to_Binary(word[1]) != "" && Ox_to_Binary(word[2]) != "")
				{
					result += Ox_to_Binary(word[1]);
					result += Ox_to_Binary(word[2]);
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode[0] == 'B' && it->opcode[1] == 'R')
		{
			result += "0000";
			if (it->opcode == "BR")
				result += "000";
			else if (it->opcode == "BRn")
				result += "100";
			else if (it->opcode == "BRz")
				result += "010";
			else if (it->opcode == "BRp")
				result += "001";
			else if (it->opcode == "BRzp")
				result += "011";
			else if (it->opcode == "BRnp")
				result += "101";
			else if (it->opcode == "BRnz")
				result += "110";
			else if (it->opcode == "BRnzp")
				result += "111";
			else
				Error_Tag = true;
			word = it->operand;
			int temp_offset = 0;
			int temp_tag = 0;
			for (auto ptr = symbols.begin(); ptr != symbols.end(); ++ptr)
			{
				if (ptr->symbol == word)
				{
					temp_offset = ptr->address - it->address;
					temp_tag = 1;
					break;
				}
			}
			if (temp_tag == 1)
			{
				string str_offset = Decimal_to_Binary(temp_offset, 9,true);
				if (str_offset != "")
				{
					result += str_offset;
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
			else
				Error_Tag = true;
		}
		else if (it->opcode == ".FILL")
		{
			word = it->operand;
			if (word.size() != 5 || word[0] != 'x')
				Error_Tag = true;
			else
			{
				if (Ox_to_Binary(word[1]) != "" && Ox_to_Binary(word[2]) != "" && 
					Ox_to_Binary(word[3]) != "" && Ox_to_Binary(word[4]) != "")
				{
					result += Ox_to_Binary(word[1]);
					result += Ox_to_Binary(word[2]);
					result += Ox_to_Binary(word[3]);
					result += Ox_to_Binary(word[4]);
					ofs << result << endl;
				}
				else
					Error_Tag = true;
			}
		}
		else if (it->opcode == ".BLKW")
		{
			word = it->operand;
			if (word[0] != '#')
				Error_Tag = true;
			else
			{
				string str_num(word, 1);
				for (int i = 0; i < str_num.size(); i++)
				{
					if (!isdigit(word[i]))
					{
						Error_Tag = true;
						break;
					}
				}
				if (!Error_Tag)
				{
					int int_num = stoi(str_num);
					for (int i = 0; i < int_num; ++i)
						ofs << "0000000000000000" << endl;
				}
			}			
		}
		else if (it->opcode == ".STRINGZ")
		{
			word = it->operand;
			string str(word, 1, word.size() - 2);
			for (int i = 0; i < str.size(); i++)
			{
				string out = "";
				int dec = str[i];
				while (dec != 0)
				{
					if (out.size() != 0)
					{
						out.insert(0,to_string(dec % 2));
						dec = dec / 2;
					}
					else
					{
						out += to_string(dec % 2);
						dec = dec / 2;
					}
				}
				while (out.size() < 16)
				{
					out.insert(0, "0");
				}
				ofs << out << endl;
			}
			ofs << "0000000000000000" << endl;
		}
		if (Error_Tag == true)
		{
			cout << it->opcode << " " << it->operand << " is a wrong command";
			break;
		}
	}
}

//int main()
//{
//	string infile = "D:\\asm\\lab4.asm";
//	string outfile = "D:\\asm\\lab4.bin";
//	assemble(infile, outfile);
//}

int main(int argc, char* argv[])
{
	int i = 0, j = 0;
	string infile, outfile, name;
	char purename[20] = { '\0' };
	if (argc == 2)
	{
		name = argv[1];
		for (i = name.size() - 4; i < name.size(); i++)
			purename[j++] = name[i];
		name = purename;
		if (name != ".asm")
			cout << "This is not a .asm file!" << endl;
		else
		{
			name = argv[1];
			for (i = 0; i < 20; i++)
				purename[i] = '\0';
			for (i = 0; i < name.size() - 4; i++)
				purename[i] = name[i];
			name = purename;
			infile = "D:\\asm\\" + name + ".asm";
			outfile = "D:\\asm\\" + name + ".bin";
			assemble(infile, outfile);
		}
	}
	else
		cout << "Error Command!" << endl;
}