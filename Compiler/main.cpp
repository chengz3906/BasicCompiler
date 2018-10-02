#include "GrammarAnaly.h"
#include "FourEleTable.h"
#include "EleTable.h"
#include "ArrayTable.h"
#include "FuncTable.h"
#include "WordAnalysis.h"
#include "GrammarAnaly.h"
#include "Optimise.h"
#include "GenMips.h"
using namespace std;


/*
bool term();
bool factor();
void statement();
bool funcall(string id);*/

int main(){
    WordAnalysis wa;
	Optimise op;
    int syms[2000];
    string words[2000];
    int lineCounts[2000];
	string lines[800];
	int plines[2000];
    int i = 0, j = 0;
    bool canend = false;
	int tempStack = 0;
	lines[j++] = wa.getLine();
	wa.getch();
    do{
		if(i >= 2000){
			ErrorHandle::Fatal(2);
		}
		plines[i] = wa.getPline();
		lineCounts[i] = wa.getLineCount();
        syms[i] = wa.getsym();
        words[i] = wa.getWord();
		if(wa.getPCL()){
			if(j >= 800){
				ErrorHandle::Fatal(3);
			}
			lines[j++] = wa.getLine();
		}
        if(i > 2 && words[i-2] == "main" && words[i-1] == "(" && words[i] == ")" && words[i-3] == "void"){
            canend = true;
        }
        if(syms[i] == LBRACE && canend){
            tempStack++;
        }
        else if(syms[i] == RBRACE && canend){
            tempStack--;
        }
        i++;
	}while (!canend || syms[i-1] != RBRACE || tempStack);/*
	int num = i;
	for(int i = 0; i < num; ++i){
		cout<<i<<' '<<syms[i]<<' '<<words[i]<<' '<<lineCounts[i]<<" "<<lines[lineCounts[i]]<<" "<<plines[i]<<endl;
	}
	i = num;*/
    GrammarAnaly ga(syms, words, lineCounts, lines, plines, i, j);
	ga.program();
	op.beginOp();
	if(ErrorHandle::getHaveError()){
		cout<<"Failed!"<<endl;
		cin>>i;
		return 0;
	}
	FourEleTable::printall();
	STable::printall();
	ArrayTable::printall();
	EleTable::printall();
	GenMips::GenCode();
	cout << "Completed"<<endl;
	cin>>i;
	return 0;
}
