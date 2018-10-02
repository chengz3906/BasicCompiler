#include "Optimise.h"


Optimise::Optimise(void)
{
}

void Optimise::beginOp(){
	int fct;
	int fourIndex = FourEleTable::getIndex();
	tabIndex = 0;
	for(int i = 0; i < FourEleTable::getIndex(); ++i){
		fct = FourEleTable::getFct(i);
		if(23 == fct || 24 == fct){
			addEle(fct, FourEleTable::geta(i),FourEleTable::getb(i),FourEleTable::getc(i));
			start = i + 1;
			getFunBlock();
			OpDag();
		}
	}
	GloAllo();
	OpOffset();
	peephole();
	FourEleTable::flushAll();
	for(int i = 0; i < tabIndex; ++i){
		//cout<<i<<" "<<newFETab[i].fct<<" "<<newFETab[i].a<<" "<<newFETab[i].b<<" "<<newFETab[i].c<<endl;
		FourEleTable::addEle(newFETab[i].fct,newFETab[i].a,newFETab[i].b,newFETab[i].c);
	}
}

void Optimise::OpOffset(){
	int fours;
	int fourt;
	int eles;
	int elet;
	int fct;
	for(int i = 0; i < tabIndex; ++i){
		fct = newFETab[i].fct;
		if(24 == fct){
			fours = i;
			eles = newFETab[i].a + 1;
			for(int j = i + 1; j < tabIndex; ++j){
				if(newFETab[j].fct == 23){
					fourt = j;
					for(int k = eles; k < STable::getIndex(); ++k){
						if(STable::getId(k) == "main"){
							elet = k;
							break;
						}
					}
					break;
				}
				if(newFETab[j].fct == 24){
					fourt = j;
					elet = newFETab[j].a;
					break;
				}
			}
		}
		else if(23 == fct){
			fours = i;
			for(int k = 0; k < STable::getIndex(); ++k){
				if(STable::getId(k) == "main"){
					eles = k + 1;
					break;
				}
			}
			fourt = tabIndex;
			elet = STable::getIndex();
		}
		else continue;
		for(int j = eles; j < elet; ++j){
			if(STable::getGloReg(j) >= 0 && STable::getParaNo(j) < 1){
				STable::deleteEle(j);
				int funcindex = STable::getRef(eles-1);
				FuncTable::setLen(funcindex,FuncTable::getLen(funcindex)-1);
				continue;
			}
			if(STable::getIsTemp(j)){
				bool flag = false;
				for(int k = fours; k < fourt; ++k){
					if(newFETab[k].fct < 6 || newFETab[k].fct > 24){
						if(EleTable::getRefer(newFETab[k].a) == j || EleTable::getRefer(newFETab[k].c) == j){
							flag = true;
							break;
						}
						if(newFETab[k].fct < 4 || newFETab[k].fct > 5){
							if(EleTable::getRefer(newFETab[k].b) == j){
								flag = true;
								break;
							}
						}
					}
				}
				if(!flag){
					STable::deleteEle(j);
					int funcindex = STable::getRef(eles-1);
					FuncTable::setLen(funcindex,FuncTable::getLen(funcindex)-1);
				}
			}
		}
		i = fourt - 1;
	}
}

void Optimise::addEle(int fct, int a, int b, int c){
	newFETab[tabIndex].fct = fct;
	newFETab[tabIndex].a = a;
	newFETab[tabIndex].b = b;
	newFETab[tabIndex].c = c;
	tabIndex++;
}

