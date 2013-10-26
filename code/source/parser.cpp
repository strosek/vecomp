//  Copyright (c) 2013 Erick Daniel Corona <edcoronag@gmail.com>
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the Software), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.

// #define DEBUG

#include <string>
#include <iostream>
#include <cstdlib>

#include "../headers/parser.hpp"

using namespace std;

Parser::Parser() :
  m_errors(0),
  m_warnings(0),
  m_tokenNo(0),
  m_scanner(),
  m_currentToken()
{
}

int Parser::getErrors() const {
  return m_errors;
}

int Parser::getWarnings() const {
  return m_warnings;
}

int Parser::parse(const string& fileName) {
  m_scanner.scan(fileName);

  if ((m_errors = m_scanner.getErrors()) == 0) {
    if (program() == EXIT_FAILURE)
      return EXIT_FAILURE;

    if (m_tokenNo != m_scanner.getMaxTokens()) {
      cerr << "njavac: error: codigo incompleto" << endl;
      ++m_errors;
      return EXIT_FAILURE;
    }
  }
  else
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::program() {
#ifdef DEBUG
  cout << "program() called, " << m_currentToken.getLexeme() << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "publico") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"publico\"" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "clase") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"clase\"" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getToken() != "identificador") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba un identificador" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "{") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba llave de apertura" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  if (variablesMain() == EXIT_FAILURE)
    return EXIT_FAILURE;

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "}") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba llave de cerradura" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int Parser::variablesMain() {
#ifdef DEBUG
  cout << "variablesMain() called, " << m_currentToken.getLexeme() << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  do {
    if (variable() == EXIT_FAILURE)
      return EXIT_FAILURE;

    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
  } while ( m_currentToken.getToken()  == "pal_reservada" && 
           (m_currentToken.getLexeme() == "entero" ||
            m_currentToken.getLexeme() == "real" || 
            m_currentToken.getLexeme() == "alfabetico" || 
            m_currentToken.getLexeme() == "logico") );

  if (mainFunction() == EXIT_FAILURE)
      return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::variable() {
#ifdef DEBUG
  cout << "variable() called, " << m_currentToken.getLexeme() << endl;
#endif
  if ( m_currentToken.getToken()  != "pal_reservada" &&
       m_currentToken.getLexeme() != "entero" &&
       m_currentToken.getLexeme() != "real" && 
       m_currentToken.getLexeme() != "alfabetico" && 
       m_currentToken.getLexeme() != "logico") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
          m_currentToken.getRow() << ":" << 
          " se esperaba tipo de dato" << endl;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    ++m_errors;
    return EXIT_FAILURE;
  }

  do {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getToken() != "identificador") {
      cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
          m_currentToken.getRow() << ":" << 
          " se esperaba un identificador" << endl;
#ifdef DEBUG
      cout << m_currentToken.getLexeme() << endl;
#endif
      ++m_errors;
      return EXIT_FAILURE;
    }

    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getToken() == "op_asignacion") {
      if (initializationValue() == EXIT_FAILURE)
        return EXIT_FAILURE;
    }
    else if (m_currentToken.getLexeme() != "," &&
             m_currentToken.getLexeme() != ";") {
      cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
          m_currentToken.getRow() << ":" << 
          " se esperaba un delimitador \",\" o \";\"" << endl;
#ifdef DEBUG
      cout << m_currentToken.getLexeme() << endl;
#endif
      ++m_errors;
      return EXIT_FAILURE;
    }
  } while (m_currentToken.getLexeme() == ",");

  if (m_currentToken.getLexeme() != "," &&
      m_currentToken.getLexeme() != ";") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba un delimitador \";\"" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int Parser::initializationValue() {
#ifdef DEBUG
  cout << "initializationValue() called, " << m_currentToken.getLexeme() << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getToken() != "const_alfabetica" &&
      m_currentToken.getToken() != "const_logica" &&
      m_currentToken.getToken() != "entero" &&
      m_currentToken.getToken() != "real" &&
      m_currentToken.getToken() != "octal" &&
      m_currentToken.getToken() != "hexadecimal") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba constante" << endl;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    ++m_errors;
    return EXIT_FAILURE;
  }
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;

  return EXIT_SUCCESS;
}

int Parser::mainFunction() {
#ifdef DEBUG
  cout << "mainFunction() called, " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getLexeme() != "publico") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"publico\"" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "estatico") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"estatico\"" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "sin_tipo") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"sin_tipo\"" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "principal") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba \"principal\"" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "(") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba parentesis de apertura" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }
  
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != ")") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba parentesis de cerradura" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }
  
  if (body() == EXIT_FAILURE)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int Parser::body() {
#ifdef DEBUG
  cout << "body() called, " << m_currentToken.getLexeme() << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "{") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba llave de apertura" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  if (statement() == EXIT_FAILURE)
    return EXIT_FAILURE;

  if (m_currentToken.getLexeme() != "}") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba llave de cerradura" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int Parser::statement() {
