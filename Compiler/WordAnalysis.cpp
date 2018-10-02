
#include "WordAnalysis.h"

const int norw = 13, imaxl = 9, nmaxl = 20;
const string rw[norw] = { "case", "char", "const", "default", "if", "int", "main",
"printf", "return", "scanf", "switch", "void", "while" };
/*const string rwsym[norw] = { "CASESY", "CHARSY", "CONSY", "DEFSY", "IFSY", "INTSY",
"MAINSY", "PRISY", "RETSY", "SCASY", "SWISY", "VOIDSY", "WHISY" };*/
ifstream incode;
ofstream outdata("data.txt");
WordAnalysis::WordAnalysis()
{
	ch = ' ';
	fileEnd = false;
	pline = 0;
	preCountLine = false;
	lineCount = 0;
	word = "";
	sym = -1;
	line = "";
	string temp;
	cin>>temp;
	incode.open(temp);
	while(line == "" && incode){
		getline(incode, line);
	}
	lline = line.length();
}
bool WordAnalysis::isalpha(char ch){
	if (('a' <= ch && 'z' >= ch) || ('A' <= ch && 'Z' >= ch) || '_' == ch){
		return true;
	}
	return false;
}

bool WordAnalysis::isdigit(char ch){
	if ('0' <= ch && '9' >= ch){
		return true;
	}
	return false;
}

bool WordAnalysis::isstrmem(char ch){
	if (32 == ch || 33 == ch || (35 <= ch && 126 >= ch)){
		return true;
	}
	return false;
}

void WordAnalysis::getch(){
	if (preCountLine){
		preCountLine = false;
		pline = 0;
		lineCount++;
	}
	if (pline >= lline){
		lastline = line;
		lastpline = pline;
		if (getline(incode, line)){
			lline = line.length();
			preCountLine = true;
			pline++;
			ch = '\n';
			return;
			//cout<<line<<endl;
		}
		else{
			if (fileEnd){
				ErrorHandle::Fatal(0);
			}
			fileEnd = true;
			return;
		}
	}
	if(pline < lline){
		ch = line[pline++];
	}
	else ch = '\0';
}