void Optimise::getFunBlock(){
	n = 1;
	int m;
	int fct;
	int minNum;
	int minLoc;
	entrance[0] = start;
	for(m = start; m < FourEleTable::getIndex(); ++m){
		fct = FourEleTable::getFct(m);
		if(6 <= fct && 12 >= fct){
			bool flag = false;
			for(int i = 0; i < n; ++i){
				if(m + 1 == entrance[i]){
					flag = true;
					break;
				}
			}
			if(!flag){
				entrance[n++] = m + 1;
			}
			flag = false;
			int tempc = FourEleTable::getc(m);
			for(int i = 0; i < n; ++i){
				if(tempc == entrance[i]){
					flag = true;
					break;
				}
			}
			if(!flag){
				entrance[n++] = FourEleTable::getc(m);
			}
		}
		if(16 == fct || 15 == fct){
			bool flag = false;
			for(int i = 0; i < n; ++i){
				if(m + 1 == entrance[i]){
					flag = true;
					break;
				}
			}
			if(!flag){
				entrance[n++] = m + 1;
			}
		}
		if(17 == fct){
			bool flag = false;
			int tempc = FourEleTable::getc(m);
			for(int i = 0; i < n; ++i){
				if(tempc == entrance[i]){
					flag = true;
					break;
				}
			}
			if(!flag){
				entrance[n++] = FourEleTable::getc(m);
			}
		}
		if(23 == fct || 24 == fct){
			break;
		}
	}
	entrance[n] = m;
	for(int i = 0; i < n; ++i){
		minNum = entrance[i + 1];
		minLoc = i + 1;
		for(int j = i + 2; j < n; ++j){
			if(entrance[j] < minNum){
				minNum = entrance[j];
				minLoc = j;
			}
		}
		int temp = entrance[i + 1];
		entrance[i + 1] = minNum;
		entrance[minLoc] = temp;
	}/*
	for(int i = 0; i < n; ++i){
		cout<<entrance[i]<<" ";
	}
	cout<<endl;*/
	if(VOIDSY != STable::getType(FourEleTable::geta(start-1)) && 24 == FourEleTable::getFct(start-1)){
		bool flag = false;
		for(int i = entrance[n-1]; i < m; ++i){
			if(18 == FourEleTable::getFct(i)){
				flag = true;
				break;
			}
		}
		if(!flag){
			for(int i = entrance[0]; i < entrance[1]; ++i){
				if(18 == FourEleTable::getFct(i)){
					flag = true;
					break;
				}
			}
		}
		if(!flag){
			cout<<"function "<<STable::getId(FourEleTable::geta(start-1))<<" may return nothing"<<endl;
			ErrorHandle::setHaveError();
		}
	}
}

