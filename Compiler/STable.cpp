#include "STable.h"
#include<fstream>
using namespace std;

struct sTabEle STable::sTab[SYMBOL_NUM];
int STable::i = 0;
STable::STable()
{
    //ctor
}
int STable::addEle(string id, int type, int kind, int link, bool isTemp, bool isGlobal, int refer, int paraNo, int offset)
{
	if(i >= SYMBOL_NUM){
		ErrorHandle::Fatal(8);
        exit(0);
    }
    sTab[i].id = id;
    sTab[i].type = type;
    sTab[i].kind = kind;
    sTab[i].link = link;
    sTab[i].isTemp = isTemp;
    sTab[i].isGlobal = isGlobal;
    sTab[i].refer = refer;
    sTab[i].paraNo = paraNo;
	sTab[i].offset = offset;
	sTab[i].useCount = 0;
	sTab[i].gloReg = -1;
    i++;
    return i - 1;
}

void STable::insertEle(int pos, string id, int type, int kind, int link, bool isTemp, bool isGlobal, int refer, int paraNo, int offset)
{
	if(i >= SYMBOL_NUM){
		ErrorHandle::Fatal(8);
		exit(0);
	}
	for(int j = i-1; j >= pos; --j){
		sTab[j+1].id = sTab[j].id;
		sTab[j+1].type = sTab[j].type;
		sTab[j+1].kind = sTab[j].kind;
		sTab[j+1].link = sTab[j].link;
		sTab[j+1].isTemp = sTab[j].isTemp;
		sTab[j+1].isGlobal = sTab[j].isGlobal;
		sTab[j+1].refer = sTab[j].refer;
		sTab[j+1].paraNo = sTab[j].paraNo;
		sTab[j+1].useCount = sTab[j].useCount;
		sTab[j+1].gloReg = sTab[j].gloReg;
		if(sTab[j].offset >= 0){
			sTab[j+1].offset = sTab[j].offset+1;
		}
		else{
			sTab[j+1].offset = sTab[j].offset;
		}
	}
	sTab[pos].id = id;
	sTab[pos].type = type;
	sTab[pos].kind = kind;
	sTab[pos].link = link;
	sTab[pos].isTemp = isTemp;
	sTab[pos].isGlobal = isGlobal;
	sTab[pos].refer = refer;
	sTab[pos].paraNo = paraNo;
	sTab[pos].offset = offset;
	sTab[pos].useCount = 0;
	sTab[pos].gloReg = -1;
	i++;
}

string STable::getId(int i){
    return sTab[i].id;
}

int STable::getIndex(){
	return i;
}

int STable::getOffset(int index){
	return sTab[index].offset;
}

int STable::getParaNo(int index){
	return sTab[index].paraNo;
}

bool STable::getIsGlo(int index){
	return sTab[index].isGlobal;
}

bool STable::checkRe(string newId, int start, bool isFunc, bool outside){
    for(int i = start; i != -1; i = sTab[i].link){
		if(outside && newId == sTab[i].id){
			return false;
		}
		if(!outside && newId == sTab[i].id && !sTab[i].isTemp && sTab[i].kind != FUNCTION && !sTab[i].isGlobal){
            return false;
        }
    }
    return true;
}

int STable::getType(int index){
    return sTab[index].type;
};

int STable::getKind(int index){
    return sTab[index].kind;
};

bool STable::getIsTemp(int index){
	return sTab[index].isTemp;
}

void  STable::setOffsetp(int index){
	if(sTab[index].kind == VARIABLE){
		sTab[index].offset++;
	}
}

void STable::moveOffset(int index, int length){
	sTab[index].offset += length;
}

int STable::locIndex(string id, int start, int tp){
    for(int i = start; i != -1; i = sTab[i].link){
        if(id == sTab[i].id && !sTab[i].isTemp){
			if(1 == tp && sTab[i].type != INTSY && sTab[i].type != CHARSY ||
				2 == tp && sTab[i].kind != FUNCTION ||
				3 == tp && sTab[i].type != ARRAY){
					return -1;
			}
            return i;
        }
    }
    return -1;
}
int STable::getVal(int index){
    return sTab[index].offset;
}

int STable::getRef(int index){
    return sTab[index].refer;
}

int STable::getGloReg(int index){
	return sTab[index].gloReg;
};

void STable::deleteEle(int index){
	for(int j = index+1; j < i; ++j){
		if(sTab[j].kind == FUNCTION){
			return;
		}
		sTab[j].offset--;
	}
}

void STable::setLink(int index, int val){
    sTab[index].link = val;
}

void STable::addUseCount(int index, int n){
	sTab[index].useCount += n;
}

int STable::getUseCount(int index){
	return sTab[index].useCount;
}

void STable::setGloReg(int index, int no){
	sTab[index].gloReg = no;
}

void STable::setIsTemp(int index, bool isTemp){
	sTab[index].isTemp = isTemp;
}

void STable::printall(){
    ofstream of("tab.txt");
    for(int j = 0; j < i; ++j){
        of<<j<<" "<<sTab[j].id<<" "<<sTab[j].type<<" "<<sTab[j].kind<<" "<<sTab[j].link<<" "<<sTab[j].offset<<" "<<sTab[j].refer<<" "<<sTab[j].paraNo<<endl;
    }
    of.close();
}
