/* Definition of member functions of class ErrorReporter */

#include "../headers/ErrorReporter.hpp"

#include <iomanip>
#include <sstream>
#include <string>

#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

ErrorReporter::ErrorReporter() :
  m_errorOut(),
  m_outFileName("errores.out"),
  m_warnings(0),
  m_errors(0),
  m_maxErrors(5),
  m_fileReader(nullptr)
{
}

ErrorReporter::ErrorReporter(const string& outFileName) :
  m_errorOut(),
  m_outFileName(outFileName),
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

ErrorReporter* ErrorReporter::getInstance(const string& outFileName)
{
  if (m_instance == nullptr)
  {
    m_instance = new ErrorReporter(outFileName);
    return m_instance; 
  }
  return m_instance;
}

void ErrorReporter::writeError(const string& message)
{
  writeErrorsFileHeader();

  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    m_errorOut << setw(WIDTH_NUMBER) << ' ' <<
        setw(WIDTH_NUMBER) << ' ' <<
        setw(WIDTH_LEXEME) << ' ' << 
        setw(WIDTH_MESSAGE) << message <<
        setw(WIDTH_LINE) << ' ' << endl;
  }

  ++m_errors;
}

void ErrorReporter::writeLexicalError(int state, char currentChar,
                                      const string& lexeme, int line,
                                      int column)
{
  writeErrorsFileHeader();

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
    m_errorOut << setw(WIDTH_NUMBER) << line <<
        setw(WIDTH_NUMBER) << column <<
        setw(WIDTH_LEXEME) << lexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    m_errorOut.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeSyntaxError(const std::string& expectedLexeme,
    const std::string& actualLexeme, int line, int column)
{
  writeErrorsFileHeader();

  ostringstream messageBuilder;

  messageBuilder << "se esperaba lexema: \"" << expectedLexeme <<
                    "\", se recibio: \"" << actualLexeme << "\"";

  if (m_errors < m_maxErrors)
  {
#ifdef DEBUG
    cout << "::: writing error" << endl;
#endif
    m_errorOut << setw(WIDTH_NUMBER) << line <<
        setw(WIDTH_NUMBER) << column <<
        setw(WIDTH_LEXEME) << actualLexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    m_errorOut.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeSyntaxError(TokenType_t expectedToken,
    TokenType_t actualToken, const string& actualLexeme, int line, int column)
{
  writeErrorsFileHeader();

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
    m_errorOut << setw(WIDTH_NUMBER) << line <<
        setw(WIDTH_NUMBER) << column <<
        setw(WIDTH_LEXEME) << actualLexeme << 
        setw(WIDTH_MESSAGE) << messageBuilder.str() <<
        setw(WIDTH_LINE) << m_fileReader->getTextAtLine(line - 1);
    m_errorOut.flush();
  }

  ++m_errors;
}

void ErrorReporter::writeErrorsFileHeader()
{
  if (!m_errorOut.is_open())
  {
    m_errorOut.open(m_outFileName, ios::trunc);

    writeErrorsFileSeparator();
    m_errorOut << setw(WIDTH_NUMBER) << "Linea" <<
        setw(WIDTH_NUMBER) << "Columna" <<
        setw(WIDTH_LEXEME) << "Lexema" << 
        setw(WIDTH_MESSAGE) << "Mensaje de error" <<
        setw(WIDTH_LINE) << "Linea" << endl;
    writeErrorsFileSeparator();
  }
}

void ErrorReporter::setFileReader(FileReader* fileReader)
{
  m_fileReader = fileReader;
}

void ErrorReporter::setMaxErrors(int maxErrors)
{
  m_maxErrors = maxErrors;
}

int  ErrorReporter::getMaxErrors() const
{
  return m_maxErrors;
}

int  ErrorReporter::getErrors() const
{
  return m_errors;
}

int  ErrorReporter::getWarnings() const
{
  return m_warnings;
}

void ErrorReporter::endErrorsFile()
{
  m_errorOut << endl;
  writeErrorsFileSeparator();

  int totalErrors;
  if (m_errors > m_maxErrors)
  {
    totalErrors = m_maxErrors;
  }
  else
  {
    totalErrors = m_errors;
  }
  m_errorOut << "errores: " << totalErrors << ", advertencias: " <<
                m_warnings << endl;

  m_errorOut.close();
}

void ErrorReporter::writeErrorsFileSeparator()
{
  for (int i = 0; 
       i < (WIDTH_NUMBER * 2 + WIDTH_LEXEME + WIDTH_MESSAGE + WIDTH_LINE); ++i)
  {
    m_errorOut << '-';
  }
  m_errorOut << endl;
}

