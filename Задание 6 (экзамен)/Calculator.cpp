#include "stdafx.h"
#include "Calculator.h"

Calculator::Calculator()
{
	double firstOperand = 0;
	double secondOperand = 0;
	int operation = NULL;
	double result = 0;
	int numSystem = 10;
	stringResult = new char[MAXSTR];
	stringResult = "0\0";
}
void Calculator::Calculate()
{
	switch (operation)
	{
	case 0:
		result = firstOperand + secondOperand;
		firstOperand = result;
		break;
	case 1:
		result = firstOperand - secondOperand;
		firstOperand = result;
		break;
	case 2:
		result = firstOperand * secondOperand;
		firstOperand = result;
		break;
	case 3:
		if (secondOperand == 0)
		{
			result = NULL;
			firstOperand = 0;
		}
		else
		{
			result = firstOperand / secondOperand;
			firstOperand = result;
		}
		break;
	default:
		break;
	}
	char sign = (result >= 0) ? '0' : '-';
	if (result == 0)
		stringResult = "Error operation!";
	else
		stringResult = doubleConvertfromDec(result, numSystem, sign);
}
void Calculator::setOperation(int opNumber)
{
	operation = opNumber;
}

char* Calculator::low_upper(char* in)
{
	for (int i = 0; in[i] != '\0'; i++)
		if (in[i] >= 'a' && in[i] <= 'z')
			in[i] = in[i] - 32;
	return in;
}

char * Calculator::getResult()
{
	return stringResult;
}
char * Calculator::getMemory()
{
	char sign = (Memory >= 0) ? '0' : '-';
	return doubleConvertfromDec(Memory, numSystem, sign);
}
void Calculator::setOperand(char operand[], int numsyst, int opNumber)
{
	numSystem = numsyst;
	if (numsyst > 10)
		strcpy(operand, low_upper(operand));
	if (opNumber == 1)
	{
		firstOperand = doubleConverttoDec(operand, numSystem);
	}
	else if(opNumber == 2)
	{
		secondOperand = doubleConverttoDec(operand, numSystem);
	}
	else if (opNumber == 0)
	{
		secondOperand = firstOperand;
	}
}

void Calculator::setMemory(char operand[], int numsyst, int opNumber)
{
	if (numsyst > 10)
		strcpy(operand, low_upper(operand));
	if (opNumber == 1)
		Memory = doubleConverttoDec(operand, numsyst);
	else if (opNumber == 2)
		Memory += doubleConverttoDec(operand, numsyst);
	else if (opNumber == 0)
		Memory = doubleConverttoDec("0", numsyst);
}
Calculator::~Calculator()
{
}

char* Calculator::doubleConvertfromDec(double dx, int r, char cx)
	{
		int i = 0, j, temp;
		double fx;
		char ctemp[65] = { "0\0" };
		char out[65] = { "0\0" };
		if (cx == '-')
			dx *= -1;
		long lx = static_cast<long>(dx);
		dx -= lx;	
		//                   calculation of integer part.
		//***************************************************************
		while (lx > 0)
		{
			temp = lx % r;
			ctemp[i] = (temp >= 0 && temp <= 9) ? temp + 48 : temp + 55;
			lx /= r;
			i++;
		}
		ctemp[i] = '\0';
		i--;
		j = 0;
		if (cx == '-')
		{
			out[j] = '-';
			j++;
		}
		if (i < 0)
		{
			out[j] = '0';
			j++;
		}
		for (; i >= 0; i--, j++)
			out[j] = ctemp[i];

		out[j] = '\0';
		//                   calculation of fraction part.
		//***************************************************************
		if (dx > 0.0)
		{
			out[j] = '.';
			j++;
			i = j;
			while (dx > 0.0000001 && j < i + 9)
			{
				fx = dx * r;
				temp = static_cast<int>(fx);
				ctemp[j] = ((temp >= 0 && temp <= 9) ? temp + 48 : temp + 55);
				dx = fx - temp;
				j++;
			}
			for (; i <= j; i++)
				out[i] = ctemp[i];
			out[i] = '\0';
		}
		return out;
	}

double Calculator::doubleConverttoDec(char* in, int r)
	{
		int i = 0, j;
		long lx = 0;
		double dx = 0.0;
		if (r != 10)
		{
			if (in[0] == '-')
				i++;
			while (in[i] != '\0' && in[i] != '.')
			{
				lx = lx * r + ((in[i] >= '0' && in[i] <= '9') ? in[i] - 48 : in[i] - 55);
				i++;
			}
			if (in[i] == '.')
			{
				j = i;
				while (in[i] != '\0')//count number of steps after decimal point
				{
					i++;
				}
				i--;
				double ir = (double)r;
				while (i > j)
				{
					dx = (dx + ((in[i] >= '0' && in[i] <= '9') ? in[i] - 48 : in[i] - 55)) * 1.0 / ir;
					i--;
				}
			}
			dx += lx;
		} else
			dx = atof(in);
		if (in[0] == '-')
			dx *= -1;
		return dx;
	}

void Calculator::setSystem(int system)
{
	numSystem = system;
}