void Optimise::OpDag(){
	int left[NODE_NUM], right[NODE_NUM], oper[NODE_NUM], self[NODE_NUM];
	int nodeRefer[NODE_NUM], nodeNo[NODE_NUM];
	int nodeIndex = 0;
	int eleIndex = 0;
	int tempa, tempb, tempc, tempf;
	int nodea, nodeb;
	int genFour[NODE_NUM];
	int gfi = 0;
	iindex = 0;
	for(int i = 0; i < n; ++i){
		int head = entrance[i];
		int tail;
		bool flag, f[2], nodes[NODE_NUM];
		int tempfct;
		oldIndex[iindex] = head;
		newIndex[iindex] = tabIndex;
		iindex++;
		while(head < entrance[i+1]){
			flag = false;
			for(int j = head; j < entrance[i+1]; ++j){
				tempfct = FourEleTable::getFct(j);
				if(tempfct > 5){
					tail = j;
					flag = true;
					break;
				}
			}
			if(!flag){
				tail = entrance[i+1];
			}
			if(head == tail){
				addEle(FourEleTable::getFct(tail), FourEleTable::geta(tail), FourEleTable::getb(tail), FourEleTable::getc(tail));
				head = tail + 1;
				continue;
			}
			eleIndex = 0;
			nodeIndex = 0;
			gfi = 0;
			for(int j = head; j < tail; ++j){
				f[0] = false;
				f[1] = false;
				tempa = FourEleTable::geta(j);
				tempb = FourEleTable::getb(j);
				tempc = FourEleTable::getc(j);
				tempf = FourEleTable::getFct(j);
				if(4 == tempf){
					for(int k = 0; k < eleIndex; ++k){
						if(tempa == nodeRefer[k]){
							nodea = nodeNo[k];
							f[0] = true;
							break;
						}
					}
					if(!f[0]){
						left[nodeIndex] = -1;
						right[nodeIndex] = -1;
						oper[nodeIndex] = -1;
						self[nodeIndex] = tempa;
						nodeRefer[eleIndex] = tempa;
						nodeNo[eleIndex] = nodeIndex;
						nodea = nodeIndex;
						nodeIndex++;
						eleIndex++;
					}
					for(int k = 0; k < eleIndex; ++k){
						if(tempc == nodeRefer[k]){
							nodeNo[k] = nodea;
							f[1] = true;
							break;
						}
					}
					if(!f[1]){
						nodeRefer[eleIndex] = tempc;
						nodeNo[eleIndex] = nodea;
						eleIndex++;
					}
				}
				else if(5 == tempf){
					for(int k = 0; k < eleIndex; ++k){
						if(tempa == nodeRefer[k]){
							nodea = nodeNo[k];
							f[0] = true;
						}
					}
					if(!f[0]){
						left[nodeIndex] = -1;
						right[nodeIndex] = -1;
						oper[nodeIndex] = -1;
						self[nodeIndex] = tempa;
						nodeRefer[eleIndex] = tempa;
						nodeNo[eleIndex] = nodeIndex;
						nodea = nodeIndex;
						nodeIndex++;
						eleIndex++;
						left[nodeIndex] = nodea;
						right[nodeIndex] = -1;
						oper[nodeIndex] = 5;
						self[nodeIndex] = tempc;
						flag = false;
						for(int k = 0; k < eleIndex; ++k){
							if(nodeRefer[k] == tempc){
								nodeNo[k] = nodeIndex;
								flag = true;
								break;
							}
						}
						if(!flag){
							nodeRefer[eleIndex] = tempc;
							nodeNo[eleIndex] = nodeIndex;
							eleIndex++;
						}
						nodeIndex++;
					}
					else{
						f[0] = false;
						f[1] = false;
						for(int k = 0; k < nodeIndex; ++k){
							if(left[k] == nodea && oper[k] == 5){
								for(int kk = 0; kk < eleIndex; ++kk){
									if(nodeRefer[kk] == tempc){
										nodeNo[kk] = k;
										f[1] = true;
										break;
									}
								}
								if(!f[1]){
									nodeRefer[eleIndex] = tempc;
									nodeNo[eleIndex] = k;
									eleIndex++;
								}
							}
							f[0] = true;
							break;
						}
						if(!f[0]){
							left[nodeIndex] = nodea;
							right[nodeIndex] = -1;
							oper[nodeIndex] = 5;
							self[nodeIndex] = tempc;
							flag = false;
							for(int k = 0; k < eleIndex; ++k){
								if(nodeRefer[k] == tempc){
									nodeNo[k] = nodeIndex;
									flag = true;
									break;
								}
							}
							if(!flag){
								nodeRefer[eleIndex] = tempc;
								nodeNo[eleIndex] = nodeIndex;
								eleIndex++;
							}
							nodeIndex++;
						}
					}
				}
				else{
					for(int k = 0; k < eleIndex; ++k){
						if(tempa == nodeRefer[k]){
							nodea = nodeNo[k];
							f[0] = true;
						}
						if(tempb == nodeRefer[k]){
							nodeb = nodeNo[k];
							f[1] = true;
						}
						if(f[0] && f[1]){
							break;
						}
					}
					if(f[0] && f[1]){
						f[0] = false;
						f[1] = false;
						for(int k = 0; k < nodeIndex; ++k){
							if((left[k] == nodea && right[k] == nodeb && oper[k] == tempf) || 
								(left[k] == nodeb && left[k] == nodea && oper[k] == tempf && (tempf == 0 || tempf == 2))){
									for(int kk = 0; kk < eleIndex; ++kk){
										if(nodeRefer[kk] == tempc){
											nodeNo[kk] = k;
											f[1] = true;
											break;
										}
									}
									if(!f[1]){
										nodeRefer[eleIndex] = tempc;
										nodeNo[eleIndex] = k;
										eleIndex++;
									}
									f[0] = true;
									break;
							}
						}
						if(!f[0]){
							left[nodeIndex] = nodea;
							right[nodeIndex] = nodeb;
							oper[nodeIndex] = tempf;
							self[nodeIndex] = tempc;
							flag = false;
							for(int k = 0; k < eleIndex; ++k){
								if(nodeRefer[k] == tempc){
									nodeNo[k] = nodeIndex;
									flag = true;
									break;
								}
							}
							if(!flag){
								nodeRefer[eleIndex] = tempc;
								nodeNo[eleIndex] = nodeIndex;
								eleIndex++;
							}
							nodeIndex++;
						}
					}
					else{
						if(!f[0]){
							left[nodeIndex] = -1;
							right[nodeIndex] = -1;
							oper[nodeIndex] = -1;
							self[nodeIndex] = tempa;
							nodeRefer[eleIndex] = tempa;
							nodeNo[eleIndex] = nodeIndex;
							nodea = nodeIndex;
							nodeIndex++;
							eleIndex++;
						}
						if(!f[1]){
							left[nodeIndex] = -1;
							right[nodeIndex] = -1;
							oper[nodeIndex] = -1;
							self[nodeIndex] = tempb;
							nodeRefer[eleIndex] = tempb;
							nodeNo[eleIndex] = nodeIndex;
							nodeb = nodeIndex;
							nodeIndex++;
							eleIndex++;
						}
						left[nodeIndex] = nodea;
						right[nodeIndex] = nodeb;
						oper[nodeIndex] = tempf;
						self[nodeIndex] = tempc;
						flag = false;
						for(int k = 0; k < eleIndex; ++k){
							if(nodeRefer[k] == tempc){
								nodeNo[k] = nodeIndex;
								flag = true;
								break;
							}
						}
						if(!flag){
							nodeRefer[eleIndex] = tempc;
							nodeNo[eleIndex] = nodeIndex;
							eleIndex++;
						}
						nodeIndex++;
					}
				}
			}

			for(int i = 0; i < nodeIndex; ++i){
				bool flag = false;
				for(int j = 0; j < eleIndex; ++j){
					if(i == nodeNo[j]){
						flag = true;
						break;
					}
				}
				if(!flag){
					bool nodeflag = false;
					for(int j = 0; j < eleIndex; ++j){
						if(nodeRefer[j] == self[i]){
							queue<int> nodequeue;
							nodequeue.push(nodeNo[j]);
							int tempnode;
							while(!nodequeue.empty()){
								tempnode = nodequeue.front();
								nodequeue.pop();
								if(tempnode == i){
									nodeflag = true;
									break;
								}
								if(left[tempnode] >= 0){
									nodequeue.push(left[tempnode]);
								}
								if(right[tempnode] >= 0){
									nodequeue.push(right[tempnode]);
								}
							}
							break;
						}
					}
					if(!nodeflag){
						string tempName = STable::getId(EleTable::getRefer(self[i]))+"+0";
						STable::insertEle(EleTable::getRefer(self[i])+1,tempName,EleTable::getType(self[i]),STable::getKind(EleTable::getRefer(self[i])),-1,true,false,-1,-1,STable::getOffset(EleTable::getRefer(self[i]))+1);
						EleTable::moveRefer(EleTable::getRefer(self[i])+1,true);
						FourEleTable::moveRefer(EleTable::getRefer(self[i])+1,true,false);
						moveRefer(EleTable::getRefer(self[i])+1,true,false);
						EleTable::insertEle(self[i]+1,EleTable::getType(self[i]),true,-1,"",EleTable::getRefer(self[i])+1);
						FourEleTable::moveRefer(self[i]+1,true,true);
						moveRefer(self[i]+1,true,true);
						for(int j = 0; j < eleIndex; ++j){
							if(nodeRefer[j] > self[i]){
								nodeRefer[j]++;
							}
						}
						for(int j = 0; j < nodeIndex; ++j){
							if(self[j] > self[i]){
								self[j]++;
							}
						}
						addEle(4,self[i],-1,self[i]+1);
						nodeRefer[eleIndex] = self[i] + 1;
						nodeNo[eleIndex] = i;
						eleIndex++;
					}
				}
			}

			while(1){
				for(int i = 0; i < nodeIndex; ++i){
					nodes[i] = true;
				}
				for(int i = 0; i < nodeIndex; ++i){
					bool flag = false;
					for(int j = 0; j < gfi; ++j){
						if(genFour[j] == i){
							nodes[i] = false;
							flag = true;
							break;
						}
					}
					if(flag){
						continue;
					}
					if(left[i] >= 0){
						nodes[left[i]] = false;
					}
					if(right[i] >= 0){
						nodes[right[i]] = false;
					}
				}
				bool flag = false;
				for(int i = 0; i < nodeIndex; ++i){
					if(nodes[i]){
						flag = true;
						genFour[gfi++] = i;
						while(left[i] >= 0){
							bool tempf = false;
							for(int j = 0; j < gfi; ++j){
								if(left[i] == genFour[j]){
									tempf = true;
									break;
								}
							}
							for(int j = 0; j < nodeIndex; ++j){
								bool tf[2];
								tf[0] = false;
								tf[1] = false;
								for(int k = 0; k < gfi; ++k){
									if(j == genFour[k]){
										tf[0] = true;
										break;
									}
								}
								if(tf[0]){
									continue;
								}
								if(left[j] == left[i] || right[j] == left[i]){
									tempf = true;
									break;
								}
							}
							if(tempf){
								break;
							}
							i = left[i];
							genFour[gfi++] = i;
						}
						break;
					}
				}
				if(!flag){
					break;
				}
			}
			for(int i = gfi-1; i >= 0; --i){
				if(oper[genFour[i]] >= 0){
					flag = false;
					for(int j = 0; j < eleIndex; ++j){
						if(nodeNo[j] == left[genFour[i]]){
							if(!flag){
								tempa = nodeRefer[j];
								flag = true;
							}
							else if(EleTable::getIsVar(nodeRefer[j]) && !STable::getIsTemp(EleTable::getRefer(nodeRefer[j]))){
								tempa = nodeRefer[j];
							}
						}
					}
					if(!flag){
						tempa = self[left[genFour[i]]];
					}
					if(oper[genFour[i]] != 5){
						flag = false;
						for(int j = 0; j < eleIndex; ++j){
							if(nodeNo[j] == right[genFour[i]]){
								if(!flag){
									tempb = nodeRefer[j];
									flag = true;
								}
								else if(EleTable::getIsVar(nodeRefer[j]) && !STable::getIsTemp(EleTable::getRefer(nodeRefer[j]))){
									tempb = nodeRefer[j];
								}
							}
						}
						if(!flag){
							tempb = self[right[genFour[i]]];
						}
					}
					flag = false;
					for(int j = 0; j < eleIndex; ++j){
						if(nodeNo[j] == genFour[i] && EleTable::getIsVar(nodeRefer[j]) && !STable::getIsTemp(EleTable::getRefer(nodeRefer[j]))){
							flag = true;
							addEle(oper[genFour[i]], tempa, tempb, nodeRefer[j]);
						}
					}
					if(!flag){
						for(int j = 0; j < eleIndex; ++j){
							if(nodeNo[j] == genFour[i]){
								addEle(oper[genFour[i]], tempa, tempb, nodeRefer[j]);
								break;
							}
						}
					}
				}
				else{
					for (int j = 0; j < eleIndex; ++j)
					{
						if(nodeNo[j] == genFour[i] && nodeRefer[j] != self[genFour[i]] && EleTable::getIsVar(nodeRefer[j]) && !STable::getIsTemp(EleTable::getRefer(nodeRefer[j]))){
							addEle(4, self[genFour[i]], -1, nodeRefer[j]);
						}
					}
				}
			}
			head = tail;
		}
	}
	for(int i = 0; i < tabIndex; ++i){
		int tempFct = newFETab[i].fct;
		if(6 <= tempFct && 12 >= tempFct || 16 == tempFct || 17 == tempFct){
			for(int j = 0; j < iindex; ++j){
				if(oldIndex[j] == newFETab[i].c){
					newFETab[i].c = newIndex[j];
					break;
				}
			}
		}
	}
}

