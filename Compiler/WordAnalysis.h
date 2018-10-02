#ifndef WORDANALYSIS_H
#define WORDANALYSIS_H
#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "STable.h"
#include "ErrorHandle.h"
using namespace std;

class WordAnalysis
{
    public:
        WordAnalysis();
        int getsym();
        string getWord();
		string getLine();
		int getPline();
		bool getPCL();
        int getLineCount();
		void getch();
    protected:
    private:
        bool isalpha(char ch);
        bool isdigit(char ch);
        bool isstrmem(char ch);
        char ch;
        bool fileEnd;
        int pline, lline;
        int lineCount;
        string line;
        string word;
		string lastline;
		int lastpline;
        int sym;
        bool preCountLine;
};

#endif // WORDANALYSIS_H
