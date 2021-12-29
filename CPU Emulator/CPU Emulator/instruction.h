#pragma once
#include <functional>
#include <vector>

class instruction_input
{
public:
	instruction_input()
	{

	}

	instruction_input(uint32_t operand)
	{
		data.push_back(operand);
	}
	instruction_input(uint32_t operand_1, uint32_t operand_2)
	{
		data.push_back(operand_1);
		data.push_back(operand_2);
	}

	std::vector<uint32_t> data;
};

class Instruction
{
public:
	Instruction()
	{

	}

	std::function<void(instruction_input*)> instructionAction = [](instruction_input*)
	{
	};
};

