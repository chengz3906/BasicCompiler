#include "ErrorHandle.h"


bool ErrorHandle::haveError;
ErrorHandle::ErrorHandle(void)
{
	haveError = false;
}

void ErrorHandle::Fatal(int no){
	switch(no){
	case 0:
		cout << "Incomplete code! No legal main function or incompatible brace number"<<endl;
		break;
	case 1:
		cout << "array table overflow"<<endl;
		break;
	case 2:
		cout << "token table overflow"<<endl;
		break;
	case 3:
		cout << "original code table overflow"<<endl;
		break;
	case 4:
		cout << "element table overflow"<<endl;
		break;
	case 5:
		cout << "four-element expression table overflow"<<endl;
		break;
	case 6:
		cout << "function table overflow"<<endl;
		break;
	case 7:
		cout << "string table overflow"<<endl;
		break;
	case 8:
		cout << "symbol table overflow"<<endl;
		break;
	case 9:
		cout << "jump label table overflow"<<endl;
		break;
	}
	cin>>no;
	exit(0);
}

void ErrorHandle::Error(int no, int linecount, string line, int chno){
	haveError = true;
	for(int i = 0; i < line.size(); ++i){
		if(line[i] != ' ' && line[i] != '\t'){
			line = line.substr(i,line.size()-i);
			chno -= i+1;
			break;
		}
	}
	cout<<"Error in line"<<linecount+1<<":"<<endl<<line<<endl;
	for(int i = 0; i < chno; ++i){
		cout<<" ";
	}
	cout<<"^    ";
	switch(no){
	case 0:
		cout << "identity too long"<<endl;
		break;
	case 1:
		cout <<"integer too long"<<endl;
		break;
	case 2:
		cout<<"integer started with 0"<<endl;
		break;
	case 3:
		cout<<"illegal input"<<endl;
		break;
	case 4:
		cout<<"char without right quotation mark"<<endl;
		break;
	case 5:
		cout<<"string without right quotation mark"<<endl;
		break;
	case 6:
		cout<<"expected integer"<<endl;
		break;
	case 7:
		cout<<"expected int or char"<<endl;
		break;
	case 8:
		cout<<"expected identity"<<endl;
		break;
	case 9:
		cout<<"expected become mark"<<endl;
		break;
	case 10:
		cout<<"expected character"<<endl;
		break;
	case 11:
		cout<<"repeated identity name"<<endl;
		break;
	case 12:
		cout<<"expected semicolon"<<endl;
		break;
	case 13:
		cout<<"nested function declaration"<<endl;
		break;
	case 14:
		cout<<"expected right bracket"<<endl;
		break;
	case 15:
		cout<<"divisor equals 0"<<endl;
		break;
	case 16:
		cout<<"expected right parent"<<endl;
		break;
	case 17:
		cout<<"illegal factor"<<endl;
		break;
	case 18:
		cout<<"expected left parent"<<endl;
		break;
	case 19:
		cout<<"expected comma"<<endl;
		break;
	case 20:
		cout<<"too many parameters"<<endl;
		break;
	case 21:
		cout<<"undeclared identity or incompatible type"<<endl;
		break;
	case 22:
		cout<<"assign value to non-variable"<<endl;
		break;
	case 23:
		cout<<"return type incompatible with declared type"<<endl;
		break;
	case 24:
		cout<<"expected left brace"<<endl;
		break;
	case 25:
		cout<<"no case statement in switch statement"<<endl;
		break;
	case 26:
		cout<<"expected colon"<<endl;
		break;
	case 27:
		cout<<"empty followed by case or default"<<endl;
		break;
	case 28:
		cout<<"expected right brace"<<endl;
		break;
	case 29:
		cout<<"illegal case statement"<<endl;
		break;
	case 30:
		cout<<"using return value from void function"<<endl;
		break;
	case 31:
		cout<<"illegal statement"<<endl;
		break;
	case 32:
		cout<<"no return statement in non-void function"<<endl;
		break;
	case 33:
		cout<<"no parameter in function call"<<endl;
		break;
	}
}

bool ErrorHandle::getHaveError(){
	return haveError;
}

void ErrorHandle::setHaveError(){
	haveError = true;
}