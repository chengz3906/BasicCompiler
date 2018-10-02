#include "GenMips.h"
int GenMips::fourNum;
int GenMips::index = 0;
int GenMips::labNo = 0;
int GenMips::bnum = 0;
int GenMips::strNum = 0;
int GenMips::soffset;
int GenMips::fct;
int GenMips::a;
int GenMips::b;
int GenMips::c;
int GenMips::areg;
int GenMips::breg;
int GenMips::creg;
int GenMips::aval;
int GenMips::bval;
int GenMips::tempReg[10];
string GenMips::strs[100];
int GenMips::labels[100];
bool GenMips::labelMark[100] = {false};

ofstream outcode("mips.txt");

GenMips::GenMips(void)
{
}

void GenMips::GenCode(){
	fourNum = FourEleTable::getIndex();
	int stabLen = STable::getIndex();
	outcode<<".data"<<endl;
	for(int i = 0; i < fourNum; ++i){
		if(FourEleTable::getFct(i) == 22 && EleTable::getType(FourEleTable::geta(i)) == STRING){
			string tempstr = EleTable::getStr(FourEleTable::geta(i));
			bool flag = true;
			for(int j = 0 ; j < strNum; ++j){
				if(tempstr == strs[j]){
					flag = false;
					break;
				}
			}
			if(!flag){
				continue;
			}
			if(strNum >= 100){
				ErrorHandle::Fatal(7);
				exit(0);
			}
			strs[strNum] = tempstr;
			outcode<<"str"<<strNum<<": .asciiz \""<<strs[strNum]<<endl;
			strNum++;
		}
	}
	outcode<<".text"<<endl;
	outcode<<"sw $gp,0($gp)"<<endl;
	outcode<<"move $fp,$gp"<<endl;
	for(int i = 0; i < stabLen; ++i){
		if(STable::getKind(i) == FUNCTION){
			i--;
			if(STable::getKind(i) == VARIABLE){
				if(STable::getType(i) == ARRAY){
					outcode<<"addi $sp,$fp,"<<4 * STable::getOffset(i) + 4 * ArrayTable::getLen(STable::getRef(i)) - 4<<endl;
				}
				else{
					outcode<<"addi $sp,$fp,"<<4 * STable::getOffset(i)<<endl;
				}
			}
			else{
				outcode<<"addi $sp,$fp,4"<<endl;
			}
			break;
		}
	}
	outcode<<"j main"<<endl;
	for(int i = 0; i < fourNum; ++i){
		int temp = FourEleTable::getFct(i);
		if(temp > 5 && temp < 13 || temp == 17){
			bool flag = true;
			int tempta = FourEleTable::getc(i);
			for(int j = 0; j < bnum; ++j){
				if(labels[j] == tempta){
					flag = false;
					break;
				}
			}
			if(flag){
				if(bnum >= 100){
					ErrorHandle::Fatal(9);
				}
				labels[bnum++] = tempta;
				//cout<<bnum-1<<" "<<i<<" "<<labels[bnum-1]<<endl;
			}
		}
	}
	for(; index < fourNum; ++index){
		fct = FourEleTable::getFct(index);
		for(int i = 0; i < bnum; ++i){
			if(index == labels[i]){
				outcode<<"lab"<<i<<":"<<endl;
				labelMark[i] = true;
				for(int i = 0; i < 10; ++i){
					tempReg[i] = -1;
				}
				break;
			}
		}
		GenLine();
	}
	for(int i = 0; i < bnum; ++i){
		if(!labelMark[i]){
			outcode<<"lab"<<i<<":"<<endl;
		}
	}
	outcode.close();
}

