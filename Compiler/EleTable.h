#ifndef ELETABLE_H
#define ELETABLE_H

#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "ErrorHandle.h"
using namespace std;

struct Ele{
    int type;
    bool isVar;
    int num;
    string str;
    int refer;
};

class EleTable
{
    public:
        EleTable();
        static int addEle(int type, bool isVar, int num, string str, int refer);
		static void insertEle(int pos, int type, bool isVar, int num, string str, int refer);
		static void moveRefer(int ref, bool dir);
        static int getRefer(int index);
        static bool getIsVar(int index);
        static int getNum(int index);
        static int getType(int index);
		static int getIndex();
		static string getStr(int index);
		static void printall();
    protected:
    private:
        static struct Ele ETab[300];
        static int i;
};

#endif // ELETABLE_H
