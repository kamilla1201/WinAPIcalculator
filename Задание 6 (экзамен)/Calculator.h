#include <string>
#define MAXSTR 30

using namespace std;
#pragma once
class Calculator
{
	double firstOperand;
	double secondOperand;
	int operation;
	double result;
	int numSystem;
	char * stringResult;
	double Memory;

public:
	Calculator();
	Calculator(char _stringResult[], int operationNumber, int _numSystem);
	void Calculate();
	double Calculator::doubleConverttoDec(char* in, int r);
	void setOperation(int opNumber);
	void setOperand(char operand[], int numsyst, int opNumber);
	void setMemory(char operand[], int numsyst, int opNumber);
	void setSystem(int system);
	char * doubleConvertfromDec(double dx, int r, char cx);
	char * getResult();
	char * getMemory();
	char* Calculator::low_upper(char* in);
	~Calculator();
};

