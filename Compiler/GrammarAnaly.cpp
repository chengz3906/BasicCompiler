#include "GrammarAnaly.h"
#include "FourEleTable.h"
#include "EleTable.h"
#include "ArrayTable.h"
#include "FuncTable.h"
#include<sstream>
using namespace std;

int staHeadSym[10] = { IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON };
GrammarAnaly::GrammarAnaly(int *symss, string *wordss, int *lineCountss, string *liness, int *pliness, int syNum, int lNum)
{
    for(int i = 0; i < syNum; ++i){
        syms[i] = symss[i];
        words[i] = wordss[i];
        lineCounts[i] = lineCountss[i];
		plines[i] = pliness[i];
        //cout<<syms[i]<<" "<<words[i]<<" "<<lineCounts[i]<<endl;
    }
	for(int i = 0; i < lNum; ++i){
		lines[i] = liness[i];
	}
    symNum = syNum;
    symIndex = 0;
	braceStack = 0;
	tabIndex = 0;
	lastTabIndex = -1;
	offset = 1;
	tempVarNum = 0;
}

void GrammarAnaly::getsym(){
    if(symIndex >= symNum){
        cout<<"Incomplete code!"<<endl;
		cin>>sym;
        exit(0);
    }
    sym = syms[symIndex];
    word = words[symIndex];
    lineCount = lineCounts[symIndex];
	line = lines[lineCount];
	pline = plines[symIndex];
    symIndex++;
}

void GrammarAnaly::readToNextSentence(){
	while (sym != SEMICOLON && sym != NEWLINE){
		getsym();
	}
	if(SEMICOLON == sym){
		getsym();
	}
	while (NEWLINE == sym){
		getsym();
	}
}

bool GrammarAnaly::readToTarget(int *tar, int n){
	int lineBefore = lineCount;
	while (lineCount == lineBefore){
		for (int i = 0; i < n; ++i){
			if (tar[i] == sym){
				return false;
			}
		}
		getsym();
	}
	return true;
}

void GrammarAnaly::skipUselessWords(int *tar, int n){
	int skipCount = 0;
	while (1){
		for (int i = 0; i < n; ++i){
			if (tar[i] == sym){
				//cout << "skipped " << skipCount << " useless words" << endl;
				return;
			}
		}
		skipCount++;
		getsym();
	}
}

bool GrammarAnaly::integerDet(){
	if (PLUS == sym || MINUS == sym){
        int tempSym = sym;
		getsym();
		if (INTEGER != sym){
			ErrorHandle::Error(6,lineCount,line,pline);
			return true;
		}
		else{
            stringstream ss;
            ss<<word;
            ss>>integerVal;
            if(tempSym == MINUS){
                integerVal = 0-integerVal;
            }
			getsym();
			return false;
		}
	}
	else if (INTEGER != sym){
		ErrorHandle::Error(6,lineCount,line,pline);
		return true;
	}
	else{
        stringstream ss;
        ss<<word;
        ss>>integerVal;
		getsym();
		return false;
	}
}

void GrammarAnaly::constDecl(bool outside){
    string tempId;
    if(sym != CONSY){
        if(outside){
            int target[4] = {INTSY, CHARSY, CONSY, VOIDSY};
            skipUselessWords(target, 4);
        }
        else{
            int target[14] = {IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, CONSY, VOIDSY};
            skipUselessWords(target, 14);
        }
    }
	while (sym == CONSY){
		bool err = false;
		bool isInt = false;
		getsym();
		if (sym == INTSY){
			isInt = true;
		}
		else if (sym != CHARSY){
			ErrorHandle::Error(7,lineCount,line,pline);
			err = true;
			//cout << "a failed const declaration" << endl;
			readToNextSentence();
			continue;
		}
		getsym();
		while (1){
			if (sym != IDENT){
				ErrorHandle::Error(8,lineCount,line,pline);
				err = true;
				int tar[2] = { SEMICOLON, COMMA };
				if (readToTarget(tar, 2)){
					ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
					break;
				}
			}
			else if(!STable::checkRe(word,tabIndex-1,false,outside)){
				ErrorHandle::Error(11,lineCount,line,pline);
				err = true;
				int tar[2] = { SEMICOLON, COMMA };
				if (readToTarget(tar, 2)){
					ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
					break;
				}
			}
			else{
				tempId = word;
				getsym();
				if (sym != BECOME){
					ErrorHandle::Error(9,lineCount,line,pline);
					err = true;
					int tar[2] = { SEMICOLON, COMMA };
					if (readToTarget(tar, 2)){
						ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
						break;
					}
				}
				else {
					getsym();
					if (!isInt){
						if (CHAR != sym){
							ErrorHandle::Error(10,lineCount,line,pline);
							err = true;
							int tar[2] = { SEMICOLON, COMMA };
							if (readToTarget(tar, 2)){
								ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
								break;
							}
						}
						else{
							STable::addEle(tempId,CHARSY,CONSY,tabIndex-1,false,outside,0,0,(int)word[1]);
							tabIndex++;
							getsym();
						}
					}
					else if (integerDet()){
						err = true;
						int tar[2] = { SEMICOLON, COMMA };
						if (readToTarget(tar, 2)){
							ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
							break;
						}
					}
					else{
						STable::addEle(tempId,INTSY,CONSY,tabIndex-1,false,outside,0,0,integerVal);
						tabIndex++;
					}
				}
			}
			if (sym == COMMA){
				getsym();
				continue;
			}
			else if (sym == SEMICOLON){
				getsym();
				if (isInt){
					//cout << "A const int declaration";
				}
				else{
					//cout << "A const char declaration";
				}
				if (err){
					//cout << " with error";
				}
				//cout << endl;
				break;
			}
			else {
				ErrorHandle::Error(12,lineCount,line,pline);
				cout << "a const declaration without semicolon" << endl;
				break;
			}
		}
        if(outside){
            int target[4] = {INTSY, CHARSY, CONSY, VOIDSY};
            skipUselessWords(target, 4);
        }
        else{
            int target[14] = {IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, CONSY, VOIDSY};
            skipUselessWords(target, 14);
        }
	}
}

