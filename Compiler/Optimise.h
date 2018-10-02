#ifndef OPTIMISE_H
#define OPTIMISE_H

#include "EleTable.h"
#include "STable.h"
#include "FourEleTable.h"
#include "FuncTable.h"
#include "ArrayTable.h"
#include "ErrorHandle.h"
#include <string>
#include <sstream>
#include<iostream>
#include<fstream>
#include <queue>
#include<stdlib.h>
using namespace std;

#define ENTRANCE_NUM 100
#define FETAB_NUM 500
#define NODE_NUM 100

class Optimise
{
public:
	Optimise(void);
	void beginOp();
private:
	void getFunBlock();
	void addEle(int fct,int a,int b,int c);
	void remEle(int index);
	void OpDag();
	void moveRefer(int ref, bool dir, bool cause);
	void peephole();
	void GloAllo();
	void OpOffset();
	struct FourEle newFETab[FETAB_NUM];
	int tabIndex;
	int n;
	int start;
	int entrance[ENTRANCE_NUM];
	bool funBlockLink[ENTRANCE_NUM][ENTRANCE_NUM];
	int oldIndex[FETAB_NUM];
	int newIndex[FETAB_NUM];
	int iindex;
};

#endif 
