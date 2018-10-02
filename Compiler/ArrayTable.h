#ifndef ARRAYTABLE_H
#define ARRAYTABLE_H

#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "ErrorHandle.h"
using namespace std;

struct AEle{
    int type;
    int length;
};

class ArrayTable
{
    public:
        ArrayTable();
        static int addEle(int type, int length);
		static int getLen(int index);
		static int getType(int index);
		static void printall();
    protected:
    private:
        static struct AEle ATab[100];
        static int i;
};

#endif // ARRAYTABLE_H