bool GenMips::Loada(){
	int offset;
	a = FourEleTable::geta(index);
	if(EleTable::getIsVar(a)){
		offset = STable::getOffset(EleTable::getRefer(a));
		if(STable::getIsGlo(EleTable::getRefer(a))){
			for(int i = 0; i < 10; ++i){
				if(a == tempReg[i]){
					areg = i;
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] < 0){
					tempReg[i] = a;
					areg = i;
					outcode<<"lw $t"<<i<<","<<4 * offset<<"($gp)"<<endl;
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] != FourEleTable::getb(index) && tempReg[i] != FourEleTable::getc(index)){
					if(EleTable::getIsVar(tempReg[i])){
						if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
						}
						else{
							if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
								outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
							else{
								outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
						}
					}
					tempReg[i] = a;
					areg = i;
					outcode<<"lw $t"<<i<<","<<4 * offset<<"($gp)"<<endl;
					return true;
				}
			}
			return true;
		}
		else{
			if(STable::getGloReg(EleTable::getRefer(a)) >= 0){
				areg = STable::getGloReg(EleTable::getRefer(a));
				return false;
			}
			for(int i = 0; i < 10; ++i){
				if(a == tempReg[i]){
					areg = i;
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] < 0){
					tempReg[i] = a;
					areg = i;
					int pno = STable::getParaNo(EleTable::getRefer(a));
					if(pno > 0){
						outcode<<"lw $t"<<i<<","<<4 * (-pno)<<"($fp)"<<endl;
					}
					else{
						outcode<<"lw $t"<<i<<","<<4 * offset<<"($fp)"<<endl;
					}
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] != FourEleTable::getb(index) && tempReg[i] != FourEleTable::getc(index)){
					if(EleTable::getIsVar(tempReg[i])){
						if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
						}
						else{
							if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
								outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
							else{
								outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
						}
					}
					tempReg[i] = a;
					areg = i;
					int pno = STable::getParaNo(EleTable::getRefer(a));
					if(pno > 0){
						outcode<<"lw $t"<<i<<","<<4 * (-pno)<<"($fp)"<<endl;
					}
					else{
						outcode<<"lw $t"<<i<<","<<4 * offset<<"($fp)"<<endl;
					}
					return true;
				}
			}
			return true;
		}
	}
	else{
		aval = EleTable::getNum(a);
		for(int i = 0; i < 10; ++i){
			if(a == tempReg[i]){
				areg = i;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] < 0){
				tempReg[i] = a;
				areg = i;
				outcode<<"li $t"<<i<<","<<aval<<endl;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] != FourEleTable::getb(index) && tempReg[i] != FourEleTable::getc(index)){
				if(EleTable::getIsVar(tempReg[i])){
					if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
						outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
					}
					else{
						if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
							outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
						else{
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
					}
				}
				tempReg[i] = a;
				areg = i;
				outcode<<"li $t"<<i<<","<<aval<<endl;
				return true;
			}
		}
		return true;
	}
}

bool GenMips::Loadb(){
	int offset;
	b = FourEleTable::getb(index);
	if(EleTable::getIsVar(b)){
		offset = STable::getOffset(EleTable::getRefer(b));
		if(STable::getIsGlo(EleTable::getRefer(b))){
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] < 0){
					tempReg[i] = b;
					breg = i;
					outcode<<"lw $t"<<i<<","<<4 * offset<<"($gp)"<<endl;
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] != FourEleTable::geta(index) && tempReg[i] != FourEleTable::getc(index)){
					if(EleTable::getIsVar(tempReg[i])){
						if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
						}
						else{
							if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
								outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
							else{
								outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
						}
					}
					tempReg[i] = b;
					breg = i;
					outcode<<"lw $t"<<i<<","<<4 * offset<<"($gp)"<<endl;
					return true;
				}
			}
			return true;
		}
		else{
			if(STable::getGloReg(EleTable::getRefer(b)) >= 0){
				breg = STable::getGloReg(EleTable::getRefer(b));
				return false;
			}
			for(int i = 0; i < 10; ++i){
				if(b == tempReg[i]){
					breg = i;
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] < 0){
					tempReg[i] = b;
					breg = i;
					int pno = STable::getParaNo(EleTable::getRefer(b));
					if(pno > 0){
						outcode<<"lw $t"<<i<<","<<4 * (-pno)<<"($fp)"<<endl;
					}
					else{
						outcode<<"lw $t"<<i<<","<<4 * offset<<"($fp)"<<endl;
					}
					return true;
				}
			}
			for(int i = 0; i < 10; ++i){
				if(tempReg[i] != FourEleTable::geta(index) && tempReg[i] != FourEleTable::getc(index)){
					if(EleTable::getIsVar(tempReg[i])){
						if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
						}
						else{
							if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
								outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
							else{
								outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
							}
						}
					}
					tempReg[i] = b;
					breg = i;
					int pno = STable::getParaNo(EleTable::getRefer(b));
					if(pno > 0){
						outcode<<"lw $t"<<i<<","<<4 * (-pno)<<"($fp)"<<endl;
					}
					else{
						outcode<<"lw $t"<<i<<","<<4 * offset<<"($fp)"<<endl;
					}
					return true;
				}
			}
			return true;
		}
	}
	else{
		bval = EleTable::getNum(b);
		for(int i = 0; i < 10; ++i){
			if(b == tempReg[i]){
				breg = i;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] < 0){
				tempReg[i] = b;
				breg = i;
				outcode<<"li $t"<<i<<","<<bval<<endl;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] != FourEleTable::geta(index) && tempReg[i] != FourEleTable::getc(index)){
				if(EleTable::getIsVar(tempReg[i])){
					if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
						outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
					}
					else{
						if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
							outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
						else{
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
					}
				}
				tempReg[i] = b;
				breg = i;
				outcode<<"li $t"<<i<<","<<bval<<endl;
				return true;
			}
		}
		return true;
	}
}

