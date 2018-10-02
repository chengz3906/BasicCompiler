#include "EleTable.h"

ofstream etout("etable.txt");
struct Ele EleTable::ETab[300];
int EleTable::i = 0;

EleTable::EleTable()
{
    //ctor
}

int EleTable::addEle(int type, bool isVar, int num, string str, int refer){
    int j;
    if(i >= 300){
        ErrorHandle::Fatal(4);
        exit(0);
    }
    for(j = 0; j < i; ++j){
        if(ETab[j].isVar == isVar && ((isVar && ETab[j].refer == refer)
               || (!isVar && ETab[j].num == num && ETab[j].str == str))){
            return j;
        }
    }
    ETab[i].type = type;
    ETab[i].isVar = isVar;
    ETab[i].num = num;
    ETab[i].str = str;
    ETab[i].refer = refer;
    i++;
    return i-1;
}

void EleTable::insertEle(int pos, int type, bool isVar, int num, string str, int refer){
	if(i >= 300){
		ErrorHandle::Fatal(4);
		exit(0);
	}
	for(int j = i-1; j >= pos; --j){
		ETab[j+1].type = ETab[j].type;
		ETab[j+1].isVar = ETab[j].isVar;
		ETab[j+1].num = ETab[j].num;
		ETab[j+1].str = ETab[j].str;
		ETab[j+1].refer = ETab[j].refer;
	}
	ETab[pos].type = type;
	ETab[pos].isVar = isVar;
	ETab[pos].num = num;
	ETab[pos].str = str;
	ETab[pos].refer = refer;
	++i;
}

int EleTable::getType(int index){
    return ETab[index].type;
}

int EleTable::getIndex(){
	return i;
}

int EleTable::getRefer(int index){
    return ETab[index].refer;
}

bool EleTable::getIsVar(int index){
    return ETab[index].isVar;
}

int EleTable::getNum(int index){
    return ETab[index].num;
}

string EleTable::getStr(int index){
	return ETab[index].str;
}


void EleTable::printall(){
	for(int j = 0; j < i; ++j){
		etout<<j<<" "<<ETab[j].type<<" "<<ETab[j].num<<" "<<ETab[j].refer<<endl;
	}
	etout.close();
}

void EleTable::moveRefer(int ref, bool dir){
	if(dir){
		for(int j = 0; j < i; ++j){
			if(ETab[j].refer >= ref){
				ETab[j].refer++;
			}
		}
	}
	else{
		for(int j = 0; j < i; ++j){
			if(ETab[j].refer > ref){
				ETab[j].refer--;
			}
		}
	}
}