void Optimise::moveRefer(int ref, bool dir, bool cause){
	if(dir){
		for(int j = 0; j < tabIndex; ++j){
			if((newFETab[j].a >= ref && cause && newFETab[j].fct != 13 && newFETab[j].fct != 15 && newFETab[j].fct != 17 && newFETab[j].fct != 24 && newFETab[j].fct != 19)
				|| (newFETab[j].a >= ref && !cause && (newFETab[j].fct == 13 || newFETab[j].fct == 15 || newFETab[j].fct == 24))){
					++newFETab[j].a;
			}
			if(newFETab[j].b >= ref && cause && newFETab[j].fct != 14){
				++newFETab[j].b;
			}
			if(newFETab[j].c >= ref && cause && (0 <= newFETab[j].fct && 5 >= newFETab[j].fct || 21 == newFETab[j].fct || 25 == newFETab[j].fct || 26 == newFETab[j].fct || 20 == newFETab[j].fct)){
				++newFETab[j].c;
			}
		}
	}
	else{
		for(int j = 0; j < tabIndex; ++j){
			if((newFETab[j].a > ref && cause && newFETab[j].fct != 13 && newFETab[j].fct != 15 && newFETab[j].fct != 17 && newFETab[j].fct != 24 && newFETab[j].fct != 19)
				|| (newFETab[j].a > ref && !cause && (newFETab[j].fct == 13 || newFETab[j].fct == 15 || newFETab[j].fct == 24))){
					--newFETab[j].a;
			}
			if(newFETab[j].b > ref && cause && newFETab[j].fct != 14){
				--newFETab[j].b;
			}
			if(newFETab[j].c > ref && cause && (0 <= newFETab[j].fct && 5 >= newFETab[j].fct || 21 == newFETab[j].fct || 25 == newFETab[j].fct || 26 == newFETab[j].fct || 20 == newFETab[j].fct)){
				--newFETab[j].c;
			}
		}
	}
}