int GrammarAnaly::varDecl(bool outside){
    string tempId;
    if(CHARSY != sym && INTSY != sym && VOIDSY != sym){
        if(outside){
            int target[3] = {INTSY, CHARSY, VOIDSY};
            skipUselessWords(target, 3);
        }
        else{
            int target[13] = {IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, VOIDSY};
            skipUselessWords(target, 13);
        }
    }
	while (CHARSY == sym || INTSY == sym){
		bool isInt = CHARSY == sym ? false : true;
		int tempSym = sym;
		bool err = false;
		getsym();
		while (1){
			if (IDENT != sym){
				ErrorHandle::Error(8,lineCount,line,pline);
				err = true;
				int tar[2] = { SEMICOLON, COMMA };
				if (readToTarget(tar, 2)){
					ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
					break;
				}
			}
			else{
                tempId = word;
				getsym();
				if(!STable::checkRe(tempId,tabIndex-1,true,outside)){
					ErrorHandle::Error(11,lineCount,line,pline-1);
					if(LPARENT != sym){
						err = true;
						int tar[2] = { SEMICOLON, COMMA };
						if (readToTarget(tar, 2)){
							ErrorHandle::Error(12,lineCount-1,lines[lineCount-1],lines[lineCount-1].size());
							break;
						}
					}
				}
			}
			if (LPARENT == sym){
				if(!outside){
					ErrorHandle::Error(13,lineCount,line,pline);
					break;
				}
				if(STable::checkRe(tempId,tabIndex-1,true,true)){
					int funcRef = FuncTable::addEle(false);
					int tabRef = STable::addEle(tempId,tempSym,FUNCTION,tabIndex-1,false,true,funcRef,-1,-1);
					lastTabIndex = tabIndex;
					tabIndex++;
					return tabRef;
				}
				else{
					int tar[3] = { VOIDSY, INTSY, CHARSY };
					//cout << "a failed function declaration. will skip useless function body" << endl;
					skipUselessWords(tar, 3);
					return -1;
				}
			}
			else if (LBRACK == sym){
				getsym();
				if (INTEGER != sym){
					ErrorHandle::Error(6,lineCount,line,pline);
					err = true;
					int tar[2] = { SEMICOLON, COMMA };
					if (readToTarget(tar, 2)){
						//cout << "a failed variable declaration" << endl;
						break;
					}
				}
				else{
					integerDet();
					if (RBRACK != sym){
						ErrorHandle::Error(14,lineCount,line,pline);
						err = true;
						int tar[2] = { SEMICOLON, COMMA };
						if (readToTarget(tar, 2)){
							//cout << "a failed variable declaration" << endl;
							break;
						}
					}
					else{
						int tempPointer = ArrayTable::addEle(tempSym,integerVal);
						STable::addEle(tempId,ARRAY,VARIABLE,tabIndex-1,false,outside,tempPointer,0,offset+1);
						tabIndex++;
						offset += integerVal;
						getsym();
					}
				}
			}
			else{
				STable::addEle(tempId,tempSym,VARIABLE,tabIndex-1,false,outside,0,0,++offset);
				tabIndex++;
			}
			if (COMMA == sym){
				getsym();
				continue;
			}
			else if (SEMICOLON == sym){
				getsym();
				if (isInt){
					//cout << "A variable int declaration";
				}
				else{
					//cout << "A variable char declaration";
				}
				if (err){
					//cout << " with error";
				}
				//cout << endl;
				break;
			}
			else{
				ErrorHandle::Error(12,lineCount,line,pline);
				//cout << "a variable declaration without semicolon" << endl;
				readToNextSentence();
				break;
			}
		}
		if(outside){
			int target[4] = {CONSY, INTSY, CHARSY, VOIDSY};
			skipUselessWords(target, 4);
			if(sym == CONSY){
				ErrorHandle::Error(7,lineCount,line,pline);
				int targe[3] = {INTSY, CHARSY, VOIDSY};
				skipUselessWords(targe, 3);
			}
        }
		else{
			int target[14] = {CONSY, IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, VOIDSY};
			skipUselessWords(target, 14);
			if(sym == CONSY){
				ErrorHandle::Error(7,lineCount,line,pline);
				int targe[13] = {IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, VOIDSY};
				skipUselessWords(targe, 13);
			}
        }
	}
	return -1;
}

int GrammarAnaly::expression(){
    int isPlus = true;
    int termRef;
    int tempRef;
	int tempTyp;
    int returnRef;
    string tempName;
	if (PLUS == sym || MINUS == sym){
        if(MINUS == sym){
            isPlus = false;
        }
		getsym();
	}
    termRef = term();
    if(termRef == -1){
        return -1;
    }
    if(!isPlus){
		tempTyp = EleTable::getType(termRef);
        if(!EleTable::getIsVar(termRef)){
            int tempVal = -EleTable::getNum(termRef);
            returnRef = EleTable::addEle(tempTyp,false,tempVal,"",-1);
        }
        else{
            STable::addEle("t",INTSY,VARIABLE,tabIndex-1,true,false,0,0,++offset);
            returnRef = EleTable::addEle(tempTyp,true,0,"",tabIndex);
			tabIndex++;
			FourEleTable::addEle(5,termRef,-1,returnRef);
        }
    }
    else{
        returnRef = termRef;
    }
	while (1){
		if (PLUS != sym && MINUS != sym){
			return returnRef;
		}
		else{
            isPlus = PLUS == sym ? true : false;
			getsym();
		}
		termRef = term();
		if(termRef == -1){
            return -1;
		}
		tempRef = returnRef;
		tempTyp = (EleTable::getType(tempRef) == CHARSY || EleTable::getType(tempRef) == CHARSY) ? CHARSY : INTSY;
        if(isPlus){
            if(!EleTable::getIsVar(termRef) && !EleTable::getIsVar(tempRef)){
                int tempVal = EleTable::getNum(termRef)+EleTable::getNum(tempRef);
                returnRef = EleTable::addEle(tempTyp,false,tempVal,"",-1);
            }
            else{
                STable::addEle("t",INTSY,VARIABLE,tabIndex-1,true,false,0,0,++offset);
                returnRef = EleTable::addEle(tempTyp,true,0,"",tabIndex);
				tabIndex++;
				FourEleTable::addEle(0,tempRef,termRef,returnRef);
            }
        }
        else{
            if(!EleTable::getIsVar(termRef) && !EleTable::getIsVar(tempRef)){
                int tempVal = EleTable::getNum(tempRef)-EleTable::getNum(termRef);
                returnRef = EleTable::addEle(tempTyp,false,tempVal,"",-1);
            }
            else{
                STable::addEle("t",INTSY,VARIABLE,tabIndex-1,true,false,0,0,++offset);
                returnRef = EleTable::addEle(tempTyp,true,0,"",tabIndex);
				tabIndex++;
				FourEleTable::addEle(1,tempRef,termRef,returnRef);
            }
        }
	}
}

