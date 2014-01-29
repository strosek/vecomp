/* Main function definition for vecomp program */

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "../headers/ErrorReporter.hpp"
#include "../headers/FileReader.hpp"
#include "../headers/Parser.hpp"
#include "../headers/main.hpp"

using namespace std;


int main(int argc, char** argv)
{
  ErrorReporter* errorReporter = nullptr;

  if (argc == 2)
  {
#ifdef DEBUG
    cout << argc << " "  << argv[0] << " " << argv[1] << endl;
#endif
    errorReporter = ErrorReporter::getInstance();

    FileReader* fileReader = FileReader::getInstance();
    Scanner* scanner = Scanner::getInstance();

    errorReporter->setFileReader(fileReader);
    errorReporter->setScanner(scanner);
    fileReader->setErrorReporter(errorReporter);
    scanner->setErrorReporter(errorReporter);
    scanner->setFileReader(fileReader);

    fileReader->readFile(argv[1]);

    Parser parser;
    parser.setErrorReporter(errorReporter);
    parser.setScanner(scanner);

    parser.parse();
  }
  else
  {
    cerr << "error: se esperaba un nombre de archivo a compilar" << endl;
    cerr << "uso:   vecomp <archivo>" << endl;
  }

  int returnValue = EXIT_SUCCESS;

  if (errorReporter == nullptr)
  {
    returnValue = EXIT_FAILURE;
  }
  else
  {
    if (errorReporter->getErrors() > 0)
    {
      unsigned int actualErrors = errorReporter->getErrors();
      unsigned int reportedErrors = actualErrors;

      if (actualErrors > errorReporter->getMaxErrors())
      {
        reportedErrors = errorReporter->getMaxErrors();
      }

      cerr << "\nCompilacion terminada con: " << reportedErrors <<
          " errores, " << errorReporter->getWarnings() << " advertencias" <<
          endl;

      returnValue = EXIT_FAILURE;
    }
    else
    {
      returnValue = EXIT_SUCCESS;
    }
  }
  
  return returnValue;
}

