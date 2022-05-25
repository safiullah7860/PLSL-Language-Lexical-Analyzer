#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include "lex.h"
#include <cctype>
#include <map>
using namespace std;
static map <Token,string> biggerMap ={
    //int, real, and string constant as defined in PA1:
    {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"},   
    //identifiers listed in PA1:
    {IDENT, "IDENT"},
    //+, -, *, /, :=, (, ), =, >, <.  :
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"},{DIV, "DIV"},{ASSOP, "ASSOP"}, {LPAREN, "LPAREN"}, 
    {RPAREN, "RPAREN"}, {EQUAL, "EQUAL"},  {COMMA, "COMMA"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {SEMICOL, "SEMICOL"},   
    {PLUS, "PLUS"}, {COLON, "COLON"}, {THEN, "THEN"},  
    //keywords listed in PA1 description: 
    {PROGRAM, "PROGRAM"}, {END, "END"}, {BEGIN, "BEGIN"}, {WRITELN, "WRITELN"}, {IF, "IF", }, {INTEGER, "INTEGER"}, 
    {REAL, "REAL"}, { STRING,"STRING"}, {FOR, "FOR"}, {TO, "TO"}, {DO, "DO"}, {DOWNTO, "DOWNTO"}, 
    {ELSE, "ELSE"}, {VAR, "VAR"},
    //error:
    {ERR, "ERR"},
    //done:
    {DONE, "DONE"} 
};
ostream& operator<<(ostream& out, const LexItem& tok) {
  Token token = tok.GetToken();
  int k = tok.GetLinenum();
  k++;
  if(token == SCONST || token == IDENT || token == RCONST || token== ICONST)
    {out<<biggerMap[token];
    out <<"(" << tok.GetLexeme() << ")";} 
  else out<<biggerMap[token];
  return out;
}
int main(int argc, char* argv[]) {
int counter = 0;
set<int> iconst; set<string> rconst; set<string> sconst; set<string> ident;
bool vbool = false; bool iconstbool = false; bool rconstbool = false;bool sconstbool = false; bool identbool = false; 
string eachLine, file;
string currentFlag = "null";
int linenumber=0;
int numOfTokens = 0;
if (argc==1) {cout <<"NO SPECIFIED INPUT FILE NAME.\n";}
    else {
        file = argv[1];
        ifstream myFile;
        myFile.open(file, ios::in);
        if (!myFile.is_open()) cout << "CANNOT OPEN the File cantopen\n";
        if (myFile.is_open()) {
            LexItem tok;
            for (int r = 1; r<argc; r++) {
                string arg(argv[r]);
                //if (r==3){cout<<"ONLY ONE FILE NAME ALLOWED.\n"; return 0; }
                if (arg=="-v") { //created currentflag to check if its not 
                    vbool = true;
                }
                else if (arg=="-iconst") {
                    iconstbool = true;
                }
                else if (arg=="-rconst"){
                    rconstbool = true;
                }
                else if (arg=="-sconst"){
                    sconstbool = true;
                }
                else if (arg=="-ident"){
                    identbool = true;
                }
                else if (arg[0] != '-') counter++;
                    
                else {cout<< "UNRECOGNIZED FLAG "<< string(argv[r])<<endl; return 0;}
            }
            if (counter>1) {cout<<"ONLY ONE FILE NAME ALLOWED.\n"; return 0;}
            while(((tok = getNextToken(myFile, linenumber)) != ERR )&&( tok != DONE)){
                        numOfTokens++;
                        if (vbool==1) {cout<<tok<<endl;}
                        if (iconstbool==1) {
                            if (tok==ICONST) {
                                int z = 0;
                                string saf = tok.GetLexeme();
                                stringstream sstr(saf);
                                sstr >> z;
                                iconst.insert(z); } 
                        }
                        if (rconstbool==1) {
                            if (tok.GetLexeme()=="0.0") {rconst.insert("0");} 
                            else if (tok==RCONST) {rconst.insert(tok.GetLexeme());} 
                        }
                        if (sconstbool) {if (tok==SCONST) {sconst.insert(tok.GetLexeme());} }
                        if (identbool) {if (tok==IDENT) {ident.insert(tok.GetLexeme());} }
                    }
            if(tok == ERR && tok.GetLexeme()!= "" ) 
                {cout << "Error in line " <<tok.GetLinenum() +1<< " (" <<tok.GetLexeme()<<")\n"; return 0;}
            cout<<"Lines: "<<linenumber<<endl;
            if (!linenumber==0) { cout<<"Tokens: "<<numOfTokens<<endl; }
            if (sconstbool==1) {
                    cout<<"STRINGS:"<<endl;
                    for (auto p = sconst.begin(); p !=sconst.end(); ++p)
                        cout << '\'' << *p <<'\''<<endl;
            }
           if (iconstbool==1) {
                    cout<<"INTEGERS:";
                    for (auto itr = iconst.begin(); itr !=iconst.end(); ++itr)
                        cout << '\n'<<*itr;
                    cout<<endl;
            }
            if (rconstbool==1) {
                    cout<<"REALS:";
                    for (auto iter = rconst.begin(); iter !=rconst.end(); ++iter)
                        cout << '\n'<<*iter; 
                    cout<<endl;
            }
            if (identbool==1) {
                    int count = 0;
                    cout<<"IDENTIFIERS:"<<endl;
                    for (auto y = ident.begin(); y !=ident.end(); ++y) {
                        if (count == 0) {
                        cout << *y;
                        count++;
                        }    
                        else {
                            cout << ", " << *y;
                        }
                    }
                    cout<<endl;
            }
            }
        }
}
