#include <stddef.h>
#include <iostream>
#include "instruction.h"

uint32_t gp_reg[4] = { 0, 0, 0, 0 };
uint32_t stack_reg[1] = { 0 };
uint64_t IP = 0;
uint32_t RAM[65535];
uint32_t STACK[1024];
bool cmp_set = false;
bool cmp_NE = 0;
bool cmp_EQ = 0;
bool cmp_GT = 0;
bool cmp_LT = 0;

uint32_t ROM_INSTRUCTION[256];
instruction_input ROM_DATA[256];

Instruction instructions[256];

void init_instructions()
{
	// NOOP
	instructions[0].instructionAction = [](instruction_input* data)
	{
	};

	// MVRR
	instructions[1].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			gp_reg[data->data[1]] = gp_reg[data->data[0]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// MVDR
	instructions[2].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			gp_reg[data->data[1]] = data->data[0];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// MVAR
	instructions[3].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			gp_reg[data->data[1]] = RAM[data->data[0]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// MVRA
	instructions[4].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			RAM[data->data[1]] = gp_reg[data->data[0]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// MVARD
	instructions[5].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			gp_reg[data->data[1]] = RAM[gp_reg[data->data[0]]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// MVRAD
	instructions[6].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 2)
		{
			RAM[gp_reg[data->data[1]]] = gp_reg[data->data[0]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// PSHR
	instructions[7].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 1)
		{
			STACK[stack_reg[0]] = gp_reg[data->data[0]];
			stack_reg[0]++;
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// PSHD
	instructions[8].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 1)
		{
			STACK[stack_reg[0]] = data->data[0];
			stack_reg[0]++;
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// POPR
	instructions[9].instructionAction = [](instruction_input* data)
	{

		if (data->data.size() == 1)
		{
			gp_reg[data->data[0]] = STACK[stack_reg[0]];
			STACK[stack_reg[0]] = 0;
			stack_reg[0]--;
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// ADD
	instructions[10].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{
			gp_reg[data->data[0]] = gp_reg[data->data[0]] + gp_reg[data->data[1]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// SUB
	instructions[11].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{
			gp_reg[data->data[0]] = gp_reg[data->data[0]] - gp_reg[data->data[1]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// OUT
	instructions[12].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{
			if (data->data[1] == 0)
			{
				std::cout << "num_out: " << gp_reg[data->data[0]] << '\n';
			}

			if (data->data[1] == 1)
			{
				std::cout << "char_out: " << (char)gp_reg[data->data[0]] << '\n';
			}
			
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// IN
	instructions[13].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{

		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// INT
	instructions[14].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{

		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JMPR
	instructions[15].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			IP = gp_reg[data->data[0]];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JMPD
	instructions[16].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			IP = data->data[0];
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// CMP
	instructions[17].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{
			if (gp_reg[data->data[0]] != gp_reg[data->data[1]])
			{
				cmp_NE = true;
				cmp_set = true;
				return;
			}
			else if (gp_reg[data->data[0]] == gp_reg[data->data[1]])
			{
				cmp_EQ = true;
				cmp_set = true;
				return;
			}
			else if (gp_reg[data->data[0]] > gp_reg[data->data[1]])
			{
				cmp_GT = true;
				cmp_set = true;
				return;
			}
			if (gp_reg[data->data[0]] < gp_reg[data->data[1]])
			{
				cmp_LT = true;
				cmp_set = true;
				return;
			}

			cmp_set = false;
			cmp_NE = false;
			cmp_EQ = false;
			cmp_GT = false;
			cmp_LT = false;
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// CPRD
	instructions[18].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 2)
		{
			if (gp_reg[data->data[0]] != data->data[1])
			{
				cmp_NE = true;
				cmp_set = true;
			}
			if (gp_reg[data->data[0]] == data->data[1])
			{
				cmp_EQ = true;
				cmp_set = true;
			}
			if (gp_reg[data->data[0]] > data->data[1])
			{
				cmp_GT = true;
				cmp_set = true;
			}
			if (gp_reg[data->data[0]] < data->data[1])
			{
				cmp_LT = true;
				cmp_set = true;
			}

			if (!gp_reg[data->data[0]] != data->data[1] and !gp_reg[data->data[0]] == data->data[1] and !gp_reg[data->data[0]] > data->data[1] and !gp_reg[data->data[0]] < data->data[1])
			{
				cmp_set = false;
				cmp_NE = false;
				cmp_EQ = false;
				cmp_GT = false;
				cmp_LT = false;
			}

		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JINE
	instructions[19].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			if (cmp_set and cmp_NE)
			{
				IP = data->data[0];
			}
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JIEQ
	instructions[20].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			if (cmp_set and cmp_EQ)
			{
				IP = data->data[0];
			}
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JIGT
	instructions[21].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			if (cmp_set and cmp_GT)
			{
				IP = data->data[0];
			}
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// JILT
	instructions[22].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			if (cmp_set and cmp_LT)
			{
				IP = data->data[0];
			}
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// INC
	instructions[23].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			gp_reg[data->data[0]] = gp_reg[data->data[0]] + 1;
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};

	// DEC
	instructions[24].instructionAction = [](instruction_input* data)
	{
		if (data->data.size() == 1)
		{
			if (gp_reg[data->data[0]] != 0)
			{
				gp_reg[data->data[0]] = gp_reg[data->data[0]] - 1;
			}
			
		}
		else
		{
			std::cout << "Error: Invalid number of operands\n";
		}
	};
}

constexpr int NOOP = 0;
constexpr int MVRR = 1;
constexpr int MVDR = 2;
constexpr int MVAR = 3;
constexpr int MVRA = 4;
constexpr int MVARD = 5;
constexpr int MVRAD = 6;
constexpr int PSHR = 7;
constexpr int PSHD = 8;
constexpr int POPR = 9;
constexpr int ADD = 10;
constexpr int SUB = 11;
constexpr int OUT = 12;
constexpr int IN = 13;
constexpr int INT = 14;
constexpr int JMPR = 15;
constexpr int JMPD = 16;
constexpr int CMP = 17;
constexpr int CPRD = 18;
constexpr int JINE = 19;
constexpr int JIEQ = 20;
constexpr int JIGT = 21;
constexpr int JILT = 22;
constexpr int INC = 23;
constexpr int DEC = 24;

constexpr int AX = 0;
constexpr int BX = 1;
constexpr int CX = 2;
constexpr int DX = 3;

int instr_addr = 0;

void instruction(int instruction, instruction_input data)
{
	ROM_INSTRUCTION[instr_addr] = instruction;
	ROM_DATA[instr_addr] = data;
	instr_addr++;
}

std::string conv_instr_name(int instr)
{
	switch (instr)
	{
	case 0:
		return "NOOP";
		break;
	case 1:
		return "MVRR";
		break;
	case 2:
		return "MVDR";
		break;
	case 3:
		return "MVAR";
		break;
	case 4:
		return "MVRA";
		break;
	case 5:
		return "MVARD";
		break;
	case 6:
		return "MVRAD";
		break;
	case 7:
		return "PSHR";
		break;
	case 8:
		return "PSHD";
		break;
	case 9:
		return "POPR";
		break;
	case 10:
		return "ADD";
		break;
	case 11:
		return "SUB";
		break;
	case 12:
		return "OUT";
		break;
	case 13:
		return "IN";
		break;
	case 14:
		return "INT";
		break;
	case 15:
		return "JMPR";
		break;
	case 16:
		return "JMPD";
		break;
	case 17:
		return "CMP";
		break;
	case 18:
		return "CMPRD";
		break;
	case 19:
		return "JINE";
		break;
	case 20:
		return "JIEQ";
		break;
	case 21:
		return "JIGT";
		break;
	case 22:
		return "JILT";
		break;
	case 23:
		return "INC";
		break;
	case 24:
		return "DEC";
		break;
	}

	return "INVAL";
}

int instr_input_num(int instr)
{
	switch (instr)
	{
	case 0:
		return 0;
		break;
	case 1:
		return 2;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 2;
		break;
	case 4:
		return 2;
		break;
	case 5:
		return 2;
		break;
	case 6:
		return 2;
		break;
	case 7:
		return 1;
		break;
	case 8:
		return 1;
		break;
	case 9:
		return 1;
		break;
	case 10:
		return 2;
		break;
	case 11:
		return 2;
		break;
	case 12:
		return 2;
		break;
	case 13:
		return 2;
		break;
	case 14:
		return 1;
		break;
	case 15:
		return 1;
		break;
	case 16:
		return 1;
		break;
	case 17:
		return 2;
		break;
	case 18:
		return 2;
		break;
	case 19:
		return 1;
		break;
	case 20:
		return 1;
		break;
	case 21:
		return 1;
		break;
	case 22:
		return 1;
		break;
	case 23:
		return 1;
		break;
	case 24:
		return 1;
		break;
	}

	return NULL;
}



int main()
{
	init_instructions();

	// Load first 2 values
	instruction(MVDR, instruction_input(1, AX));
	instruction(MVDR, instruction_input(1, BX));

	instruction(MVDR, instruction_input(0, CX));

	// Add the values
	instruction(ADD, instruction_input(AX, BX));

	// Load result into RAM
	instruction(MVRAD, instruction_input(AX, CX));

	// Set up pointer for the numbers to add
	instruction(MVRR, instruction_input(CX, DX));
	instruction(DEC, instruction_input(DX));

	// Put the values into AX and BX
	instruction(MVARD, instruction_input(CX, AX));
	instruction(MVARD, instruction_input(DX, BX));

	// Increment loop index
	instruction(INC, instruction_input(CX));

	// Jump to the start if loop index < 20
	instruction(CPRD, instruction_input(CX, 20));
	instruction(JILT, instruction_input(3));

	// Output values
	instruction(MVAR, instruction_input(0, CX));
	instruction(OUT, instruction_input(CX, 0));

	instruction(MVAR, instruction_input(1, CX));
	instruction(OUT, instruction_input(CX, 0));

	instruction(MVAR, instruction_input(2, CX));
	instruction(OUT, instruction_input(CX, 0));

	instruction(MVAR, instruction_input(3, CX));
	instruction(OUT, instruction_input(CX, 0));

	instruction(MVAR, instruction_input(4, CX));
	instruction(OUT, instruction_input(CX, 0));



	while(IP < 255)
	{
		std::cout << IP << " : " << conv_instr_name(ROM_INSTRUCTION[IP]) << ", ";

		if (instr_input_num(ROM_INSTRUCTION[IP]) != 0)
		{
			std::cout << ROM_DATA[IP].data[0] << " ";
		}
		if (instr_input_num(ROM_INSTRUCTION[IP]) == 2)
		{
			std::cout << ROM_DATA[IP].data[1] << "\n";
		}
		else
		{
			std::cout << "\n";
		}

		bool _continue = true;
		while (!_continue)
		{
			std::string command;
			std::cin >> command;

			if (strstr(command.c_str(), "print_addr:") != NULL)
			{
				std::string num_text;
				
				num_text.push_back(command[11]);
				if (command.size() == 13)
				{
					num_text.push_back(command[12]);
				}
				if (command.size() == 14)
				{
					num_text.push_back(command[13]);
				}

				std::cout << "print_addr: " << RAM[atoi(num_text.c_str())] << '\n';
			}
			if (strstr(command.c_str(), "print_reg:") != NULL)
			{
				std::cout << "print_reg: " << gp_reg[atoi(&command[10])] << '\n';
			}

			if (strstr(command.c_str(), "is_cmp_set") != NULL)
			{
				if (cmp_set)
				{

					std::cout << "cmp: " << "true" << '\n';
					if (cmp_NE)
					{
						std::cout << "cmp: " << "!=" << '\n';
					}
					if (cmp_EQ)
					{
						std::cout << "cmp: " << "==" << '\n';
					}
					if (cmp_GT)
					{
						std::cout << "cmp: " << ">" << '\n';
					}
					if (cmp_LT)
					{
						std::cout << "cmp: " << "<" << '\n';
					}
				}
				else
				{
					std::cout << "cmp: " << "false" << '\n';
				}
				
			}

			if (strstr(command.c_str(), "cont") != NULL)
			{
				_continue = true;
			}
		}

		instructions[ROM_INSTRUCTION[IP]].instructionAction(&ROM_DATA[IP]);


		IP++;
	}



	return 0;
}