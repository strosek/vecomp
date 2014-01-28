/* Definition of member functions of class ErrorReporter */

#include "../headers/ErrorReporter.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

ErrorReporter::ErrorReporter() :
  m_warnings(0),
  m_errors(0),
  m_maxErrors(5),
  m_fileReader(nullptr)
{
}

ErrorReporter* ErrorReporter::m_instance = nullptr;

ErrorReporter::~ErrorReporter()
{
  delete m_instance;
}

ErrorReporter* ErrorReporter::getInstance()
{
  if (m_instance == nullptr)
  {
    m_instance = new ErrorReporter();
    return m_instance; 
  }
  return m_instance;
}

void ErrorReporter::writeError(const string& message)
{
  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    cerr << "error: " << m_fileReader->getCurrentFileName() << ':' <<
        setw(WIDTH_LEXEME) << ' ' << setw(WIDTH_MESSAGE) << message <<
        setw(WIDTH_LINE) << ' ' << endl;
  }

  ++m_errors;
}

void ErrorReporter::writeError(int line, int column, const std::string& lexeme,
                               const std::string& message)
{
  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    cerr << "error: " << m_fileReader->getCurrentFileName() << ':' << line <<
        ':' << column << setw(WIDTH_LEXEME) << lexeme << 
        setw(WIDTH_MESSAGE) << message <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    cerr.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeLexicalError(int state, char currentChar,
                                      const string& lexeme, int line,
                                      int column)
{
  ostringstream messageBuilder;
  switch (state)
  {
  case 0 :
    messageBuilder << "simbolo ilegal: '" << currentChar << "'";
    break;
  case 3 :
    messageBuilder << "constante hexadecimal incompleta, se encontro : '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba digito";
    break;
  case 6 :
  case 10 :
    messageBuilder << "constante flotante incompleta, se encontro : '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba digito";
    break;
  case 9 :
    messageBuilder << 
        "constante flotante incompleta, se encontro : '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba digito o signo";
    break;
  case 17 :
    messageBuilder << "comentario multilinea sin cerrar";
    break;
  case 20 :
    messageBuilder << "operador logico incompleto, se encontro: '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba '|'";
    break;
  case 21 :
    messageBuilder << "operador logico incompleto, se encontro: '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba '&'";
    break;
  case 30 :
    messageBuilder << "constante caracter incompleta, se encontro: '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba '\\' o letra";
    break;
  case 31 :
    messageBuilder << "constante caracter incompleta, se encontro: '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba letra";
    break;
  case 32 :
    messageBuilder << "constante caracter incompleta, se encontro: '";
    if (!isspace(currentChar))
      messageBuilder << currentChar;
    else
      messageBuilder << ' ';
    messageBuilder << "', se esperaba '''";
    break;
  case 34 :
    messageBuilder << "comentario multilinea sin cerrar";
    break;
  default :
    messageBuilder << "error desconocido";
    break;
  }
  
  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    cerr << "error: " << m_fileReader->getCurrentFileName() << ':' << line <<
        ':' << column << setw(WIDTH_LEXEME) << lexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    cerr.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeSyntaxError(const std::string& expectedLexeme,
    const std::string& actualLexeme, int line, int column)
{
  ostringstream messageBuilder;

  messageBuilder << "se esperaba lexema: \"" << expectedLexeme <<
                    "\", se recibio: \"" << actualLexeme << "\"";

  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    cerr << "error: " << m_fileReader->getCurrentFileName() << ':' << line <<
        ':' << column << setw(WIDTH_LEXEME) << actualLexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    cerr.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeSyntaxError(TokenType_t expectedToken,
    TokenType_t actualToken, const string& actualLexeme, int line, int column)
{
  ostringstream messageBuilder;

  messageBuilder << "se esperaba token: \"" <<
                    TokenLexeme::getTokenString(expectedToken) <<
                    "\", se recibio: \"" <<
                    TokenLexeme::getTokenString(actualToken) << "\"";

  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    cerr << "error: " << m_fileReader->getCurrentFileName() << ':' << line <<
        ':' << column << setw(WIDTH_LEXEME) << actualLexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    cerr.flush();
  }

  ++m_errors;
}

void ErrorReporter::setFileReader(FileReader* fileReader)
{
  m_fileReader = fileReader;
}

void ErrorReporter::setMaxErrors(int maxErrors)
{
  m_maxErrors = maxErrors;
}

unsigned int  ErrorReporter::getMaxErrors() const
{
  return m_maxErrors;
}

unsigned int  ErrorReporter::getErrors() const
{
  return m_errors;
}

unsigned int  ErrorReporter::getWarnings() const
{
  return m_warnings;
}

