#ifndef GRAMMARANALY_H
#define GRAMMARANALY_H

#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "WordAnalysis.h"
using namespace std;

class GrammarAnaly
{
    public:
        GrammarAnaly(int *symss, string *wordss, int *lineCountss, string *liness, int *pliness, int syNum, int lNum);
        void program();
    protected:
    private:
        int syms[2000];
        string words[2000];
        int lineCounts[2000];
		string lines[800];
		int plines[2000];
        int sym;
        string word;
        int lineCount;
		string line;
		int pline;
        int symNum;
        int symIndex;
        int braceStack;
        int tabIndex;
        int lastTabIndex;
        int offset;
        int tempVarNum;
        int integerVal;
        bool isLeaf;
        int returnType;
		int currentFunc;
        int hasRet;
        void getsym();
        void readToNextSentence();
        bool readToTarget(int *tar, int n);
        void skipUselessWords(int *tar, int n);
        bool integerDet();
        void constDecl(bool outside);
        int varDecl(bool outside);
        int expression();
        int term();
        int factor();
        void conditionSta();
        void loopSta();
        void statementLine();
        int funcall(int tabRef);
        void becomeSta(string word, int temppline);
        void readSta();
        void writeSta();
        void returnSta();
        void caseSta();
        void statement();
        void compoundSta(bool notMain);
        bool funcDecl(bool skipped, int tabRef);
        void mainDecla();
};

#endif // GRAMMARANALY_H
