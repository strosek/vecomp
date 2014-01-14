/* Main function definition for vecomp program */

#include <cstdio>
#include <cstdlib>
#include <iomanip>
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

    errorReporter->setFileReader(fileReader);
    fileReader->setErrorReporter(errorReporter);

    fileReader->readFile(argv[1]);

    Parser parser(fileReader, errorReporter);
    parser.parse();
  }
  else
  {
    cerr << "error: se esperaba un nombre de archivo a compilar" << endl;
    cerr << "uso:   vecomp <archivo>" << endl;
  }

  int returnValue;
  if (errorReporter == nullptr)
  {
    returnValue = EXIT_FAILURE;
  }
  else
  {
    returnValue = errorReporter->getErrors() == 0 ? EXIT_SUCCESS :
        EXIT_FAILURE;
  }
  
  return returnValue;
}