int WordAnalysis::getsym(){
	int len, i, j, k;
	word = "";
	if (isalpha(ch)){
		len = 0;
		int reported = false;
		do{
			if (len < nmaxl){
				word.append(1, ch);
			}
			else if(!reported){
				reported = true;
				ErrorHandle::Error(0,lineCount,line,pline-1);
			}
			len++;
			getch();
		} while (isalpha(ch) || isdigit(ch));
		i = 0;
		j = norw;
		bool found = false;
		while (i < j){
			int comre;
			k = (i + j) / 2; 
			comre = rw[k].compare(word);
			if (comre == 0){
				found = true;
				break;
			}
			if (comre > 0){
				j = k;
			}
			else{
				i = k + 1;
			}
		}
		if (found){
			sym = k;
		}
		else{
			sym = IDENT;
		}
	}
	else if (isdigit(ch)){
		int chmark = pline;
		len = 0;
		sym = INTEGER;
		while (isdigit(ch)){
			len++;
			if(len < imaxl){
				word.append(1, ch);
			}
			getch();
		}
		if(len > imaxl){
			if(preCountLine){
				ErrorHandle::Error(1,lineCount,lastline,chmark);
			}
			else{
				ErrorHandle::Error(1,lineCount,line,chmark);
			}
		}
		if (word[0] == '0' && word.size() > 1){
			if(preCountLine){
				ErrorHandle::Error(2,lineCount,lastline,chmark);
			}
			else{
				ErrorHandle::Error(2,lineCount,line,chmark);
			}
		}
	}
	else if (ch == '<'){
		getch();
		if (ch == '='){
			sym = LESEQ;
			word = "<=";
			getch();
		}
		else{
			sym = LESS;
			word = "<";
		}
	}
	else if (ch == '>'){
		getch();
		if (ch == '='){
			sym = GRTEQ;
			word = ">=";
			getch();
		}
		else{
			sym = GREATER;
			word = ">";
		}
	}
	else if (ch == '='){
		getch();
		if (ch == '='){
			sym = EQUAL;
			word = "==";
			getch();
		}
		else{
			sym = BECOME;
			word = "=";
		}
	}
	else if (ch == '!'){
		word.append(1, ch);
		getch();
		if (ch == '='){
			sym = NOTEQ;
			word = "!=";
			getch();
		}
		else{
			if(preCountLine){
				ErrorHandle::Error(3,lineCount,lastline,lastpline);
			}
			else{
				ErrorHandle::Error(3,lineCount,line,pline);
			}
		}
	}
	else if (ch == '\''){
		word.append(1, ch);
		getch();
		if ((ch == '+' || ch == '-' || ch == '*' || ch == '/'
			|| isalpha(ch) || isdigit(ch))){
			word.append(1, ch);
			getch();
			if (ch == '\''){
				word.append(1, ch);
				sym = CHAR;
				getch();
			}
			else{
				if(preCountLine){
					ErrorHandle::Error(4,lineCount,lastline,lastpline);
				}
				else{
					ErrorHandle::Error(4,lineCount,line,pline);
				}
			}
		}
		else{
			if(preCountLine){
				ErrorHandle::Error(3,lineCount,lastline,lastpline);
			}
			else{
				ErrorHandle::Error(3,lineCount,line,pline);
			}
			getch();
			if(ch == '\''){
				getch();
			}
			else{
				if(preCountLine){
					ErrorHandle::Error(4,lineCount,lastline,lastpline);
				}
				else{
					ErrorHandle::Error(4,lineCount,line,pline);
				}
			}
		}
	}
	else if (ch == '\"'){
		word.append(1, ch);
		getch();
		while (isstrmem(ch)){
			word.append(1, ch);
			if(ch == '\\'){
				word.append(1,ch);
			}
			getch();
		}
		if (ch == '\"'){
			word.append(1, ch);
			sym = STRING;
			getch();
		}
		else{
			if(preCountLine){
				ErrorHandle::Error(5,lineCount,lastline,lastpline);
			}
			else{
				ErrorHandle::Error(5,lineCount,line,pline);
			}
		}
	}
	else if (ch == '+'){
		word.append(1, ch);
		getch();
		sym = PLUS;
	}
	else if (ch == '-'){
		word.append(1, ch);
		getch();
		sym = MINUS;
	}
	else if (ch == '*'){
		word.append(1, ch);
		getch();
		sym = MULTI;
	}
	else if (ch == '/'){
		word.append(1, ch);
		getch();
		sym = DIVIDE;
	}
	else if (ch == ';'){
		word.append(1, ch);
		getch();
		sym = SEMICOLON;
	}
	else if (ch == ','){
		word.append(1, ch);
		getch();
		sym = COMMA;
	}
	else if (ch == '['){
		word.append(1, ch);
		getch();
		sym = LBRACK;
	}
	else if (ch == ']'){
		word.append(1, ch);
		getch();
		sym = RBRACK;
	}
	else if (ch == '('){
		word.append(1, ch);
		getch();
		sym = LPARENT;
	}
	else if (ch == ')'){
		word.append(1, ch);
		getch();
		sym = RPARENT;
	}
	else if (ch == '{'){
		word.append(1, ch);
		getch();
		sym = LBRACE;
	}
	else if (ch == '}'){
		word.append(1, ch);
		getch();
		sym = RBRACE;
	}
	else if (ch == ':'){
		word.append(1, ch);
		getch();
		sym = COLON;
	}
	else if(ch == '\n'){
		word.append(1, ch);
		getch();
		sym = NEWLINE;
	}
	else{
		word.append(1, ch);
		getch();
		sym = UNKNOWN;
		if(preCountLine){
			ErrorHandle::Error(3,lineCount,lastline,lastpline);
		}
		else{
			ErrorHandle::Error(3,lineCount,line,pline-1);
		}
	}
	while (ch == ' ' || ch == '\t' || ch == '\0'){
		getch();
	}
	return sym;
}

string WordAnalysis::getWord(){
    return word;
}

int WordAnalysis::getLineCount(){
    return lineCount;
}

int WordAnalysis::getPline(){
	return pline;
}

string WordAnalysis::getLine(){
	return line;
}

bool WordAnalysis::getPCL(){
	return preCountLine;
}