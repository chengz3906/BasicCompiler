#ifndef STABLE_H
#define STABLE_H

#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "ErrorHandle.h"
using namespace std;

#define SYMBOL_NUM 10000

enum symbolNo{
	CASESY, CHARSY, CONSY, DEFSY, IFSY, INTSY, MAINSY, PRISY, RETSY, SCASY, SWISY, VOIDSY, WHISY, IDENT,
	INTEGER, NOTEQ, EQUAL, GRTEQ, GREATER, LESEQ, LESS, BECOME, CHAR, STRING, PLUS, MINUS, MULTI, DIVIDE, SEMICOLON, COMMA,
	LBRACK, RBRACK, LPARENT, RPARENT, LBRACE, RBRACE, COLON, VARIABLE, ARRAY, FUNCTION, NEWLINE, UNKNOWN
};

struct sTabEle{
    string id;//标识符名
    int type;//标识符类型（int、char、array）
    int kind;//标识符类别（变量、常量、函数名）
    int link;//同一函数内上一个标识符的符号表指针（第一个标识符填0）
    bool isTemp;//是否为临时变量（生成四元式需用到）
    bool isGlobal;//是否为全局变量
    int refer;//该标识符在数组信息表或函数信息表中的位置（一般常量和变量填0）
    int paraNo;//参数编号（非参数填0）
	int offset;//标识符在相应数据区所存位置相对基址的偏移，对于常量，存储其值；对于数组，存储其第一项的偏移；对于函数则填0（由于MIPS可根据label跳转，故不需存储函数开头指令位置）
	int useCount;
	int gloReg;
};

class STable
{
    public:
        STable();
        static int addEle(string id, int type, int kind, int link, bool isTemp, bool isGlobal, int refer, int paraNo, int offset);
		static void insertEle(int pos, string id, int type, int kind, int link, bool isTemp, bool isGlobal, int refer, int paraNo, int offset);
        static bool checkRe(string newId, int start, bool isFunc, bool outside);
        static string getId(int i);
        static int getType(int index);
        static int getKind(int index);
		static int getIndex();
		static int getOffset(int index);
		static void setOffsetp(int index);
		static void moveOffset(int index, int length);
		static int getParaNo(int index);
		static bool getIsGlo(int index);
		static bool getIsTemp(int index);
		static void deleteEle(int index);
		static void setIsTemp(int index, bool isTemp);
		static void addUseCount(int index, int n);
		static int getUseCount(int index);
		static void setGloReg(int index, int no);
		static int getGloReg(int index);
        static int locIndex(string id, int start, int tp);
        static int getVal(int index);
        static int getRef(int index);
        static void setLink(int index, int val);
        static void printall();
    protected:
    private:
        static struct sTabEle sTab[SYMBOL_NUM];
        static int i;
};

#endif // STABLE_H
