#include "lex.h"
#include <string>
#include <iostream>
#include <map>
#include <algorithm> 
#include <cctype>
using namespace std;
//I am building two maps, one with keywords, and one with every token possible (with respect to PA1 desription)
char newLine = '\n';
bool reddit = false;
static map <string,Token> smallMap={ 
    //keywords listed in PA1 description: 
    {"PROGRAM", PROGRAM}, {"END", END}, {"BEGIN", BEGIN}, {"WRITELN", WRITELN}, {"IF", IF}, {"INTEGER", INTEGER}, 
    {"REAL", REAL}, {"STRING", STRING}, {"FOR", FOR}, {"TO", TO}, {"DOWNTO", DOWNTO}, {"DO", DO},  {"ELSE", ELSE}, {"VAR", VAR}, {"THEN", THEN},  
};
static map <Token,string> bigMap ={
    {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"},   
    {IDENT, "IDENT"}, 
    //+, -, *, /, :=, (, ), =, >, <.  :
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"},{DIV, "DIV"},{ASSOP, "ASSOP"}, {DO, "DO"}, {LPAREN, "LPAREN"}, 
    {RPAREN, "RPAREN"}, {EQUAL, "EQUAL"},  {COMMA, "COMMA"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {SEMICOL, "SEMICOL"},   
    {PLUS, "PLUS"}, {COLON, "COLON"}, {THEN, "THEN"},  
    //keywords listed in PA1 description: 
    {PROGRAM, "PROGRAM"}, {END, "END"}, {BEGIN, "BEGIN"}, {WRITELN, "WRITELN"}, {IF, "IF", }, {INTEGER, "INTEGER"}, 
    {REAL, "REAL"}, { STRING,"STRING"}, {FOR, "FOR"}, {TO, "TO"}, {DOWNTO, "DOWNTO"}, 
    {ELSE, "ELSE"}, {VAR, "VAR"},
    //error:
    {ERR, "ERR"},
    //done:
    {DONE, "DONE"} 
};

    LexItem id_or_kw(const string& lexeme, int linenum) {
//accepts a reference to a string of a lexeme and a line number
//returns a LexItem object containing either the keyword Token if it is found, or IDENT if not
    Token token = IDENT; //identifier
    auto safi = (smallMap.find(lexeme));
    if (safi!=smallMap.end())///if it doesnt reach end of smallMap (keywords)
        token = smallMap[lexeme];
    return LexItem(token, lexeme, linenum);
    
}
LexItem getNextToken (istream& in, int& linenumber) {
    enum TokState {START, INID, INSTRING, ININT, INREAL, INCOMMENT} ;
    TokState lexstate = START;
    string lexeme;
    char chrct;
    bool charstar= false; 
    bool sl= false; 
while(in.get(chrct)){ 
    if(chrct == EOF){ // if it reaches end of file
         return LexItem(DONE, "", 0);
    }
    switch (lexstate){
      
      case START:
        if (chrct=='\n') {
            linenumber++;
        }
        if(isspace(chrct))
          continue;
          lexeme = chrct;
        
        if(isalpha(chrct)|| (chrct=='_')) lexstate = INID;
        else if(chrct=='"'){
          lexstate = INSTRING;
          lexeme.clear();    
        } 
        else if(chrct == '(') {
            if(in.peek()=='*') 
            {
                lexstate = INCOMMENT; 
                charstar = true;
                continue; }
            return LexItem(LPAREN, lexeme, linenumber);    
        }
        else if (chrct == ')') {
            if (charstar==1) {
                charstar = false;
                continue;
            }
            return LexItem(RPAREN, lexeme, linenumber);
            //break;
        }
        else if(isdigit(chrct)) {lexstate = ININT; }
        else if (chrct == '.') {lexeme = ""; lexeme += '0'; lexeme+= chrct; lexstate= INREAL;}
        else if(chrct == '+') {return LexItem(PLUS, lexeme, linenumber); } 
        else if(chrct == '-') { return LexItem(MINUS, lexeme, linenumber);  } 
        else if(chrct == '/') {return LexItem(DIV, lexeme, linenumber);}
        else if(chrct == '\'') {lexstate = INSTRING; sl = false; }
        else if(chrct == ',') {return LexItem(COMMA, lexeme, linenumber); } 
        else if(chrct == '_' || chrct=='?') {return LexItem(ERR, lexeme, linenumber); } 
        else if(chrct == '*') { return LexItem(MULT, lexeme, linenumber); }  
        else if(chrct == '>') { return LexItem(GTHAN, lexeme, linenumber); }//gthan
        else if(chrct == '<') { return LexItem(LTHAN, lexeme, linenumber); }//lthan
        else if(chrct == ';') {return LexItem(SEMICOL, lexeme, linenumber); } //semicolon
        else if(chrct == '='){  
          return LexItem(EQUAL, lexeme, linenumber);
          continue;
        }
        else if(chrct == ':'){  
            if(in.peek() == '=') {
                in.get(chrct);
                lexeme += chrct;
            return LexItem(ASSOP, lexeme, linenumber);
            }
            else return LexItem(COLON, lexeme, linenumber);
        }
        break;
      
      case ININT:
            if(isdigit(chrct)) {lexeme +=chrct; continue;}
            else if(chrct == '.'){
                lexeme += chrct;
                lexstate = INREAL;
                if(in.peek()==' ') return LexItem(ERR, lexeme, linenumber);
                continue;} 
            else {
                lexstate = START;
                in.putback(chrct);
                return LexItem(ICONST, lexeme, linenumber);}
        break; 

      case INCOMMENT: 
        reddit = true;
        if ((in.peek()==')')&&(chrct == '*')) {
            reddit = false;
            lexstate = START;
        }
        else if (chrct=='\n') {
            linenumber++;
        }
    break;
    
      case INID:
        if(isalpha(chrct) || isdigit(chrct)||chrct =='_'){
            lexeme += chrct;
            continue;}
        else if(chrct == '.' ){ //if it is a period
            if(isalpha(in.peek()))
               {
                    lexeme = "";
                    lexeme+=chrct;
                    lexeme+=in.peek();
                   return LexItem(ERR, lexeme, linenumber);
               }
               else return LexItem(ERR, lexeme, linenumber); 
        } 
        else {
          in.putback(chrct);
          string upper = lexeme;
          for (int y= 0; y<upper.length(); y++)
            upper[y] = toupper(upper[y]); 
           return id_or_kw(upper, linenumber);
        }
        break; 

      case INSTRING:
                if(sl){
                    sl = false;
                    lexeme += chrct;
                    break;
                }
                if(chrct == '\n'){
                    lexeme = lexeme.substr(0, lexeme.length());
                    return LexItem(ERR, lexeme, linenumber);
                }
                if(chrct == '\\'){ sl = true; break; }
                lexeme += chrct;
                if(chrct == '\''){
                    lexeme = lexeme.substr(1, lexeme.length()-2);
                    return LexItem(SCONST, lexeme, linenumber);
                }
            break;
      case INREAL: 
        if(isdigit(chrct)){
           lexeme += chrct;  
            if(!isdigit(in.peek())&&(in.peek()=='.')){
                lexeme+=in.peek();
                lexstate= START;
                return LexItem(ERR, lexeme, linenumber);
            }
        }
        else {
            in.putback(chrct);
            //lexeme +=chrct;
            return LexItem(RCONST, lexeme, linenumber);
        }
        break;
    }
  }
  if (reddit==1) {cout << endl << "Missing a comment end delimiters '*)' at line " << linenumber << endl; }
  return LexItem();
}