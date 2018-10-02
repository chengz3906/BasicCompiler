#include "ArrayTable.h"

ofstream atout("arraytable.txt");
struct AEle ArrayTable::ATab[100];
int ArrayTable::i = 0;

ArrayTable::ArrayTable()
{
    //ctor
}

int ArrayTable::addEle(int type, int length){
    if(i >= 100){
		ErrorHandle::Fatal(1);
        exit(0);
    }
    ATab[i].type = type;
    ATab[i].length = length;
    i++;
    return i-1;
}

int ArrayTable::getLen(int index){
	return ATab[index].length;
}

void ArrayTable::printall(){
	for(int j = 0; j < i; ++j){
		atout<<j<<" "<<ATab[j].type<<" "<<ATab[j].length<<endl;
	}
}

int ArrayTable::getType(int index){
	return ATab[index].type;
}