#ifdef DEBUG
  cout << "statement() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    if (command() == EXIT_FAILURE)
      return EXIT_FAILURE;

    if (m_currentToken.getLexeme() != ";") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
    if (m_currentToken.getLexeme() != ";") {
      cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
          m_currentToken.getRow() << ":" << 
          " se esperaba delimitador \";\"" << endl;
#ifdef DEBUG
      cout << m_currentToken.getLexeme() << endl;
#endif
      ++m_errors;
      return EXIT_FAILURE;
    }

    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
  } while (m_currentToken.getLexeme() != "}");
  
  return EXIT_SUCCESS;
}

int Parser::command() {
#ifdef DEBUG
  cout << "command() called, " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getToken() != "identificador" &&
      m_currentToken.getLexeme() != "imprime") {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
  }
  if (m_currentToken.getLexeme() == "imprime") {
    if (print() == EXIT_FAILURE)
      return EXIT_FAILURE;
  }
  else {
    if (assign() == EXIT_FAILURE)
      return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int Parser::assign() {
#ifdef DEBUG
  cout << "assign() called, " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getToken() != "identificador") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba identificador" << endl;
    ++m_errors;
#ifdef DEBUG
    cout << m_currentToken.getLexeme() << endl;
#endif
    return EXIT_FAILURE;
  }

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getToken() != "op_asignacion") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba operador de asignacion" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }

  if (expression() == EXIT_FAILURE)
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::print() {
#ifdef DEBUG
  cout << "print() called, " << m_currentToken.getLexeme() << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme() != "(") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba parentesis de apertura" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }
  
  if (expression() == EXIT_FAILURE)
    return EXIT_FAILURE;
  
  if (m_currentToken.getLexeme() != ")") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba parentesis de cerradura" << endl;
    ++m_errors;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int Parser::expression() {
#ifdef DEBUG
  cout << "expression() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getLexeme() == "no") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }

    if (relationalExpression() == EXIT_FAILURE)
      return EXIT_FAILURE;
    
    temporal = m_currentToken.getLexeme();
    if (m_currentToken.getLexeme() == "y" ||
        m_currentToken.getLexeme() == "o") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
  } while (temporal == "y" ||
           temporal == "o");
  
  return EXIT_SUCCESS;
}

int Parser::relationalExpression() {
#ifdef DEBUG
  cout << "relationalExpression() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    if (arithmeticExpression() == EXIT_FAILURE)
      return EXIT_FAILURE;
    
    temporal = m_currentToken.getToken();
    if (m_currentToken.getToken() == "op_relacional") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
  } while (temporal == "op_relacional");
  
  return EXIT_SUCCESS;
}

int Parser::arithmeticExpression() {
#ifdef DEBUG
  cout << "arithmeticExpression() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    if (multiplication() == EXIT_FAILURE)
      return EXIT_FAILURE;
    
    temporal = m_currentToken.getLexeme();
    if (m_currentToken.getLexeme() == "+" ||
        m_currentToken.getLexeme() == "-") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
  } while (temporal == "+" ||
           temporal == "-");
  
  return EXIT_SUCCESS;
}

int Parser::multiplication() {
#ifdef DEBUG
  cout << "multiplication() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    if (exponent() == EXIT_FAILURE)
      return EXIT_FAILURE;
    
    temporal = m_currentToken.getLexeme();
    if (m_currentToken.getLexeme() == "*" ||
        m_currentToken.getLexeme() == "/" ||
        m_currentToken.getLexeme() == "%") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
  } while (temporal == "*" ||
           temporal == "/" ||
           temporal == "%");
  
  return EXIT_SUCCESS;
}

int Parser::exponent() {
#ifdef DEBUG
  cout << "exponent() called, " << m_currentToken.getLexeme() << endl;
#endif
  string temporal;
  do {
    if (sign() == EXIT_FAILURE)
      return EXIT_FAILURE;
    
    temporal = m_currentToken.getLexeme();
    if (m_currentToken.getLexeme() == "^") {
      m_currentToken = m_scanner.getNextTokenLexeme();
      ++m_tokenNo;
    }
  } while (temporal == "^");
  
  return EXIT_SUCCESS;
}

int Parser::sign() {
#ifdef DEBUG
  cout << "sign() called, " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getLexeme() == "-") {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
  }

  if (term() == EXIT_FAILURE)
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::term() {
#ifdef DEBUG
  cout << "term() called, " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getLexeme() == "(") {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;

    if (term() == EXIT_FAILURE)
      return EXIT_FAILURE;

    if (m_currentToken.getLexeme() != ")") {
      cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
          m_currentToken.getRow() << ":" << 
          " se esperaba parentesis de cerradura" << endl;
      ++m_errors;
      return EXIT_FAILURE;
    }
  }
  else if (m_currentToken.getToken() != "identificador" &&
           m_currentToken.getToken() != "entero" &&
           m_currentToken.getToken() != "real" &&
           m_currentToken.getToken() != "octal" &&
           m_currentToken.getToken() != "hexadecimal" &&
           m_currentToken.getToken() != "const_alfabetica") {
    cerr << "njavac: error: " << m_currentToken.getLine() << ", " <<
        m_currentToken.getRow() << ":" << 
        " se esperaba identificador o numero" << endl;
    ++m_errors;
  }
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  
  return EXIT_SUCCESS;
}