bool GenMips::Loadc(){
	int offset;
	c = FourEleTable::getc(index);
	offset = STable::getOffset(EleTable::getRefer(c));
	if(STable::getIsGlo(EleTable::getRefer(c))){
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] < 0){
				tempReg[i] = c;
				creg = i;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] != FourEleTable::geta(index) && tempReg[i] != FourEleTable::getb(index)){
				if(EleTable::getIsVar(tempReg[i])){
					if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
						outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
					}
					else{
						if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
							outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
						else{
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
					}
				}
				tempReg[i] = c;
				creg = i;
				return true;
			}
		}
		return true;
	}
	else{
		if(STable::getGloReg(EleTable::getRefer(c)) >= 0){
			creg = STable::getGloReg(EleTable::getRefer(c));
			return false;
		}
		for(int i = 0; i < 10; ++i){
			if(c == tempReg[i]){
				creg = i;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] < 0){
				tempReg[i] = c;
				creg = i;
				return true;
			}
		}
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] != FourEleTable::geta(index) && tempReg[i] != FourEleTable::getb(index)){
				if(EleTable::getIsVar(tempReg[i])){
					if(STable::getIsGlo(EleTable::getRefer(tempReg[i]))){
						outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($gp)"<<endl;
					}
					else{
						if(STable::getParaNo(EleTable::getRefer(tempReg[i])) > 0){
							outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
						else{
							outcode<<"sw $t"<<i<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[i]))<<"($fp)"<<endl;
						}
					}
				}
				tempReg[i] = c;
				creg = i;
				return true;
			}
		}
		return true;
	}
}

