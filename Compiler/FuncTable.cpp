#include "FuncTable.h"

struct FEle FuncTable::FTab[50];
int FuncTable::i;
FuncTable::FuncTable()
{
    //ctor
}

int FuncTable::addEle(bool isVoid){
	if(i >= 50){
		ErrorHandle::Fatal(6);
        exit(0);
    }
    FTab[i].isVoid = isVoid;
    i++;
    return i - 1;
}

int FuncTable::getParaNo(int index){
    return FTab[index].paraNo;
}

int FuncTable::getIsVoid(int index){
    return FTab[index].isVoid;
}

int FuncTable::getLen(int index){
	return FTab[index].length;
}

bool FuncTable::getIsLeaf(int index){
	return FTab[index].isLeaf;
}

void FuncTable::setParaNo(int index, int val){
    FTab[index].paraNo = val;
}

void FuncTable::setLen(int index, int val){
    FTab[index].length = val;
}

void FuncTable::setIsLeaf(int index, bool val){
    FTab[index].isLeaf = val;
}

void FuncTable::setSRnum(int index, int val){
    FTab[index].srNum = val;
}

int FuncTable::getSRnum(int index){
	return FTab[index].srNum;
}