/* Scanner class definition */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <utility>
#include <fstream>
#include <queue>
#include <map>

#include "tokenlexeme.hpp"

#define N_KEYWORDS    20

#define N_TRANSITIONS 27
#define N_STATES      37

typedef enum {
  TRANS_PIPE,        //  0
  TRANS_AMPERS,      //  1
  TRANS_COLON,       //  2
  TRANS_EQUAL,       //  3
  TRANS_ZERO,        //  4
  TRANS_OCT,         //  5
  TRANS_DEC,         //  6
  TRANS_HEX,         //  7
  TRANS_DOT,         //  8
  TRANS_DQUOTE,      //  9
  TRANS_SQUOTE,      // 10
  TRANS_SLASH,       // 11
  TRANS_BACKSLASH,   // 12
  TRANS_ANY,         // 13 
  TRANS_NEWLINE,     // 14 
  TRANS_SIGN,        // 15
  TRANS_ASTERISK,    // 16
  TRANS_ARITHMETIC,  // 17
  TRANS_EXCLAMATION, // 18
  TRANS_X,           // 19
  TRANS_E,           // 20
  TRANS_DELIMITER,   // 21
  TRANS_UNDERSCORE,  // 22
  TRANS_LESSER,      // 23
  TRANS_GREATER,     // 24
  TRANS_LETTER,      // 25
  TRANS_SPACE        // 26
} Transition_t;

typedef enum
{
  KEYWORD_ALPHANUM,
  KEYWORD_CASE,
  KEYWORD_CHANNEL,
  KEYWORD_CHAR,
  KEYWORD_CONST,
  KEYWORD_CONTINUE,
  KEYWORD_DEFAULT,
  KEYWORD_FOR,
  KEYWORD_DIFFER,
  KEYWORD_DIV,
  KEYWORD_INT,
  KEYWORD_UINT,
  KEYWORD_STRUCT,
  KEYWORD_FUNCTION,
  KEYWORD_IMPORT,
  KEYWORD_INTERFACE,
  KEYWORD_BREAK,
  KEYWORD_GO,
  KEYWORD_GOTO,
  KEYWORD_LOGIC,
  KEYWORD_MAP,
  KEYWORD_MOD,
  KEYWORD_PACKET,
  KEYWORD_MAIN,
  KEYWORD_RANGE,
  KEYWORD_REAL,
  KEYWORD_RETURN,
  KEYWORD_IF,
  KEYWORD_ELSE,
  KEYWORD_SELECT,
  KEYWORD_TYPE,
  KEYWORD_SWITCH,
  KEYWORD_VAR,
} Keyword_t;


static const int automata[N_STATES][N_TRANSITIONS] {
  // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26
  // |   &   :   =   0 oct dec hex   .   "   '   /   \ any  nl  +-   *   %   !   x   e del   _   <   >   l spa
  { 20, 21, 26, 23,  1,  5,  5, 28,  7, 12, 30, 14, -1, -1,  0, 19, 19, 19, 24, 28, 28, 29, 28, 35, 35, 28,  0}, //  0
  { -2, -2, -2, -2,  2,  2, -2, -2,  6, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  3,  9, -2, -2, -2, -2, -2, -2}, //  1 oct
  { -2, -2, -2, -2,  2,  2, -2, -2,  6, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  9, -2, -2, -2, -2, -2, -2}, //  2 oct
  { -1, -1, -1, -1,  4,  4,  4,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //  3
  { -2, -2, -2, -2,  4,  4,  4,  4, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, //  4 hex
  { -2, -2, -2, -2,  5,  5,  5, -2,  6, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  9, -2, -2, -2, -2, -2, -2}, //  5 dec
  { -1, -1, -1, -1,  8,  8,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  9, -1, -1, -1, -1, -1, -1}, //  6 float
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, //  7 del
  { -2, -2, -2, -2,  8,  8,  8, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  9, -2, -2, -2, -2, -2, -2}, //  8 float
  { -1, -1, -1, -1, 11, 11, 11, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //  9
  { -1, -1, -1, -1, 11, 11, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 10
  { -2, -2, -2, -2, 11, 11, 11, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 11 float
  { 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}, // 12
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 13 string
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 15, -2, -2, -2, -2, 17, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 14 arith
  { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}, // 15
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 16 linecom
  { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 34, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17}, // 17 
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 18 multicom
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 19 arith 
  { 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 20
  { -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 21
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 22 logical 
  { -2, -2, -2, 25, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 23 assign
  { -2, -2, -2, 25, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 24 logical
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 25 rel 
  { -2, -2, -2, 27, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 26 delim
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 27 assign
  { -2, -2, -2, -2, 28, 28, 28, 28, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 28, 28, -2, 28, -2, -2, 28, -2}, // 28 iden
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 29 delim
  { 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 31, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33}, // 30
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1}, // 31
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 32
  { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 33 charc
  { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17}, // 34
  { -2, -2, -2, 36, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, // 35 rel
  { -2, -2, -2, 36, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}  // 36 rel
  // |   &   :   =   0 oct dec hex   .   "   '   /   \ any  nl  +-   *   %   !   x   e   l   _   <   >   l spa
};

static const int STATE_ERROR        = -1;
static const int STATE_ACCEPT_ERROR = -2;

static const int ERRORS_MAX_LIMIT   =  5;

static const int WIDTH_NUMBER =   8;
static const int WIDTH_LEXEME =  15;
static const int WIDTH_MESSAGE = 80;
static const int WIDTH_LINE =   100;

class Scanner
{
public:
  Scanner();

  void        scan(const std::string& fileName);
  int         getMaxTokens() const;
  int         getErrors() const;
  TokenLexeme getNextTokenLexeme();

private:
  Transition_t getTransitionIndex(char character);
  void         lexicalError(int state, char currentChar, 
                            const std::string& line,
                            const std::string& lexeme);
  void         lexicalError(const std::string& message);
  void         writeErrorsFileHeader();
  bool         isTerminalState(int state);

  int                               m_lineNo;
  size_t                            m_column;
  int                               m_errors;
  std::ifstream                     m_sourceFile;
  int                               m_currentToken;
  int                               m_nTokens;
  std::queue<TokenLexeme>           m_tokensLexemes;
  std::map<std::string, Keyword_t>  m_keywordsMap;
  std::ofstream                     m_errorOut;
};

#endif // SCANNER_HPP

