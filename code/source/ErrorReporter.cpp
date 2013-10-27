/* Definition of member functions of class ErrorReporter */

#include "../headers/ErrorReporter.hpp"

#include <iomanip>
#include <sstream>

using namespace std;

ErrorReporter::ErrorReporter() :
  m_errorOut(),
  m_outFileName("errores.out")
{
}

ErrorReporter::ErrorReporter(const string& outFileName) :
  m_errorOut(),
  m_outFileName(outFileName)
{
}

void ErrorReporter::writeError(const string& message)
{
  writeErrorsFileHeader();

  m_errorOut << setw(WIDTH_NUMBER) << ' ' <<
      setw(WIDTH_NUMBER) << ' ' <<
      setw(WIDTH_LEXEME) << ' ' << 
      setw(WIDTH_MESSAGE) << message <<
      setw(WIDTH_LINE) << ' ';
}

void ErrorReporter::writeLexicalError(int state, char currentChar,
                                      const string& line, const string& lexeme,
                                      int lineNo, int columnNo) {
  writeErrorsFileHeader();

  ostringstream messageBuilder;
  switch (state) {
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
  
  string errorMessage = messageBuilder.str();
  m_errorOut << setw(WIDTH_NUMBER) << lineNo <<
      setw(WIDTH_NUMBER) << columnNo <<
      setw(WIDTH_LEXEME) << lexeme << 
      setw(WIDTH_MESSAGE) << errorMessage <<
      setw(WIDTH_LINE) << line;
}

void ErrorReporter::writeErrorsFileHeader() {
  if (!m_errorOut.is_open()) {
    m_errorOut.open(m_outFileName, ios::app);

    for (int i = 0; 
         i < (WIDTH_NUMBER * 2 + WIDTH_LEXEME + WIDTH_LEXEME + WIDTH_LINE);
         ++i)
      m_errorOut << '-';
    m_errorOut << endl;

    m_errorOut << setw(WIDTH_NUMBER) << "Linea" <<
        setw(WIDTH_NUMBER) << "Columna" <<
        setw(WIDTH_LEXEME) << "Lexema" << 
        setw(WIDTH_MESSAGE) << "Mensaje de error" <<
        setw(WIDTH_LINE) << "Linea" << endl;

    for (int i = 0;
         i < (WIDTH_NUMBER * 2 + WIDTH_LEXEME + WIDTH_MESSAGE + WIDTH_LINE);
         ++i)
    {
      m_errorOut << '-';
    }
    m_errorOut << endl;
  }
}

