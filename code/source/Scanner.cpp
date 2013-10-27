/* Definition of member functions of class Scanner */

#include "../headers/Scanner.hpp"

#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;


Scanner::Scanner() :
  m_lineNo(1),
  m_column(1),
  m_errors(0),
  m_sourceFile(),
  m_currentToken(0),
  m_nTokens(0),
  m_tokensLexemes(),
  m_keywordsMap(),
  m_errorReporter()
{
  m_keywordsMap["alfanumerico"] = KEYWORD_ALPHANUM;
  m_keywordsMap["canal"] =        KEYWORD_CHANNEL;
  m_keywordsMap["caracter"] =     KEYWORD_CHAR;
  m_keywordsMap["caso"] =         KEYWORD_CASE;
  m_keywordsMap["const"] =        KEYWORD_CONST;
  m_keywordsMap["continua"] =     KEYWORD_CONTINUE;
  m_keywordsMap["defecto"] =      KEYWORD_DEFAULT;
  m_keywordsMap["desde"] =        KEYWORD_FOR;
  m_keywordsMap["diferir"] =      KEYWORD_DIFFER;
  m_keywordsMap["div"] =          KEYWORD_DIV;
  m_keywordsMap["entero"] =       KEYWORD_INT;
  m_keywordsMap["enteros"] =      KEYWORD_UINT;
  m_keywordsMap["estructura"] =   KEYWORD_STRUCT;
  m_keywordsMap["funcion"] =      KEYWORD_FUNCTION;
  m_keywordsMap["importar"] =     KEYWORD_IMPORT;
  m_keywordsMap["interfaz"] =     KEYWORD_INTERFACE;
  m_keywordsMap["interrumpe"] =   KEYWORD_BREAK;
  m_keywordsMap["ir"] =           KEYWORD_GO;
  m_keywordsMap["ir_a"] =         KEYWORD_GOTO;
  m_keywordsMap["logico"] =       KEYWORD_LOGIC;
  m_keywordsMap["mapa"] =         KEYWORD_MAP;
  m_keywordsMap["mod"] =          KEYWORD_MOD;
  m_keywordsMap["paquete"] =      KEYWORD_PACKET;
  m_keywordsMap["principal"] =    KEYWORD_MAIN;
  m_keywordsMap["rango"] =        KEYWORD_RANGE;
  m_keywordsMap["real"] =         KEYWORD_REAL;
  m_keywordsMap["regresa"] =      KEYWORD_RETURN;
  m_keywordsMap["si"] =           KEYWORD_IF;
  m_keywordsMap["sino"] =         KEYWORD_ELSE;
  m_keywordsMap["selecciona"] =   KEYWORD_SELECT;
  m_keywordsMap["tipo"] =         KEYWORD_TYPE;
  m_keywordsMap["valor"] =        KEYWORD_SWITCH;
  m_keywordsMap["variable"] =     KEYWORD_VAR;
}


void Scanner::scan(const string& fileName) { 
  m_sourceFile.open(fileName.c_str());

  if (m_sourceFile.is_open())
  {

    char currentChar;
    int nextState = 0, currentState = 0;
    TokenType_t token;
    string lexeme;

    string line;
    size_t lineLength;
    m_lineNo = 1;
    while (m_sourceFile.good() && m_errors < ERRORS_MAX_LIMIT) {
      getline(m_sourceFile, line);
      line += "\n";

      lineLength = line.length();
      m_column = 1;
      while (m_column <= lineLength && m_errors <= ERRORS_MAX_LIMIT) {
        currentChar = line.at(m_column - 1);
        nextState = automata[currentState][getTransitionIndex(currentChar)];

#ifdef DEBUG
        cout << "CState: " << setw(2) << currentState << "  NState: " << 
                setw(2) << nextState << "  TIndex: " << 
                setw(2) << getTransitionIndex(currentChar) << 
                " Char: ";
        if (!isspace(currentChar))
            cout << currentChar;
        else
            cout << ' ';
        cout << " : " << setw(3) << (int)currentChar << endl;
#endif
   
        
        if (nextState == STATE_ACCEPT_ERROR) {
          switch (currentState) {
            case 1 :
            case 2 : token = TOKEN_OCT;
              break;
            case 4 :
              token = TOKEN_HEX;
              break;
            case 5 :
              token = TOKEN_DEC;
              break;
            case 6 :
            case 11 :
              token = TOKEN_FLOAT;
              break;
            case 7 :
            case 26 :
              token = TOKEN_DELIMITER;
              break;
            case 8 :
              token = TOKEN_FLOAT;
              break;
            case 13 :
              token = TOKEN_STRING;
              break;
            case 14 :
            case 19 :
              token = TOKEN_ARITHOP;
              break;
            case 16 :
              token = TOKEN_LINECOMMENT;
              break;
            case 18 :
              token = TOKEN_MULTICOMMENT;
              break;
            case 22 :
            case 24 :
              token = TOKEN_LOGICOP;
              break;
            case 23 :
            case 27 :
              token = TOKEN_ASSIGNOP;
              break;
            case 25 :
            case 35 :
            case 36 :
              token = TOKEN_RELOP;
              break;
            case 28 : 
              token = TOKEN_IDEN;
              if (m_keywordsMap.find(lexeme) != m_keywordsMap.end()) {
                token = TOKEN_KEYWORD;
              }
              else if (lexeme.compare("verdadero") == 0 ||
                       lexeme.compare("falso") == 0) {
                token = TOKEN_LOGICCONST;
              }
              break;
            case 29 :
              token = TOKEN_DELIMITER;
              break;
            case 33 :
              token = TOKEN_CHARCONST;
              break;
            default :
              cerr << "vecomp: error de estado siguiente" << endl;
              break;
          }

          if (token != TOKEN_LINECOMMENT && token != TOKEN_MULTICOMMENT)
            m_tokensLexemes.push(TokenLexeme(token, lexeme, m_lineNo, 
                                             m_column - lexeme.length()));

          token = TOKEN_INVALID;
          lexeme = "";
          nextState = 0;
          --m_column;
        }
        else if (nextState == STATE_ERROR) {
#ifdef DEBUG
          cout << "calling lexicalerror" << currentState << " " <<
              currentChar << endl;
#endif
          m_errorReporter.writeLexicalError(currentState, currentChar, line,
                                            lexeme, m_lineNo, m_column);
          ++m_errors;

          token = TOKEN_INVALID;
          lexeme = "";
          nextState = 0;
        }
        else if ( isspace(currentChar) && 
                 (currentState == 12 || currentState == 15 || 
                  currentState == 17))
          lexeme += currentChar;
        else if (!isspace(currentChar))
          lexeme += currentChar;

        currentState = nextState;

        ++m_column;
      }
      ++m_lineNo;
      line = "";
    }

    if (!m_sourceFile.good()) {
#ifdef DEBUG
      cout << "final state: " << currentState << endl;
#endif
      if (currentState != 0 && !isTerminalState(currentState))
        m_errorReporter.writeLexicalError(currentState, currentChar, line,
                                          lexeme, m_lineNo, m_column);
    }

    m_nTokens = m_tokensLexemes.size();

    if (m_tokensLexemes.empty())
      m_errorReporter.writeError("archivo vacio");
  }
  else {
    m_errorReporter.writeError("error al abrir codigo fuente");
    ++m_errors;
  }
}