int GrammarAnaly::term(){
    int factorRef;
    int returnRef;
    int tempRef;
    int tempSym;
	int tempTyp;
    string tempName;
    factorRef = factor();
    if(factorRef == -1){
        return -1;
    }
    returnRef = factorRef;
	while (1){
		if (MULTI != sym && DIVIDE != sym){
			return returnRef;
		}
		else{
            tempSym = sym;
			getsym();
		}
		factorRef = factor();
		if(factorRef == -1){
            return -1;
		}
		tempRef = returnRef;
		tempTyp = (EleTable::getType(factorRef) == CHARSY || EleTable::getType(tempRef) == CHARSY) ? CHARSY : INTSY;
        if(tempSym == MULTI){
            if(!EleTable::getIsVar(factorRef) && !EleTable::getIsVar(tempRef)){
                int tempVal = EleTable::getNum(factorRef)*EleTable::getNum(tempRef);
                returnRef = EleTable::addEle(tempTyp,false,tempVal,"",-1);
            }
            else{
                STable::addEle("t",INTSY,VARIABLE,tabIndex-1,true,false,0,0,++offset);
				returnRef = EleTable::addEle(tempTyp,true,0,"",tabIndex);
				tabIndex++;
				FourEleTable::addEle(2,tempRef,factorRef,returnRef);
            }
        }
        else {
            if(!EleTable::getIsVar(factorRef) && !EleTable::getIsVar(tempRef)){
				if(!EleTable::getNum(factorRef)){
					ErrorHandle::Error(15,lineCount,line,pline);
					return -1;
				}
                int tempVal = EleTable::getNum(tempRef)/EleTable::getNum(factorRef);
				returnRef = EleTable::addEle(tempTyp,false,tempVal,"",-1);
            }
            else{
                STable::addEle("t",INTSY,VARIABLE,tabIndex-1,true,false,0,0,++offset);
                returnRef = EleTable::addEle(tempTyp,true,0,"",tabIndex);
				tabIndex++;
				FourEleTable::addEle(3,tempRef,factorRef,returnRef);
            }
        }
	}
}

int GrammarAnaly::factor(){
    int tabRef;
    int returnRef;
	int temppline = pline;
	if (IDENT == sym){
		int temppline = pline;
		string id = word;
		getsym();
		if (LBRACK == sym){
			getsym();
			int offsetRef = expression();
			if(offsetRef == -1){
                return -1;
			}
			else if (RBRACK != sym){
				ErrorHandle::Error(14,lineCount,line,pline);
				return -1;
			}
			else{
                tabRef = STable::locIndex(id,tabIndex-1,3);
                if(tabRef == -1){
					ErrorHandle::Error(21,lineCount,line,temppline);
                    return -1;
				}
				int tempRef = STable::addEle("t",ArrayTable::getType(STable::getRef(tabRef)),VARIABLE,tabIndex-1,true,false,-1,-1,++offset);
				tabIndex++;
                returnRef = EleTable::addEle(ArrayTable::getType(STable::getRef(tabRef)),true,0,"",tempRef);
				int tabEleRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabRef);
				FourEleTable::addEle(25,tabEleRef,offsetRef,returnRef);
				getsym();
				return returnRef;
			}
		}
		else if (LPARENT == sym){
		    tabRef = STable::locIndex(id,lastTabIndex,2);
			if(tabRef == -1){
				ErrorHandle::Error(21,lineCount,line,temppline);
                return -1;
		    }
		    int funcRef = STable::getRef(tabRef);
			if(FuncTable::getIsVoid(funcRef)){
				ErrorHandle::Error(30,lineCount,line,pline);
                return -1;
            }
			returnRef = funcall(tabRef);
			if(returnRef < 0){
                return -1;
			}
			else{
                return returnRef;
			}
		}
		else{
            tabRef = STable::locIndex(id,tabIndex-1,1);
			if(tabRef == -1){
				ErrorHandle::Error(21,lineCount,line,pline);
                return -1;
            }
            int val;
            if(STable::getKind(tabRef) == VARIABLE){
                returnRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabRef);
            }
            else if(STable::getKind(tabRef) == CONSY){
                val = STable::getVal(tabRef);
                returnRef = EleTable::addEle(STable::getType(tabRef),false,val,"",tabRef);
            }
			else{
				ErrorHandle::Error(21,lineCount,line,pline);
				return -1;
			}
			return returnRef;
		}
	}
	else if (LPARENT == sym){
		getsym();
		returnRef = expression();
		if(returnRef == -1){
            return -1;
		}
		else if (RPARENT != sym){
			ErrorHandle::Error(16,lineCount,line,pline);
			return -1;
		}
		else{
			getsym();
			return returnRef;
		}
	}
	else if (PLUS == sym || MINUS == sym || INTEGER == sym){
		if (integerDet()){
			return -1;
		}
		else{
            returnRef = EleTable::addEle(INTSY,false,integerVal,"",-1);
			return returnRef;
		}
	}
	else if (CHAR == sym){
        returnRef = EleTable::addEle(CHARSY,false,(int)word[1],"",-1);
		getsym();
		return returnRef;
	}
	else{
		ErrorHandle::Error(17,lineCount,line,pline);
		return -1;
	}
}

