#ifndef GENMIPS_H
#define GENMIPS_H

#include "FourEleTable.h"
#include "FuncTable.h"
#include "ArrayTable.h"
#include <fstream>
using namespace std;
class GenMips
{
public:
	GenMips(void);
	static void GenCode();
	static void GenLine();
	static bool Loada();
	static bool Loadb();
	static bool Loadc();
private:
	static int fourNum;
	static int index;
	static int fct;
	static int a;
	static int b;
	static int c;
	static int aval;
	static int bval;
	static int areg;
	static int breg;
	static int creg;
	static int labNo;
	static int bnum;
	static int soffset;
	static int strNum;
	static int tempReg[10];
	static string strs[100];
	static int labels[100];
	static bool labelMark[100];
};

#endif // GENMIPS_H
