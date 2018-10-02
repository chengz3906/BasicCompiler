#ifndef FOURELETABLE_H
#define FOURELETABLE_H

#define FENUM 500
#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include"EleTable.h"
#include"STable.h"
using namespace std;

struct FourEle{
    int fct;
    int a;
    int b;
    int c;
};

class FourEleTable
{
    public:
        FourEleTable();
        static int addEle(int fct, int a, int b, int c);
		static void moveRefer(int ref, bool dir, bool cause);
        static int getIndex();
		static void flushAll();
		static int getFct(int index);
		static int geta(int index);
		static int getb(int index);
		static int getc(int index);
        static void setc(int index);
        static void printall();
    protected:
    private:
        static struct FourEle FETab[FENUM];
        static int i;
};

#endif // FOURELETABLE_H
