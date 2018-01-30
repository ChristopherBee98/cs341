// File: Sally.cpp
//
// CMSC 341 Spring 2017 Project 2
//
// Implementation of member functions of Sally Forth interpreter
//

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>
using namespace std ;

#include "Sally.h"


// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
  m_kind = kind ;
  m_value = val ;
  m_text = txt ;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
  m_kind = kind ;
  m_value = val ;
  m_dothis = fptr ;
}


// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) :
  istrm(input_stream)  // use member initializer to bind reference
{

  symtab["DUMP"]    =  SymTabEntry(KEYWORD,0,&doDUMP) ;

  symtab["+"]      =  SymTabEntry(KEYWORD, 0, &doPlus) ;
  symtab["-"]      =  SymTabEntry(KEYWORD, 0, &doMinus) ;
  symtab["*"]      =  SymTabEntry(KEYWORD, 0, &doTimes) ;
  symtab["/"]      =  SymTabEntry(KEYWORD, 0, &doDivide) ;
  symtab["%"]      =  SymTabEntry(KEYWORD, 0, &doMod) ;
  symtab["NEG"]    =  SymTabEntry(KEYWORD, 0, &doNEG) ;
  symtab["SET"]    =  SymTabEntry(KEYWORD, 0, &doSET) ;
  symtab["@"]      =  SymTabEntry(KEYWORD, 0, &doAT) ;
  symtab["!"]      =  SymTabEntry(KEYWORD, 0, &doSTORE) ;
  symtab[">"]      =  SymTabEntry(KEYWORD, 0, &doGTHAN) ;
  symtab[">="]     =  SymTabEntry(KEYWORD, 0, &doGTHANEQUAL) ;
  symtab["<"]      =  SymTabEntry(KEYWORD, 0, &doLTHAN) ;
  symtab["<="]     =  SymTabEntry(KEYWORD, 0, &doLTHANEQUAL) ;
  symtab["=="]     =  SymTabEntry(KEYWORD, 0, &doEQUAL) ;
  symtab["!="]     =  SymTabEntry(KEYWORD, 0, &doNOTEQUAL) ;
  symtab["NOT"]    =  SymTabEntry(KEYWORD, 0, &doNOT) ;
  symtab["OR"]     =  SymTabEntry(KEYWORD, 0, &doOR) ;
  symtab["AND"]    =  SymTabEntry(KEYWORD, 0, &doAND) ;
  symtab["IFTHEN"] =  SymTabEntry(KEYWORD, 0, &doIFTHEN) ;
  symtab["ELSE"]   =  SymTabEntry(KEYWORD, 0, &doELSE) ;
  symtab["ENDIF"]  =  SymTabEntry(KEYWORD, 0, &doENDIF) ;
  symtab["DUP"]    =  SymTabEntry(KEYWORD, 0, &doDUP) ;
  symtab["DROP"]   =  SymTabEntry(KEYWORD, 0, &doDROP) ;
  symtab["SWAP"]   =  SymTabEntry(KEYWORD, 0, &doSWAP) ;
  symtab["ROT"]    =  SymTabEntry(KEYWORD, 0, &doROT) ;
  symtab["DO"]     =  SymTabEntry(KEYWORD, 0, &doDO) ;
  symtab["UNTIL"]  =  SymTabEntry(KEYWORD, 0, &doUNTIL) ;

  symtab["."]    =  SymTabEntry(KEYWORD,0,&doDot) ;
  symtab["SP"]   =  SymTabEntry(KEYWORD,0,&doSP) ;
  symtab["CR"]   =  SymTabEntry(KEYWORD,0,&doCR) ;
  m_isDo = false;
  m_isIf = false;
}


// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered 
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
// 
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detects base 10 numbers
// 
//
bool Sally::fillBuffer() {
  string line ;     // single line of input
  int pos ;         // current position in the line
  int len ;         // # of char in current token
  long int n ;      // int value of token
  char *endPtr ;    // used with strtol()


  while(true) {    // keep reading until empty line read or eof

    // get one line from standard in
    //
    getline(istrm, line) ;   

    // if "normal" empty line encountered, return to mainLoop
    //
    if ( line.empty() && !istrm.eof() ) {
      return true ;
    }

    // if eof encountered, return to mainLoop, but say no more
    // input available
    //
    if ( istrm.eof() )  {
      return false ;
    }
    // Process line read

    pos = 0 ;                      // start from the beginning

    // skip over initial spaces & tabs
    //
    while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
      pos++ ; 
    }

    // Keep going until end of line
    //
    while (line[pos] != '\0') {

      // is it a comment?? skip rest of line.
      //
      if (line[pos] == '/' && line[pos+1] == '/') break ;

      // is it a string literal? 
      //
      if (line[pos] == '.' && line[pos+1] == '"') {

            pos += 2 ;  // skip over the ."
            len = 0 ;   // track length of literal

            // look for matching quote or end of line
            //
            while(line[pos+len] != '\0' && line[pos+len] != '"') {
	      len++ ;
            }

            // make new string with characters from
            // line[pos] to line[pos+len-1]
            string literal(line,pos,len) ;  // copy from pos for len chars

            // Add to token list
            //
            tkBuffer.push_back( Token(STRING,0,literal) ) ;  

            // Different update if end reached or " found
            //
            if (line[pos+len] == '\0') {
	      pos = pos + len ;
            } else {
	      pos = pos + len + 1 ;
            }

      } else {  // otherwise "normal" token

	len = 0 ;  // track length of token

	// line[pos] should be an non-white space character
	// look for end of line or space or tab
	//
	while(line[pos+len] != '\0' && line[pos+len] != ' ' && line[pos+len] != '\t') {
	  len++ ;
	}

	string literal(line,pos,len) ;   // copy form pos for len chars
	pos = pos + len ;

	// Try to convert to a number
	//
	n = strtol(literal.c_str(), &endPtr, 10) ;

	if (*endPtr == '\0') {
	  tkBuffer.push_back( Token(INTEGER,n,literal) ) ;
	} else {
	  tkBuffer.push_back( Token(UNKNOWN,0,literal) ) ;
	}
      }

      // skip over trailing spaces & tabs
      //
      while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
	pos++ ; 
      }

    }
  }
}



// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception 
//
Token Sally::nextToken() {
  Token tk ;
  bool more = true ;
  
  while(more && tkBuffer.empty() ) {
    more = fillBuffer() ;
  }

  if ( !more && tkBuffer.empty() ) {
    throw EOProgram("End of Program") ;
  }

  tk = tkBuffer.front() ;
  if (m_isDo == true) {
    doBuffer.push_back(tk);
  }
  tkBuffer.pop_front() ;
  return tk ;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {

  Token tk ;
  map<string,SymTabEntry>::iterator it ;

  try {
    while( 1 ) {
      tk = nextToken() ;

      if (tk.m_kind == INTEGER || tk.m_kind == STRING) {
	// if INTEGER or STRING just push onto stack
	params.push(tk) ;

      } else { 
	it = symtab.find(tk.m_text) ;
            
	if ( it == symtab.end() )  {   // not in symtab

	  params.push(tk) ;

	} else if (it->second.m_kind == KEYWORD)  {

	  // invoke the function for this operation
	  //
	  if (it->second.m_dothis == symtab["IFTHEN"].m_dothis || it->second.m_dothis == symtab["ELSE"].m_dothis) {
	    m_isIf = true;
	    it->second.m_dothis(this);
	  } else {
	    it->second.m_dothis(this) ;
	  }   
               
	} else if (it->second.m_kind == VARIABLE) {

	  // variables are pushed as tokens
	  //
	  tk.m_kind = VARIABLE ;
	  params.push(tk) ;

	} else {

	  // default action
	  //
	  params.push(tk) ;

	}
      }
    }
  } catch (EOProgram& e) {

    cerr << "End of Program\n" ;
    if ( params.size() == 0 ) {
      cerr << "Parameter stack empty.\n" ;
    } else {
      cerr << "Parameter stack has " << params.size() << " token(s).\n" ;
    }

  } catch (out_of_range& e) {

    cerr << "Parameter stack underflow??\n" ;

  } catch (...) {

    cerr << "Unexpected exception caught\n" ;

  }
}

// -------------------------------------------------------


void Sally::doPlus(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for +.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value + p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMinus(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for -.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value - p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doTimes(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for *.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value * p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doDivide(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for /.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value / p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMod(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for %.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value % p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

//first defines a function
//done
void Sally::doSET(Sally *Sptr) {
  Token p1, p2;
  if (Sptr->params.size() < 2) {
    throw out_of_range("Needs two parameters for SET.");
  }
  p1 = Sptr->params.top();
  Sptr->params.pop();
  string name = p1.m_text;
  p2 = Sptr->params.top();
  Sptr->params.pop();
  int value = p2.m_value;
  Sptr->symtab[name] = SymTabEntry(VARIABLE, value);  
}
//done
void Sally::doAT(Sally *Sptr) {
  Token p1;
  if (Sptr->params.size() < 1) {
    throw out_of_range("Needs one parameters for @.");
  }
  p1 = Sptr->params.top();
  Sptr->params.pop();
  Sptr->params.push(Token(INTEGER, Sptr->symtab[p1.m_text].m_value, ""));
}
//changes previously defined variable
//done
void Sally::doSTORE(Sally *Sptr) {
  Token p1, p2;
  if (Sptr->params.size() < 2) {
    throw out_of_range("Needs two parameters for !.");
  }
  p1 = Sptr->params.top();
  Sptr->params.pop();
  string name = p1.m_text;
  p2 = Sptr->params.top();
  Sptr->params.pop();
  int value = p2.m_value;
  Sptr->symtab[name] = SymTabEntry(VARIABLE, value);
}
//done
void Sally::doGTHAN(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for >.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p2.m_value > p1.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else if (p2.m_value <= p1.m_value)
    Sptr->params.push(Token(INTEGER, 0, "") ) ;
}
//done
void Sally::doGTHANEQUAL(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for >=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  //checks for greater value or equal value
  if (p2.m_value >= p1.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else if (p2.m_value < p1.m_value)
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doLTHAN(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for <.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p2.m_value < p1.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else if (p2.m_value >= p1.m_value)
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doLTHANEQUAL(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for <=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p2.m_value <= p1.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else if (p2.m_value > p1.m_value)
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doEQUAL(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for =.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p2.m_value == p1.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doNOTEQUAL(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for !=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p1.m_value != p2.m_value)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doAND(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for AND.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p1.m_value && p2.m_value == 1)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done
void Sally::doOR(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for OR.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p1.m_value || p2.m_value == 1)
    Sptr->params.push(Token(INTEGER, 1, ""));
  else
    Sptr->params.push(Token(INTEGER, 0, ""));
}
//done? check later
void Sally::doNOT(Sally *Sptr) {
  Token p1;
  cout << Sptr->params.size() << endl;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for NOT.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if (p1.m_value == 1)
    Sptr->params.push(Token(INTEGER, 0, ""));
  else
    Sptr->params.push(Token(INTEGER, 1, ""));
}
//performs if operations
void Sally::doIFTHEN(Sally *Sptr) {
  Token p1, tk;
  int counter = 0;
  map<string,SymTabEntry>::iterator it;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for IFTHEN.") ;
  }
  p1 = Sptr->params.top() ;
  if (p1.m_value == 1) {
    cout << "IFTHEN" << endl;
    while( counter == 0 ) {
      tk = Sptr->nextToken() ;
      if (tk.m_kind == INTEGER || tk.m_kind == STRING) {
        // if INTEGER or STRING just push onto stack
        Sptr->params.push(tk) ;
      } else {
        it = Sptr->symtab.find(tk.m_text) ;
	if ( it == Sptr->symtab.end() )  {   // not in symtab
	  Sptr->params.push(tk) ;
	} else if (it->second.m_kind == KEYWORD)  {
	  // invoke the function for this operation
          //
	  if (it->second.m_dothis == Sptr->symtab["ELSE"].m_dothis) {
	    it->second.m_dothis(Sptr) ;
	    counter++;
	  } else {
	    it->second.m_dothis(Sptr) ;
	  }
	} else if (it->second.m_kind == VARIABLE) {
	  // variables are pushed as tokens
          //
          tk.m_kind = VARIABLE ;
          Sptr->params.push(tk) ;
	} else {
	  // default action
          //
          Sptr->params.push(tk) ;
	}
      }
    }
    Sptr->params.pop();
    Sptr->m_isIf = false;
  }
  else {
    cout << endl;
  }
}
//performs else operations
void Sally::doELSE(Sally *Sptr) {
  Token tk;
  int counter = 0;
  map<string,SymTabEntry>::iterator it;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for ELSE.") ;
  }
  if (Sptr->params.size() == 1) {
    while( counter == 0 ) {
      tk = Sptr->nextToken() ;
      if (tk.m_kind == INTEGER || tk.m_kind == STRING) {
	// if INTEGER or STRING just push onto stack
	Sptr->params.push(tk) ;
      } else {
	it = Sptr->symtab.find(tk.m_text) ;
	if ( it == Sptr->symtab.end() )  {   // not in symtab
	  Sptr->params.push(tk) ;
	} else if (it->second.m_kind == KEYWORD)  {
	  // invoke the function for this operation
	  //
	  if (it->second.m_kind == Sptr->symtab["ENDIF"].m_kind) {
	    it->second.m_dothis(Sptr) ;
	    counter++;
	  } else {
	    it->second.m_dothis(Sptr) ;
	  }
	} else if (it->second.m_kind == VARIABLE) {
	  // variables are pushed as tokens
	  //
	  tk.m_kind = VARIABLE ;
	  Sptr->params.push(tk) ;
	} else {
	  // default action
	  //
	  Sptr->params.push(tk) ;
	}
      }
    }
    Sptr->params.pop();
    Sptr->m_isIf = false;
  }
  else {
    cout << endl;
  }
}

void Sally::doENDIF(Sally *Sptr) {
  cout << endl;
}
//done
void Sally::doDUP(Sally *Sptr) {
  Token p1;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for DUP.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.push(Token(INTEGER, p1.m_value, ""));
}
//done
void Sally::doDROP(Sally *Sptr) {
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for DROP.") ;
  }
  Sptr->params.pop() ;
}
//done
void Sally::doSWAP(Sally *Sptr) {
  Token p1, p2 ;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for SWAP.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  Sptr->params.push(Token(INTEGER, p2.m_value, ""));
  Sptr->params.push(Token(INTEGER, p1.m_value, ""));
}
//done
void Sally::doROT(Sally *Sptr) {
  Token p1, p2, p3;
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need three parameters for ROT.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop();
  p2 = Sptr->params.top() ;
  Sptr->params.pop();
  p3 = Sptr->params.top() ;
  Sptr->params.pop();
  //rotates all of the values
  Sptr->params.push(Token(INTEGER, p2.m_value, ""));
  Sptr->params.push(Token(INTEGER, p1.m_value, ""));
  Sptr->params.push(Token(INTEGER, p3.m_value, ""));
}
//done
void Sally::doDO(Sally *Sptr) {
  //sets the buffer to start saving
  Sptr->m_isDo = true;
}
//done
void Sally::doUNTIL(Sally *Sptr) {
  Token p1;
  p1 = Sptr->params.top();
  Sptr->params.pop();
  if (p1.m_value == 1) {
    Sptr->m_isDo = false;
  } else {
    //sends the buffer back to start
    Sptr->tkBuffer.splice(Sptr->tkBuffer.begin(), Sptr->doBuffer);
  }
}

void Sally::doNEG(Sally *Sptr) {
  Token p ;

  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for NEG.") ;
  }
  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  Sptr->params.push( Token(INTEGER, -p.m_value, "") ) ;
}


void Sally::doDot(Sally *Sptr) {

  Token p ;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for .") ;
  }

  p = Sptr->params.top() ;
  Sptr->params.pop() ;

  if (p.m_kind == INTEGER) {
    cout << p.m_value ;
  } else {
    cout << p.m_text ;
  }
}


void Sally::doSP(Sally *Sptr) {
  cout << " " ;
}


void Sally::doCR(Sally *Sptr) {
  cout << endl ;
}

void Sally::doDUMP(Sally *Sptr) {
  Token p1;
  while (!(Sptr->params.empty())) {
    p1 = Sptr->params.top();
    cout << p1.m_value << endl;
    Sptr->params.pop();
  }
} 

//enum TokenKind { UNKNOWN, KEYWORD, INTEGER, VARIABLE, STRING } ;