void GenMips::GenLine(){
	int funcRef;
	int type;
	int len;
	int offset;
	bool ba,bb,bc;
	switch(fct){
	case 0:
		ba = Loada();
		bb = Loadb();
		bc = Loadc();
		outcode<<"add ";
		if(!bc){
			outcode<<"$s"<<creg<<",";
		}
		else{
			outcode<<"$t"<<creg<<",";
		}
		if(!ba){
			outcode<<"$s"<<areg<<",";
		}
		else{
			outcode<<"$t"<<areg<<",";
		}
		if(!bb){
			outcode<<"$s"<<breg<<endl;
		}
		else{
			outcode<<"$t"<<breg<<endl;
		}
		break;
	case 1:
		ba = Loada();
		bb = Loadb();
		bc = Loadc();
		outcode<<"sub ";
		if(!bc){
			outcode<<"$s"<<creg<<",";
		}
		else{
			outcode<<"$t"<<creg<<",";
		}
		if(!ba){
			outcode<<"$s"<<areg<<",";
		}
		else{
			outcode<<"$t"<<areg<<",";
		}
		if(!bb){
			outcode<<"$s"<<breg<<endl;
		}
		else{
			outcode<<"$t"<<breg<<endl;
		}
		break;
	case 2:
		ba = Loada();
		bb = Loadb();
		bc = Loadc();
		outcode<<"mult ";
		if(!ba){
			outcode<<"$s"<<areg<<",";
		}
		else{
			outcode<<"$t"<<areg<<",";
		}
		if(!bb){
			outcode<<"$s"<<breg<<endl;
		}
		else{
			outcode<<"$t"<<breg<<endl;
		}
		outcode<<"mflo ";
		if(!bc){
			outcode<<"$s"<<creg<<endl;
		}
		else{
			outcode<<"$t"<<creg<<endl;
		}
		break;
	case 3:
		ba = Loada();
		bb = Loadb();
		bc = Loadc();
		outcode<<"beq ";
		if(!bb){
			outcode<<"$s"<<breg<<",";
		}
		else{
			outcode<<"$t"<<breg<<",";
		}
		outcode<<"$0,tlab"<<labNo<<endl;
		outcode<<"div ";
		if(!ba){
			outcode<<"$s"<<areg<<",";
		}
		else{
			outcode<<"$t"<<areg<<",";
		}
		if(!bb){
			outcode<<"$s"<<breg<<endl;
		}
		else{
			outcode<<"$t"<<breg<<endl;
		}
		outcode<<"mflo ";
		if(!bc){
			outcode<<"$s"<<creg<<endl;
		}
		else{
			outcode<<"$t"<<creg<<endl;
		}
		outcode<<"tlab"<<labNo++<<":"<<endl;
		break;
	case 4:
		ba = Loada();
		bc = Loadc();
		outcode<<"move ";
		if(!bc){
			outcode<<"$s"<<creg<<",";
		}
		else{
			outcode<<"$t"<<creg<<",";
		}
		if(!ba){
			outcode<<"$s"<<areg<<endl;
		}
		else{
			outcode<<"$t"<<areg<<endl;
		}
		break;
	case 5:
		ba = Loada();
		bc = Loadc();
		outcode<<"sub ";
		if(!bc){
			outcode<<"$s"<<creg<<",";
		}
		else{
			outcode<<"$t"<<creg<<",";
		}
		outcode<<"$0,";
		if(!ba){
			outcode<<"$s"<<areg<<endl;
		}
		else{
			outcode<<"$t"<<areg<<endl;
		}
		break;
	case 6:
		c = FourEleTable::getc(index);
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				outcode<<"j lab"<<i<<endl;
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 7:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"beq $t0,$t1,lab"<<i<<endl;
				outcode<<"beq ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 8:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"bne $t0,$t1,lab"<<i<<endl;
				outcode<<"bne ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 9:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"blt $t0,$t1,lab"<<i<<endl;
				outcode<<"blt ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 10:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"ble $t0,$t1,lab"<<i<<endl;
				outcode<<"ble ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 11:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"bgt $t0,$t1,lab"<<i<<endl;
				outcode<<"bgt ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 12:
		c = FourEleTable::getc(index);
		ba = Loada();
		bb = Loadb();
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(int i = 0; i < bnum; ++i){
			if(c == labels[i]){
				//outcode<<"bge $t0,$t1,lab"<<i<<endl;
				outcode<<"bge ";
				if(!ba){
					outcode<<"$s"<<areg<<",";
				}
				else{
					outcode<<"$t"<<areg<<",";
				}
				if(!bb){
					outcode<<"$s"<<breg<<",lab"<<i<<endl;
				}
				else{
					outcode<<"$t"<<breg<<",lab"<<i<<endl;
				}
				break;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 13:
		soffset = FourEleTable::geta(index);
		break;
	case 14:
		b = FourEleTable::getb(index);
		ba = Loada();
		//outcode<<"sw $t0,"<<4 * (soffset - b)<<"($sp)"<<endl;
		outcode<<"sw ";
		if(!ba){
			outcode<<"$s"<<areg<<","<<4 * (soffset - b)<<"($sp)"<<endl;
		}
		else{
			outcode<<"$t"<<areg<<","<<4 * (soffset - b)<<"($sp)"<<endl;
		}
		break;
	case 15:
		a = FourEleTable::geta(index);
		funcRef = STable::getRef(EleTable::getRefer(a));
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		outcode<<"sw $fp,"<<4 * soffset<<"($sp)"<<endl;
		outcode<<"addi $fp,$sp,"<<4 * soffset<<endl;
		outcode<<"sw $sp,4($fp)"<<endl;
		outcode<<"addi $sp,$fp,"<<4 * FuncTable::getLen(funcRef)<<endl;
		outcode<<"jal "<<"func"<<STable::getId(EleTable::getRefer(a))<<endl;
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 16:
		ba = Loada();
		c = FourEleTable::getc(index);
		bool flag;
		bool bb;
		bool bbb;
		int target;
		flag = true;
		for(int i = 0; i < 10; ++i){
			if(tempReg[i] >= 0 && EleTable::getIsVar(tempReg[i])){
				int temprefer = EleTable::getRefer(tempReg[i]);
				if(STable::getIsGlo(temprefer)){
					outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($gp)"<<endl;
				}
				else{
					if(STable::getParaNo(temprefer) > 0){
						outcode<<"sw $t"<<i<<","<<-4 * STable::getParaNo(temprefer)<<"($fp)"<<endl;
					}
					else{
						outcode<<"sw $t"<<i<<","<<STable::getOffset(temprefer) * 4<<"($fp)"<<endl;
					}
				}
			}
		}
		for(target = c; FourEleTable::getFct(target) == 17 && target < FourEleTable::getIndex(); ++target){
			for(int j = 0; j < bnum; ++j){
				if(labels[j] == FourEleTable::getc(target)){
					if(FourEleTable::geta(target)){
						outcode<<"j lab"<<j<<endl;
						flag = false;
					}
					else{
						//outcode<<"li $t1,"<<EleTable::getNum(FourEleTable::getb(target))<<endl;
						b = FourEleTable::getb(target);
						bval = EleTable::getNum(b);
						bb = false;
						for(int k = 0; k < 10; ++k){
							if(b == tempReg[k]){
								breg = k;
								bb = true;
								break;
							}
						}
						if(!bb){
							bbb = false;
							for(int k = 0; k < 10; ++k){
								if(tempReg[k] < 0){
									tempReg[k] = b;
									breg = k;
									outcode<<"li $t"<<k<<","<<bval<<endl;
									bbb = true;
									break;
								}
							}
							if(!bbb){
								for(int k = 0; k < 10; ++k){
									if(tempReg[k] != FourEleTable::geta(index)){
										if(EleTable::getIsVar(tempReg[k])){
											if(STable::getIsGlo(EleTable::getRefer(tempReg[k]))){
												outcode<<"sw $t"<<k<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[k]))<<"($gp)"<<endl;
											}
											else{
												if(STable::getParaNo(EleTable::getRefer(tempReg[k])) > 0){
													outcode<<"sw $t"<<k<<","<<-4 * STable::getParaNo(EleTable::getRefer(tempReg[k]))<<"($fp)"<<endl;
												}
												else{
													outcode<<"sw $t"<<k<<","<<4 * STable::getOffset(EleTable::getRefer(tempReg[k]))<<"($fp)"<<endl;
												}
											}
										}
										tempReg[k] = b;
										breg = k;
										outcode<<"li $t"<<k<<","<<bval<<endl;
									}
								}
							}
						}
						//outcode<<"beq $t0,$t1,lab"<<j<<endl;
						outcode<<"beq ";
						if(!ba){
							outcode<<"$s"<<areg<<",";
						}
						else{
							outcode<<"$t"<<areg<<",";
						}
						outcode<<"$t"<<breg<<",lab"<<j<<endl;
					}
					break;
				}
			}
		}
		if(flag){
			for(int j = 0; j < bnum; ++j){
				if(labels[j] == target){
					outcode<<"j lab"<<j<<endl;
				}
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 18:
		ba = Loada();
		//outcode<<"move $v1,$t0"<<endl;
		outcode<<"move $v1,";
		if(!ba){
			outcode<<"$s"<<areg<<endl;
		}
		else{
			outcode<<"$t"<<areg<<endl;
		}
		break;
	case 19:
		a = FourEleTable::geta(index);
		if(!FuncTable::getIsLeaf(a)){
			outcode<<"lw $ra,8($fp)"<<endl;
			for(int i = 0; i < FuncTable::getSRnum(a); ++i){
				outcode<<"lw $s"<<i<<","<<12 + 4 * i<<"($fp)"<<endl;
			}
		}
		else{
			for(int i = 0; i < FuncTable::getSRnum(a); ++i){
				outcode<<"lw $s"<<i<<","<<8 + 4 * i<<"($fp)"<<endl;
			}
		}
		outcode<<"lw $sp,4($fp)"<<endl;
		outcode<<"lw $fp,0($fp)"<<endl;
		outcode<<"jr $ra"<<endl;
		break;
	case 20:
		//outcode<<"move $t0,$v1"<<endl;
		bc = Loadc();
		outcode<<"move ";
		if(!bc){
			outcode<<"$s"<<creg<<",$v1"<<endl;
		}
		else{
			outcode<<"$t"<<creg<<",$v1"<<endl;
		}
		break;
	case 21:
		c = FourEleTable::getc(index);
		if(EleTable::getType(c) == CHARSY){
			outcode<<"li $v0,12"<<endl;
		}
		else{
			outcode<<"li $v0,5"<<endl;
		}
		outcode<<"syscall"<<endl;
		//outcode<<"move $t0,$v0"<<endl;
		bc = Loadc();
		outcode<<"move ";
		if(!bc){
			outcode<<"$s"<<creg<<",$v0"<<endl;
		}
		else{
			outcode<<"$t"<<creg<<",$v0"<<endl;
		}
		break;
	case 22:
		ba = Loada();
		type = EleTable::getType(a);
		if(type == CHARSY){
			//outcode<<"move $a0,$t0"<<endl;
			outcode<<"move $a0,";
			if(!ba){
				outcode<<"$s"<<areg<<endl;
			}
			else{
				outcode<<"$t"<<areg<<endl;
			}
			outcode<<"li $v0,11"<<endl;
			outcode<<"syscall"<<endl;
		}
		else if(type == STRING){
			string tempstr = EleTable::getStr(a);
			for(int i = 0; i < strNum; ++i){
				if(strs[i] == tempstr){
					outcode<<"la $a0,str"<<i<<endl;
					outcode<<"li $v0,4"<<endl;
					outcode<<"syscall"<<endl;
					break;
				}
			}
		}
		else if(type == INTSY){
			//outcode<<"move $a0,$t0"<<endl;
			outcode<<"move $a0,";
			if(!ba){
				outcode<<"$s"<<areg<<endl;
			}
			else{
				outcode<<"$t"<<areg<<endl;
			}
			outcode<<"li $v0,1"<<endl;
			outcode<<"syscall"<<endl;
		}
		break;
	case 23:
		outcode<<"main:"<<endl;
		outcode<<"move $fp,$sp"<<endl;
		outcode<<"sw $fp,0($fp)"<<endl;
		int mainLen;
		for(int i = STable::getIndex() - 1; i > 0; --i){
			if(STable::getOffset(i) > 0 && STable::getKind(i) == VARIABLE){
				mainLen = STable::getOffset(i);
				break;
			}
		}
		outcode<<"addi $sp,$fp,"<<4 * mainLen<<endl;
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 24:
		a = FourEleTable::geta(index);
		funcRef = STable::getRef(a);
		outcode<<"func"<<STable::getId(a)<<":"<<endl;
		if(!FuncTable::getIsLeaf(STable::getRef(a))){
			outcode<<"sw $ra,8($fp)"<<endl;
			for(int i = 0; i < FuncTable::getSRnum(funcRef); ++i){
				outcode<<"sw $s"<<i<<","<<12 + 4 * i<<"($fp)"<<endl;
			}
		}
		else{
			for(int i = 0; i < FuncTable::getSRnum(funcRef); ++i){
				outcode<<"sw $s"<<i<<","<<8 + 4 * i<<"($fp)"<<endl;
			}
		}
		for(int i = a + 1; STable::getParaNo(i) > 0 ; ++i){
			if(STable::getGloReg(i) >= 0){
				outcode<<"lw $s"<<STable::getGloReg(i)<<","<<-4 * STable::getParaNo(i)<<"($fp)"<<endl;
			}
		}
		for(int i = 0; i < 10; ++i){
			tempReg[i] = -1;
		}
		break;
	case 25:
		a = FourEleTable::geta(index);
		len = ArrayTable::getLen(STable::getRef(EleTable::getRefer(a)));
		offset = STable::getOffset(EleTable::getRefer(a));
		bb = Loadb();
		bc = Loadc();
		/*
		outcode<<"li $t0,"<<len<<endl;
		outcode<<"ble $t0,$t1,tlab"<<labNo<<endl;
		outcode<<"blt $t1,0,tlab"<<labNo<<endl;*/
		if(STable::getIsGlo(EleTable::getRefer(a))){
			//outcode<<"sll $t1,$t1,2"<<endl;
			outcode<<"sll ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
			//outcode<<"add $gp,$gp,$t1"<<endl;
			outcode<<"add $gp,$gp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			//outcode<<"lw $t9,"<<4 * offset<<"($gp)"<<endl;
			outcode<<"lw ";
			if(!bc){
				outcode<<"$s"<<creg<<","<<4 * offset<<"($gp)"<<endl;
			}
			else{
				outcode<<"$t"<<creg<<","<<4 * offset<<"($gp)"<<endl;
			}
			//outcode<<"sub $gp,$gp,$t1"<<endl;
			outcode<<"sub $gp,$gp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			outcode<<"srl ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
		}
		else{
			//outcode<<"sll $t1,$t1,2"<<endl;
			outcode<<"sll ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
			//outcode<<"add $fp,$fp,$t1"<<endl;
			outcode<<"add $fp,$fp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			//outcode<<"lw $t9,"<<4 * offset<<"($fp)"<<endl;
			outcode<<"lw ";
			if(!bc){
				outcode<<"$s"<<creg<<","<<4 * offset<<"($fp)"<<endl;
			}
			else{
				outcode<<"$t"<<creg<<","<<4 * offset<<"($fp)"<<endl;
			}
			//outcode<<"sub $fp,$fp,$t1"<<endl;
			outcode<<"sub $fp,$fp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			outcode<<"srl ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
		}
		//outcode<<"tlab"<<labNo++<<":"<<endl;
		break;
	case 26:
		ba = Loada();
		bb = Loadb();
		c = FourEleTable::getc(index);
		len = ArrayTable::getLen(STable::getRef(EleTable::getRefer(c)));
		offset = STable::getOffset(EleTable::getRefer(c));/*
		outcode<<"li $t0,"<<len<<endl;
		outcode<<"ble $t0,$t1,tlab"<<labNo<<endl;
		outcode<<"blt $t1,0,tlab"<<labNo<<endl;*/
		if(STable::getIsGlo(EleTable::getRefer(c))){
			//outcode<<"sll $t1,$t1,2"<<endl;
			outcode<<"sll ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
			//outcode<<"add $gp,$gp,$t1"<<endl;
			outcode<<"add $gp,$gp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			//outcode<<"sw $t0,"<<4 * offset<<"($gp)"<<endl;
			outcode<<"sw ";
			if(!ba){
				outcode<<"$s"<<areg<<","<<4 * offset<<"($gp)"<<endl;
			}
			else{
				outcode<<"$t"<<areg<<","<<4 * offset<<"($gp)"<<endl;
			}
			//outcode<<"sub $gp,$gp,$t1"<<endl;
			outcode<<"sub $gp,$gp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			outcode<<"srl ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
		}
		else{
			//outcode<<"sll $t1,$t1,2"<<endl;
			outcode<<"sll ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
			//outcode<<"add $fp,$fp,$t1"<<endl;
			outcode<<"add $fp,$fp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			//outcode<<"sw $t0,"<<4 * offset<<"($fp)"<<endl;
			outcode<<"sw ";
			if(!ba){
				outcode<<"$s"<<areg<<","<<4 * offset<<"($fp)"<<endl;
			}
			else{
				outcode<<"$t"<<areg<<","<<4 * offset<<"($fp)"<<endl;
			}
			//outcode<<"sub $fp,$fp,$t1"<<endl;
			outcode<<"sub $fp,$fp,";
			if(!bb){
				outcode<<"$s"<<breg<<endl;
			}
			else{
				outcode<<"$t"<<breg<<endl;
			}
			outcode<<"srl ";
			if(!bb){
				outcode<<"$s"<<breg<<",";
			}
			else{
				outcode<<"$t"<<breg<<",";
			}
			if(!bb){
				outcode<<"$s"<<breg<<",2"<<endl;
			}
			else{
				outcode<<"$t"<<breg<<",2"<<endl;
			}
		}
		//outcode<<"tlab"<<labNo++<<":"<<endl;
		break;
	}
}