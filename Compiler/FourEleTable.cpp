#include "FourEleTable.h"
#include<fstream>
using namespace std;

struct FourEle FourEleTable::FETab[FENUM];
int FourEleTable::i = 0;
FourEleTable::FourEleTable()
{
    //ctor
}

int FourEleTable::addEle(int fct, int a, int b, int c){
    if(i >= FENUM){
        ErrorHandle::Fatal(5);
        exit(0);
    }
    FETab[i].fct = fct;
    FETab[i].a = a;
    FETab[i].b = b;
    FETab[i].c = c;
    i++;
    return i - 1;
}

void FourEleTable::moveRefer(int ref, bool dir, bool cause){
	if(dir){
		for(int j = 0; j < i; ++j){
			if((FETab[j].a >= ref && cause && FETab[j].fct != 13 && FETab[j].fct != 15 && FETab[j].fct != 17 && FETab[j].fct != 24 && FETab[j].fct != 19)
			|| (FETab[j].a >= ref && !cause && (FETab[j].fct == 13 || FETab[j].fct == 15 || FETab[j].fct == 24))){
				++FETab[j].a;
			}
			if(FETab[j].b >= ref && cause && FETab[j].fct != 14){
				++FETab[j].b;
			}
			if(FETab[j].c >= ref && cause && (0 <= FETab[j].fct && 5 >= FETab[j].fct || 21 == FETab[j].fct || 25 == FETab[j].fct || 26 == FETab[j].fct || 20 == FETab[j].fct)){
				++FETab[j].c;
			}
		}
	}
	else{
		for(int j = 0; j < i; ++j){
			if((FETab[j].a > ref && cause && FETab[j].fct != 13 && FETab[j].fct != 15 && FETab[j].fct != 17 && FETab[j].fct != 24 && FETab[j].fct != 19)
				|| (FETab[j].a > ref && !cause && (FETab[j].fct == 13 || FETab[j].fct == 15 || FETab[j].fct == 24))){
				--FETab[j].a;
			}
			if(FETab[j].b > ref && cause && FETab[j].fct != 14){
				--FETab[j].b;
			}
			if(FETab[j].c > ref && cause && (0 <= FETab[j].fct && 5 >= FETab[j].fct || 21 == FETab[j].fct || 25 == FETab[j].fct || 26 == FETab[j].fct || 20 == FETab[j].fct)){
				--FETab[j].c;
			}
		}
	}
}

int FourEleTable::getIndex(){
    return i;
}

void FourEleTable::flushAll(){
	i = 0;
}

int FourEleTable::getFct(int index){
	return FETab[index].fct;
}

int FourEleTable::geta(int index){
	return FETab[index].a;
}

int FourEleTable::getb(int index){
	return FETab[index].b;
}

int FourEleTable::getc(int index){
	return FETab[index].c;
}

void FourEleTable::setc(int index){
    FETab[index].c = i;
}

void FourEleTable::printall(){
    ofstream of("four.txt");
    for(int j = 0; j < i; ++j){
        of<<j<<" "<<FETab[j].fct<<" "<<FETab[j].a<<" "<<FETab[j].b<<" "<<FETab[j].c<<endl;/*
        if(EleTable::getRefer(FETab[j].a) != -1){
            of<<STable::getId(EleTable::getRefer(FETab[j].a))<<" ";
        }
        else{
            of<<EleTable::getNum(FETab[j].a)<<" ";
        }
        if(EleTable::getRefer(FETab[j].b) != -1){
            of<<STable::getId(EleTable::getRefer(FETab[j].b))<<" ";
        }
        else{
            of<<EleTable::getNum(FETab[j].b)<<" ";
        }
        if(EleTable::getRefer(FETab[j].c) != -1){
            of<<STable::getId(EleTable::getRefer(FETab[j].c))<<endl;
        }
        else{
            of<<EleTable::getNum(FETab[j].c)<<endl;
        }*/
    }
    of.close();
}
