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
    string id;//��ʶ����
    int type;//��ʶ�����ͣ�int��char��array��
    int kind;//��ʶ����𣨱�������������������
    int link;//ͬһ��������һ����ʶ���ķ��ű�ָ�루��һ����ʶ����0��
    bool isTemp;//�Ƿ�Ϊ��ʱ������������Ԫʽ���õ���
    bool isGlobal;//�Ƿ�Ϊȫ�ֱ���
    int refer;//�ñ�ʶ����������Ϣ�������Ϣ���е�λ�ã�һ�㳣���ͱ�����0��
    int paraNo;//������ţ��ǲ�����0��
	int offset;//��ʶ������Ӧ����������λ����Ի�ַ��ƫ�ƣ����ڳ������洢��ֵ���������飬�洢���һ���ƫ�ƣ����ں�������0������MIPS�ɸ���label��ת���ʲ���洢������ͷָ��λ�ã�
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