void GrammarAnaly::conditionSta(){
    int eleRef1;
    int eleRef2;
    int fourEleRef;
    bool flag = true;
	getsym();
	if (LPARENT != sym){
		ErrorHandle::Error(18,lineCount,line,pline);
		//cout << "a failed condition statement without left parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else getsym();
	eleRef1 = expression();
	if (eleRef1 == -1){
		//cout << "a failed condition statement with expression errors" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else if (LESS == sym || LESEQ == sym || GREATER == sym || GRTEQ == sym || EQUAL == sym || NOTEQ == sym){
        int tempSym = sym;
		getsym();
        eleRef2 = expression();
        if (eleRef2 == -1){
			//cout << "a failed condition statement with expression errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		STable::setIsTemp(EleTable::getRefer(eleRef1), false);
		STable::setIsTemp(EleTable::getRefer(eleRef2), false);
		fourEleRef = FourEleTable::addEle(tempSym - 8,eleRef1,eleRef2,-1);
		flag = false;
	}
	if (RPARENT != sym){
		ErrorHandle::Error(16,lineCount,line,pline);
		//cout << "a failed condition statement without right parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		getsym();
		if(flag){
			STable::setIsTemp(EleTable::getRefer(eleRef1), false);
            eleRef2 = EleTable::addEle(INTSY,false,0,"",-1);
            fourEleRef = FourEleTable::addEle(7,eleRef1,eleRef2,-1);
		}
		//cout << "an condition statement" << endl;
		while(NEWLINE == sym){
			getsym();
		}
		statement();
		while(NEWLINE == sym){
			getsym();
		}
        FourEleTable::setc(fourEleRef);
	}
}

void GrammarAnaly::loopSta(){
    int eleRef1;
    int eleRef2;
    int fourEleRef;
	int curPos = FourEleTable::getIndex();
    bool flag = true;
	getsym();
	if (LPARENT != sym){
		ErrorHandle::Error(18,lineCount,line,pline);
		//cout << "a failed loop statement without left parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else getsym();
	eleRef1 = expression();
	if (eleRef1 == -1){
		//cout << "a failed loop statement with expression errors" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else if (LESS == sym || LESEQ == sym || GREATER == sym || GRTEQ == sym || EQUAL == sym || NOTEQ == sym){
        int tempSym = sym;
		getsym();
        eleRef2 = expression();
        if (eleRef2 == -1){
			//cout << "a failed condition statement with expression errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		STable::setIsTemp(EleTable::getRefer(eleRef1), false);
		STable::setIsTemp(EleTable::getRefer(eleRef2), false);
		fourEleRef = FourEleTable::addEle(tempSym - 8,eleRef1,eleRef2,-1);
		flag = false;
	}
	if (RPARENT != sym){
		ErrorHandle::Error(16,lineCount,line,pline);
		//cout << "a failed loop statement without right parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		getsym();
		if(flag){
			STable::setIsTemp(EleTable::getRefer(eleRef1), false);
            eleRef2 = EleTable::addEle(INTSY,false,0,"",-1);
            fourEleRef = FourEleTable::addEle(7,eleRef1,eleRef2,-1);
		}
		//cout << "a loop statement" << endl;
		while(NEWLINE == sym){
			getsym();
		}
		statement();
		while(NEWLINE == sym){
			getsym();
		}
		FourEleTable::addEle(6,0,0,curPos);
        FourEleTable::setc(fourEleRef);
	}
}

void GrammarAnaly::statementLine(){
	getsym();
	while (RBRACE != sym){
		statement();
	}
	braceStack--;
	getsym();
}

int GrammarAnaly::funcall(int tabRef){
	int EleRef;
	int paraNo;
    string tempName;
    isLeaf = false;
	getsym();
	int funcRef = STable::getRef(tabRef);
	paraNo = FuncTable::getParaNo(funcRef);
	int paraRef[20];
	FourEleTable::addEle(13,paraNo,0,0);
	if (RPARENT == sym){
        if(paraNo != 0){
			ErrorHandle::Error(33,lineCount,line,pline);
            return -1;
        }
        EleRef = EleTable::addEle(FUNCTION,true,0,"",tabRef);
        FourEleTable::addEle(15,EleRef,0,0);
		getsym();
		if(FuncTable::getIsVoid(funcRef)){
            return -2;
		}
		else{
            STable::addEle("t",STable::getType(tabRef),VARIABLE,tabIndex-1,true,false,-1,-1,++offset);
            EleRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabIndex);
            FourEleTable::addEle(20,0,0,EleRef);
            tabIndex++;
            return EleRef;
		}
	}
    int tempRef;
	int i = 1;
	while (1){
        if(i > paraNo){
			ErrorHandle::Error(20,lineCount,line,pline);
            return -1;
        }
        tempRef = expression();
        if(tempRef == -1){
            return -1;
		}
		STable::setIsTemp(EleTable::getRefer(tempRef), false);
		paraRef[i-1] = tempRef;
        i++;
		if (COMMA == sym){
			getsym();
			continue;
		}
		else if (RPARENT != sym){
			if(i == paraNo + 1){
				ErrorHandle::Error(16,lineCount,line,pline);
			}
			else{
				ErrorHandle::Error(19,lineCount,line,pline);
			}
			return -1;
		}
		else {
		    if(i == paraNo + 1){
				for(int j = 0; j < paraNo; ++j){
					FourEleTable::addEle(14,paraRef[j],j+1,0);
				}
                EleRef = EleTable::addEle(FUNCTION,true,0,"",tabRef);
                FourEleTable::addEle(15,EleRef,0,0);
                getsym();
                if(FuncTable::getIsVoid(funcRef)){
                    return -2;
                }
                else{
                    STable::addEle("t",STable::getType(tabRef),VARIABLE,tabIndex-1,true,false,-1,-1,++offset);
                    EleRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabIndex);
                    FourEleTable::addEle(20,0,0,EleRef);
                    tabIndex++;
                    return EleRef;
                }
		    }
			else{
				ErrorHandle::Error(19,lineCount,line,pline);
                getsym();
                return -1;
		    }
		}
	}
}

void GrammarAnaly::becomeSta(string word, int temppline){
    int eleRef1;
    int eleRef2;
    int tabRef;
    int offsetRef;
    bool flag = false;
	if (LBRACK == sym){
		getsym();
		offsetRef = expression();
		if(offsetRef == -1){
			//cout << "a failed become statement with expression errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		if (RBRACK != sym){
			ErrorHandle::Error(14,lineCount,line,pline);
			//cout << "a failed become statement without right bracket" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
            flag = true;
            getsym();
		}
	}
	if (BECOME != sym){
		ErrorHandle::Error(9,lineCount,line,pline);
		//cout << "a failed become statement without become symbol" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		getsym();
		eleRef2 = expression();
		if(eleRef2 == -1){
			//cout << "a failed become statement with expression errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
            if(flag){
				tabRef = STable::locIndex(word,tabIndex-1,3);
				if(tabRef == -1){
					ErrorHandle::Error(21,lineCount,line,temppline);
					readToTarget(staHeadSym, 10);
					return;
				}/*
				string tempName;
				stringstream ss;
				ss<<"t"<<tempVarNum++;
				ss>>tempName;
				int tempRef = STable::addEle(tempName,STable::getType(tabRef+1),VARIABLE,tabIndex-1,true,false,-1,-1,++offset);
				tabIndex++;
				eleRef1 = EleTable::addEle(STable::getType(tabRef+1),true,0,"",tempRef);*/
				int tabEleRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabRef);
				FourEleTable::addEle(26,eleRef2,offsetRef,tabEleRef);
            }
			else{
				tabRef = STable::locIndex(word,tabIndex-1,1);
				if(tabRef == -1){
					ErrorHandle::Error(21,lineCount,line,temppline);
					readToTarget(staHeadSym, 10);
					return;
				}
				if(STable::getKind(tabRef) != VARIABLE){
					ErrorHandle::Error(22,lineCount,line,temppline);
					readToTarget(staHeadSym, 10);
					return;
				}
				eleRef1 = EleTable::addEle(STable::getType(tabRef),true,0,"",tabRef);
				FourEleTable::addEle(4,eleRef2,0,eleRef1);
            }
			if (SEMICOLON != sym){
				ErrorHandle::Error(12,lineCount,line,pline);
				//cout << "a become statement without semicolon" << endl;
				readToTarget(staHeadSym, 10);
				return;
			}
			else{
				//cout << "a become statement" << endl;
				getsym();
				return;
			}
		}
	}
}

void GrammarAnaly::readSta(){
    int tabRef;
    int eleRef;
	getsym();
	if (LPARENT != sym){
		ErrorHandle::Error(18,lineCount,line,pline);
		//cout << "a failed read statement without left parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else getsym();
	while (1){
		if (IDENT != sym){
			ErrorHandle::Error(8,lineCount,line,pline);
			//cout << "a failed read statement with errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
            tabRef = STable::locIndex(word,tabIndex-1,1);
			if(tabRef == -1){
				ErrorHandle::Error(21,lineCount,line,pline);
				//cout << "a failed read statement with errors" << endl;
				readToTarget(staHeadSym, 10);
				return;
			}
			if(STable::getKind(tabRef) != VARIABLE){
				ErrorHandle::Error(22,lineCount,line,pline);
                //cout << "a failed read statement with errors" << endl;
                readToTarget(staHeadSym, 10);
                return;
            }
            eleRef = EleTable::addEle(STable::getType(tabRef),true,0,"",tabRef);
            FourEleTable::addEle(21,0,0,eleRef);
            getsym();
		}
		if (COMMA == sym){
			getsym();
			continue;
		}
		else if (RPARENT != sym){
			ErrorHandle::Error(16,lineCount,line,pline);
			//cout << "a failed read statement without right parent" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
			getsym();
			if (SEMICOLON != sym){
				ErrorHandle::Error(12,lineCount,line,pline);
				//cout << "a read statement without semicolon" << endl;
				readToTarget(staHeadSym, 10);
				return;
			}
			else{
				//cout << "a read statement" << endl;
				getsym();
				return;
			}
		}
	}
}

void GrammarAnaly::writeSta(){
    int eleRef;
	getsym();
	if (LPARENT != sym){
		ErrorHandle::Error(18,lineCount,line,pline);
		//cout << "a failed write statement without left parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else getsym();
	if (STRING == sym){
	    string tempStr = word.substr(1,word.length()-1);
        eleRef = EleTable::addEle(STRING,false,0,tempStr,-1);
        FourEleTable::addEle(22,eleRef,0,0);
		getsym();
		if (COMMA == sym){
			getsym();
			eleRef = expression();
			if(eleRef == -1){
                //cout << "a failed write statement with expression errors" << endl;
                readToTarget(staHeadSym, 10);
                return;
			}
			STable::setIsTemp(EleTable::getRefer(eleRef), false);
			FourEleTable::addEle(22,eleRef,0,0);
		}
		if (RPARENT != sym){
			ErrorHandle::Error(16,lineCount,line,pline);
			//cout << "a failed write statement without right parent" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
			getsym();
			if (SEMICOLON != sym){
				ErrorHandle::Error(12,lineCount,line,pline);
				//cout << "a write statement without semicolon" << endl;
				readToTarget(staHeadSym, 10);
				return;
			}
			else{
				//cout << "a write statement" << endl;
				getsym();
				return;
			}
		}
	}
    eleRef = expression();
    if(eleRef == -1){
		//cout << "a failed write statement with errors" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	STable::setIsTemp(EleTable::getRefer(eleRef), false);
	FourEleTable::addEle(22,eleRef,0,0);
	if (RPARENT != sym){
		ErrorHandle::Error(16,lineCount,line,pline);
		//cout << "a failed write statement without right parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		getsym();
		if (SEMICOLON != sym){
			ErrorHandle::Error(12,lineCount,line,pline);
			//cout << "a write statement without semicolon" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else{
			//cout << "a write statement" << endl;
			getsym();
			return;
		}
	}
}

void GrammarAnaly::returnSta(){
    int eleRef;
    bool flag = false;
	getsym();
	if (LPARENT == sym){
		getsym();
		eleRef = expression();
		if(eleRef == -1){
			//cout << "a failed return statement with expression errors" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else if (RPARENT != sym){
			ErrorHandle::Error(16,lineCount,line,pline);
			//cout << "a failed return statement without right parent" << endl;
			readToTarget(staHeadSym, 10);
			return;
		}
		else {
			if(returnType == VOIDSY){
				ErrorHandle::Error(23,lineCount,line,pline);
                //cout << "a failed return statement" << endl;
                readToTarget(staHeadSym, 10);
                return;
			}
			STable::setIsTemp(EleTable::getRefer(eleRef), false);
            flag = true;
            getsym();
		}
	}
	if (SEMICOLON != sym){
		if(returnType == VOIDSY || flag){
			ErrorHandle::Error(12,lineCount,line,pline);
		}
		else{
			ErrorHandle::Error(18,lineCount,line,pline);
		}
		//cout << "a return statement without semicolon" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		//cout << "a return statement" << endl;
		if(flag){
            FourEleTable::addEle(18,eleRef,0,0);
		}
		else{
			if(returnType != VOIDSY){
				ErrorHandle::Error(23,lineCount,line,pline);
                //cout << "a failed return statement" << endl;
                readToTarget(staHeadSym, 10);
                return;
            }
		}
		if(currentFunc != -1){
			FourEleTable::addEle(19,currentFunc,0,0);
		}
        hasRet = true;
		getsym();
		return;
	}
}

void GrammarAnaly::caseSta(){
    int expRef;
    int eleRef;
    int expPointer;
	int tempStack;
	int caseTyp[100];
	int casePos[100];
	int caseRef[100];
	int casePointer[100];
	int casen = 0;
	getsym();
	if (LPARENT != sym){
		ErrorHandle::Error(18,lineCount,line,pline);
		//cout << "a failed switch statement without left parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		getsym();
	}
	expRef = expression();
	if(expRef == -1){
		//cout << "a failed switch statement with expression errors" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else if (RPARENT != sym){
		ErrorHandle::Error(16,lineCount,line,pline);
		//cout << "a failed switch statement without right parent" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		STable::setIsTemp(EleTable::getRefer(expRef), false);
	    expPointer = FourEleTable::addEle(16,expRef,0,-1);
		getsym();
		while(NEWLINE == sym){
			getsym();
		}
	}
	if (LBRACE != sym){
		ErrorHandle::Error(24,lineCount,line,pline);
		//cout << "a failed switch statement without left brace" << endl;
		readToTarget(staHeadSym, 10);
		return;
	}
	else{
		tempStack = braceStack;
		braceStack++;
		getsym();
		while(NEWLINE == sym){
			getsym();
		}
		//cout << "a switch statement" << endl;
	}
	bool haveCase = false;
	while (1){
		if (LBRACE == sym){
			braceStack++;
			getsym();
			int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
			skipUselessWords(tar, 4);
			continue;
		}
		if (RBRACE == sym){
			braceStack--;
			if (tempStack == braceStack){
				if (!haveCase){
					ErrorHandle::Error(25,lineCount,line,pline);
				}
				getsym();
				break;
			}
			else{
				getsym();
				int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
				skipUselessWords(tar, 4);
				continue;
			}
		}
		if (CASESY != sym){
			if (DEFSY == sym && haveCase){
				getsym();
				if (COLON != sym){
					ErrorHandle::Error(26,lineCount,line,pline);
					//cout << "a failed default case statement without colon" << endl;
					int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
					skipUselessWords(tar, 4);
					continue;
				}
				else{
					caseTyp[casen] = 1;
                    casePos[casen] = FourEleTable::getIndex();
                    caseRef[casen] = 0;
					//cout << "a default case statement" << endl;
					getsym();
					while(NEWLINE == sym){
						getsym();
					}
					if(sym == RBRACE){
						ErrorHandle::Error(27,lineCount,line,pline);
					}
					else{
						statement();
						while(NEWLINE == sym){
							getsym();
						}
					}
					braceStack--;
					if (RBRACE != sym){
						ErrorHandle::Error(28,lineCount,line,pline);
						readToTarget(staHeadSym, 10);
					}
					else{
						getsym();
					}
					casePointer[casen] = FourEleTable::addEle(6,0,0,-1);
					casen++;
					break;
				}
			}
			else{
				if(DEFSY == sym){
					getsym();
				}
				else{
					ErrorHandle::Error(29,lineCount,line,pline);
				}
				//cout << "a failed case statement without case symbol" << endl;
				int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
				skipUselessWords(tar, 4);
				continue;
			}
		}
		else{
			getsym();
		}
		if (CHAR == sym){
		    eleRef = EleTable::addEle(CHARSY,false,(int)word[1],"",-1);
			caseTyp[casen] = 0;
            casePos[casen] = FourEleTable::getIndex();
            caseRef[casen] = eleRef;
			getsym();
		}
		else if (integerDet()){
			//cout << "a failed case statement with integer errors" << endl;
			int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
			skipUselessWords(tar, 4);
			continue;
		}
		else{
			eleRef = EleTable::addEle(INTSY,false,integerVal,"",-1);
			caseTyp[casen] = 0;
            casePos[casen] = FourEleTable::getIndex();
            caseRef[casen] = eleRef;
		}
		if (COLON != sym){
			ErrorHandle::Error(26,lineCount,line,pline);
			//cout << "a failed case statement without colon" << endl;
			int tar[4] = { CASESY, DEFSY, RBRACE, LBRACE };
			skipUselessWords(tar, 4);
			continue;
		}
		else{
			//cout << "a case statement" << endl;
			haveCase = true;
			getsym();
			while(NEWLINE == sym){
				getsym();
			}
			if(sym == RBRACE){
				ErrorHandle::Error(27,lineCount,line,pline);
			}
			else{
				statement();
				while(NEWLINE == sym){
					getsym();
				}
			}
            casePointer[casen] = FourEleTable::addEle(6,0,0,-1);
            casen++;
		}
	}
	FourEleTable::setc(expPointer);
	for(int i = 0; i < casen; ++i){
        FourEleTable::addEle(17,caseTyp[i],caseRef[i],casePos[i]);
	}
	for(int i = 0; i < casen; ++i){
        FourEleTable::setc(casePointer[i]);
	}
}

void GrammarAnaly::statement(){
	if (sym == IFSY){
		conditionSta();
	}
	else if (WHISY == sym){
		loopSta();
	}
	else if (LBRACE == sym){
		braceStack++;
		statementLine();
	}
	else if (IDENT == sym){
		string funcName = word;
		int temppline = pline;
		getsym();
		if (LPARENT == sym){
            int tempRef = STable::locIndex(funcName,lastTabIndex,2);
			if(tempRef == -1){
				ErrorHandle::Error(21,lineCount,line,pline);
				readToTarget(staHeadSym,10);
				return;
			}
			if (funcall(tempRef) == -1){
				//cout << "a function call statement with errors" << endl;
				readToTarget(staHeadSym, 10);
			}
			else{
				if (SEMICOLON != sym){
					ErrorHandle::Error(12,lineCount,line,pline);
					//cout << "a function call statement without semicolon" << endl;
					readToTarget(staHeadSym, 10);
				}
				else{
					//cout << "a function call statement" << endl;
					getsym();
				}
			}
		}
		else if (BECOME == sym || LBRACK == sym){
			becomeSta(funcName, temppline);
		}
		else{
			ErrorHandle::Error(31,lineCount,line,pline);
			//cout << "an illegal statement" << endl;
			readToTarget(staHeadSym, 10);
		}
	}
	else if (SCASY == sym){
		readSta();
	}
	else if (PRISY == sym){
		writeSta();
	}
	else if (RETSY == sym){
		returnSta();
	}
	else if (SWISY == sym){
		caseSta();
	}
	else if (SEMICOLON == sym){
		//cout << "an empty statement" << endl;
		getsym();
	}
	else if(NEWLINE == sym){
		getsym();
	}
	else if (RBRACE != sym){
		ErrorHandle::Error(31,lineCount,line,pline);
		//cout << "an illegal statement" << endl;
		readToTarget(staHeadSym, 10);
	}
}

void GrammarAnaly::compoundSta(bool notMain){
	constDecl(false);
	varDecl(false);
	while (1){
		if (RBRACE == sym){
			braceStack--;
			if (braceStack == 0){
				if (notMain) getsym();
				while(NEWLINE == sym){
					getsym();
				}
				return;
			}
			getsym();
		}
		while(NEWLINE == sym){
			getsym();
		}
		statement();
	}
}

bool GrammarAnaly::funcDecl(bool skipped, int tabRef){
    int paraNo = 1;
    int tempSym;
    int funcRef;
	if (skipped){
		bool err = false;
		getsym();
		while (sym != RPARENT){
			if (INTSY != sym && CHARSY != sym){
				ErrorHandle::Error(7,lineCount,line,pline);
				int tar[3] = { VOIDSY, INTSY, CHARSY };
				//cout << "a failed function declaration. will skip useless function body" << endl;
				skipUselessWords(tar, 3);
				return true;
			}
			else {
                tempSym = sym;
                getsym();
			}
			if (IDENT != sym){
				ErrorHandle::Error(8,lineCount,line,pline);
				int tar[3] = { VOIDSY, INTSY, CHARSY };
				//cout << "a failed function declaration. will skip useless function body" << endl;
				skipUselessWords(tar, 3);
				return true;
			}
			else {
                STable::addEle(word,tempSym,VARIABLE,tabIndex-1,false,false,-1,paraNo++,-1);
                tabIndex++;
                getsym();
			}
			if (COMMA == sym){
				getsym();
				continue;
			}
			else if (sym != RPARENT){
				ErrorHandle::Error(16,lineCount,line,pline);
				err = true;
				break;
			}
		}
		if (RPARENT == sym){
			getsym();
		}
		while(NEWLINE == sym){
			getsym();
		}
		if (LBRACE != sym){
			ErrorHandle::Error(24,lineCount,line,pline);
			int tar[3] = { VOIDSY, INTSY, CHARSY };
			//cout << "a failed function declaration. will skip useless function body" << endl;
			skipUselessWords(tar, 3);
			return true;
		}
		else {
			if (STable::getType(tabRef) == INTSY){
				//cout << "an int function declaration";
			}
			else{
				//cout << "a char function declaration";
			}
			if (err){
				//cout << " without right parent";
			}
			//cout << endl;
			braceStack++;
			getsym();
			isLeaf = true;
			returnType = STable::getType(tabRef);
			hasRet = false;
			funcRef = STable::getRef(tabRef);
			FuncTable::setParaNo(funcRef,paraNo - 1);
			currentFunc = funcRef;
			offset = 2;
			FourEleTable::addEle(24,tabRef,0,0);
			int templc = lineCount;
			string templine = line;
			int temppline = pline;
			compoundSta(true);
			if(returnType != VOIDSY && !hasRet){
				ErrorHandle::Error(32,templc,templine,temppline);
			}
			FourEleTable::addEle(19,funcRef,0,0);
			FuncTable::setIsLeaf(funcRef,isLeaf);
			if(!isLeaf){
				offset++;
				for(int i = tabRef + 1; STable::getKind(i) == VARIABLE; ++i){
					STable::setOffsetp(i);
				}
			}
			FuncTable::setLen(funcRef,offset);
			return true;
		}
	}
	else{
		if(INTSY != sym && CHARSY != sym && VOIDSY != sym){
			ErrorHandle::Error(7,lineCount,line,pline);
			int tar[3] = { VOIDSY, INTSY, CHARSY };
			skipUselessWords(tar, 3);
			return true;
		}
		bool err = false;
		int funcSym = sym;
		getsym();
		if (MAINSY == sym){
			return false;
		}
		else if (IDENT != sym){
			ErrorHandle::Error(8,lineCount,line,pline);
			int tar[3] = { VOIDSY, INTSY, CHARSY };
			//cout << "a failed function declaration. will skip useless function body" << endl;
			skipUselessWords(tar, 3);
			return true;
		}
		else {
			if(STable::checkRe(word,lastTabIndex,true,true)){
				funcRef = FuncTable::addEle(funcSym == VOIDSY);
				tabRef = STable::addEle(word,funcSym,FUNCTION,lastTabIndex,false,true,funcRef,-1,-1);
				lastTabIndex = tabIndex;
				tabIndex++;
			}
			else{
				ErrorHandle::Error(11,lineCount,line,pline);
				int tar[3] = { VOIDSY, INTSY, CHARSY };
				//cout << "a failed function declaration. will skip useless function body" << endl;
				skipUselessWords(tar, 3);
				return true;
			}
            getsym();
        }
		if (LPARENT != sym){
			ErrorHandle::Error(18,lineCount,line,pline);
			int tar[3] = { VOIDSY, INTSY, CHARSY };
			//cout << "a failed function declaration. will skip useless function body" << endl;
			skipUselessWords(tar, 3);
			return true;
		}
		else getsym();
		while (sym != RPARENT){
			if (INTSY != sym && CHARSY != sym){
				ErrorHandle::Error(7,lineCount,line,pline);
				int tar[3] = { VOIDSY, INTSY, CHARSY };
				//cout << "a failed function declaration. will skip useless function body" << endl;
				skipUselessWords(tar, 3);
				return true;
			}
			else {
                tempSym = sym;
                getsym();
            }
			if (IDENT != sym){
				ErrorHandle::Error(8,lineCount,line,pline);
				int tar[3] = { VOIDSY, INTSY, CHARSY };
				//cout << "a failed function declaration. will skip useless function body" << endl;
				skipUselessWords(tar, 3);
				return true;
			}
			else {
                STable::addEle(word,tempSym,VARIABLE,tabIndex-1,false,false,-1,paraNo++,-1);
                tabIndex++;
                getsym();
            }
			if (COMMA == sym){
				getsym();
				continue;
			}
			else if (sym != RPARENT){
				ErrorHandle::Error(16,lineCount,line,pline);
				err = true;
				break;
			}
		}
		if (RPARENT == sym){
			getsym();
		}
		while(NEWLINE == sym){
			getsym();
		}
		if (LBRACE != sym){
			ErrorHandle::Error(24,lineCount,line,pline);
			int tar[3] = { VOIDSY, INTSY, CHARSY };
			//cout << "a failed function declaration. will skip useless function body" << endl;
			skipUselessWords(tar, 3);
			return true;
		}
		else {
			if (INTSY == funcSym){
				//cout << "an int function declaration";
			}
			else if (CHARSY == funcSym){
				//cout << "a char function declaration";
			}
			else{
				//cout << "a void function declaration";
			}
			if (err){
				//cout << " with errors";
			}
			//cout << endl;
			braceStack++;
			getsym();
			isLeaf = true;
			returnType = STable::getType(tabRef);
			hasRet = false;
			FuncTable::setParaNo(funcRef,paraNo - 1);
			currentFunc = funcRef;
			offset = 2;
			FourEleTable::addEle(24,tabRef,0,0);
			int templc = lineCount;
			string templine = line;
			int temppline = pline;
			compoundSta(true);
			if(returnType != VOIDSY && !hasRet){
				ErrorHandle::Error(32,templc,templine,temppline);
			}
			FourEleTable::addEle(19,funcRef,0,0);
			FuncTable::setIsLeaf(funcRef,isLeaf);
			if(!isLeaf){
				offset++;
				for(int i = tabRef + 1; STable::getKind(i) == VARIABLE; ++i){
					STable::setOffsetp(i);
				}
			}
			FuncTable::setLen(funcRef,offset);
			return true;
		}
	}
}

void GrammarAnaly::mainDecla(){
	getsym();
	if (LPARENT != sym){
		//error(10)
		int tar[1] = { RBRACE };
		//cout << "a failed main function declaration. will jump to the end" << endl;
		skipUselessWords(tar, 1);
		//cin >> sym;
		exit(0);
	}
	else getsym();
	if (RPARENT != sym){
		//error(11)
		int tar[1] = { RBRACE };
		//cout << "a failed main function declaration. will jump to the end" << endl;
		skipUselessWords(tar, 1);
		//cin >> sym;
		exit(0);
	}
	else getsym();
	while(NEWLINE == sym){
		getsym();
	}
	if(LBRACE != sym){
		ErrorHandle::Error(24,lineCount,line,pline);
		int target[13] = {IFSY, WHISY, LBRACE, RBRACE, IDENT, SCASY, SWISY, PRISY, RETSY, SEMICOLON, INTSY, CHARSY, CONSY};
		skipUselessWords(target, 13);
	}
    int funcRef = FuncTable::addEle(true);
	string mainname = "main";
	STable::addEle(mainname,VOIDSY,FUNCTION,lastTabIndex,false,true,funcRef,-1,-1);
	tabIndex++;
    offset = 0;
    isLeaf = true;
	braceStack++;
	//cout << "a main function declaration" << endl;
	getsym();
    returnType = VOIDSY;
	FourEleTable::addEle(23,0,0,0);
	currentFunc = -1;
	compoundSta(false);
	FuncTable::setIsLeaf(funcRef,isLeaf);
	if(!isLeaf){
        offset++;
	}
	FuncTable::setLen(funcRef,offset);
}

void GrammarAnaly::program(){
	int temp;
	bool tempb;
	getsym();
	while(NEWLINE == sym){
		getsym();
	}
	constDecl(true);
	temp = varDecl(true);
	lastTabIndex = tabIndex - 1;
	if (temp != -1){
		tempb = funcDecl(true,temp);
	}
	else{
		tempb = funcDecl(false,-1);
	}
	while (tempb){
		tempb = funcDecl(false,-1);
	}
	mainDecla();
}