TokenLexeme Scanner::getNextTokenLexeme() {
  TokenLexeme temporal;
  if (!m_tokensLexemes.empty()) {
    temporal = m_tokensLexemes.front();
    m_tokensLexemes.pop();
  }
  return temporal;
}

Transition_t Scanner::getTransitionIndex(char character) {
  Transition_t transitionIndex = TRANS_ANY; 

  if (isdigit(character)) {
    if (character == '0')
      transitionIndex = TRANS_ZERO;
    else if (character <= '7')
      transitionIndex = TRANS_OCT;
    else
      transitionIndex = TRANS_DEC;
  }
  else if (isalpha(character)) {
    if (tolower(character) == 'e')
      transitionIndex = TRANS_E;
    else if (tolower(character) == 'x')
      transitionIndex = TRANS_X;
    else if (tolower(character) <= 'f')
      transitionIndex = TRANS_HEX;
    else
      transitionIndex = TRANS_LETTER;
  }
  else {
    switch (character) {
      case '|' :
        transitionIndex = TRANS_PIPE;
        break;
      case '&' :
        transitionIndex = TRANS_AMPERS;
        break;
      case '!' :
        transitionIndex = TRANS_EXCLAMATION;
        break;
      case ' ' :
      case '\t' :
      case '\r' :
        transitionIndex = TRANS_SPACE;
        break;
      case '\n' :
        transitionIndex = TRANS_NEWLINE;
        break;
      case ',' :
      case ';' :
      case '(' :
      case ')' :
      case '[' :
      case ']' :
      case '{' :
      case '}' :
        transitionIndex = TRANS_DELIMITER;
        break;
      case '+' :
      case '-' :
        transitionIndex = TRANS_SIGN;
        break;
      case '*' :
        transitionIndex = TRANS_ASTERISK;
        break;
      case '/' :
        transitionIndex = TRANS_SLASH;
        break;
      case '%' :
      case '^' :
        transitionIndex = TRANS_ARITHMETIC;
        break;
      case ':' :
        transitionIndex = TRANS_COLON;
        break;
      case '=' :
        transitionIndex = TRANS_EQUAL;
        break;
      case '<' :
        transitionIndex = TRANS_LESSER;
        break;
      case '>' :
        transitionIndex = TRANS_GREATER;
        break;
      case '_' :
        transitionIndex = TRANS_UNDERSCORE;
        break;
      case '\'' :
        transitionIndex = TRANS_SQUOTE;
        break;
      case '\\' :
        transitionIndex = TRANS_BACKSLASH;
      case '"' :
        transitionIndex = TRANS_DQUOTE;
        break;
      case '.' :
        transitionIndex = TRANS_DOT;
        break;
      default :
        transitionIndex = TRANS_ANY;
        break;
    }
  }
  
  return transitionIndex;
}

int Scanner::getMaxTokens() const {
  return m_nTokens;
}

int Scanner::getErrors() const {
  return m_errors;
}

bool Scanner::isTerminalState(int state) {
  switch (state) {
    case 1 :
    case 2 :
    case 4 :
    case 5 :
    case 6 :
    case 7 :
    case 8 :
    case 11 :
    case 13 :
    case 14 :
    case 16 :
    case 18 :
    case 19 :
    case 22 :
    case 23 :
    case 24 :
    case 25 :
    case 26 :
    case 27 :
    case 28 :
    case 29 :
    case 33 :
    case 35 :
    case 36 :
      return true;
      break;
  }

  return false;
}