void Optimise::peephole(){
	for(int i = 1; i < tabIndex; ++i){
		if(newFETab[i].fct == 19 && newFETab[i-1].fct == 19){
			remEle(i);
		}
	}
}

void Optimise::remEle(int index){
	for(int i = 0; i < tabIndex; ++i){
		if((newFETab[i].fct >= 6 && newFETab[i].fct <= 12) || newFETab[i].fct == 17 || newFETab[i].fct == 16){
			if(newFETab[i].c >= index){
				newFETab[i].c--;
			}
		}
	}
	for(int i = index; i < tabIndex - 1; ++i){
		newFETab[i].fct = newFETab[i+1].fct;
		newFETab[i].a = newFETab[i+1].a;
		newFETab[i].b = newFETab[i+1].b;
		newFETab[i].c = newFETab[i+1].c;
	}
	tabIndex--;
}

void Optimise::GloAllo(){
	int fct;
	int tfct;
	int fours;
	int fourt;
	int eles;
	int elet;
	int sortArray[8];
	int eleCount;
	for(int i = 0; i < tabIndex; ++i){
		eleCount = 0;
		fct = newFETab[i].fct;
		if(24 == fct){
			fours = i;
			eles = newFETab[i].a + 1;
			for(int j = i + 1; j < tabIndex; ++j){
				if(newFETab[j].fct == 23){
					fourt = j;
					for(int k = eles; k < STable::getIndex(); ++k){
						if(STable::getId(k) == "main"){
							elet = k;
							break;
						}
					}
					break;
				}
				if(newFETab[j].fct == 24){
					fourt = j;
					elet = newFETab[j].a;
					break;
				}
			}
		}
		else if(23 == fct){
			fours = i;
			for(int k = 0; k < STable::getIndex(); ++k){
				if(STable::getId(k) == "main"){
					eles = k + 1;
					break;
				}
			}
			fourt = tabIndex;
			elet = STable::getIndex();
		}
		else continue;
		for(int j = fours; j < fourt; ++j){
			tfct = newFETab[j].fct;
			if(tfct < 6 || (tfct > 6 && tfct < 13) || tfct == 14 || tfct == 16 || tfct == 18 || tfct == 26 || tfct == 22){
				if(EleTable::getIsVar(newFETab[j].a) && !STable::getIsGlo(EleTable::getRefer(newFETab[j].a))){
					STable::addUseCount(EleTable::getRefer(newFETab[j].a),1);
				}
			}
			if(tfct < 6 || tfct == 20 || tfct == 21 || tfct == 25){
				if(EleTable::getIsVar(newFETab[j].c) && !STable::getIsGlo(EleTable::getRefer(newFETab[j].c))){
					STable::addUseCount(EleTable::getRefer(newFETab[j].c),1);
				}
			}
			if(tfct < 4 || (tfct > 6 && tfct < 13) || tfct == 17 || tfct == 25 || tfct == 26){
				if(EleTable::getIsVar(newFETab[j].b) && !STable::getIsGlo(EleTable::getRefer(newFETab[j].b))){
					STable::addUseCount(EleTable::getRefer(newFETab[j].b),1);
				}
			}
		}
		for(int j = fours; j < fourt; ++j){
			if(newFETab[j].fct == 6 && newFETab[j].c < j){
				for(int k = newFETab[j].c; k < j; ++k){
					tfct = newFETab[k].fct;
					if(tfct < 6 || (tfct > 6 && tfct < 13) || tfct == 14 || tfct == 16 || tfct == 18 || tfct == 26 || tfct == 22){
						if(EleTable::getIsVar(newFETab[k].a) && !STable::getIsGlo(EleTable::getRefer(newFETab[k].a))){
							STable::addUseCount(EleTable::getRefer(newFETab[k].a),5);
						}
					}
					if(tfct < 6 || tfct == 20 || tfct == 21 || tfct == 25){
						if(EleTable::getIsVar(newFETab[k].c) && !STable::getIsGlo(EleTable::getRefer(newFETab[k].c))){
							STable::addUseCount(EleTable::getRefer(newFETab[k].c),5);
						}
					}
					if(tfct < 4 || (tfct > 6 && tfct < 13) || tfct == 17 || tfct == 25 || tfct == 26){
						if(EleTable::getIsVar(newFETab[k].b) && !STable::getIsGlo(EleTable::getRefer(newFETab[k].b))){
							STable::addUseCount(EleTable::getRefer(newFETab[k].b),5);
						}
					}
				}
			}
		}
		int k,kk;
		for(int j = eles; j < elet; ++j){
			if(STable::getType(j) != ARRAY && STable::getKind(j) == VARIABLE){
				for(k = 0; k < 8 && k < eleCount; ++k){
					if(STable::getUseCount(j) > STable::getUseCount(sortArray[k])){
						break;
					}
				}
				kk = (eleCount >= 8) ? 7 : eleCount;
				for(; kk > k; kk--){
					sortArray[kk] = sortArray[kk - 1];
				}
				if(k < 8){
					sortArray[k] = j;
				}
				eleCount++;
			}
		}
		for(int j = 0; j < 8 && j < eleCount; ++j){
			STable::setGloReg(sortArray[j], j);
		}
		if(fct == 24){
			kk = (eleCount > 8) ? 8 : eleCount;
			for(int j = eles; j < elet; ++j){
				if(STable::getKind(j) == VARIABLE){
					STable::moveOffset(j, kk);
				}
			}
			int funcindex = STable::getRef(eles - 1);
			FuncTable::setLen(funcindex, FuncTable::getLen(funcindex) + kk);
			FuncTable::setSRnum(funcindex, kk);
		}
		i = fourt - 1;
	}
}