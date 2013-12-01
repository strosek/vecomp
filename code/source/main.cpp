/* Main function definition for vecomp program */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#include "../headers/main.hpp"
#include "../headers/FileReader.hpp"
#include "../headers/ErrorReporter.hpp"
#include "../headers/Parser.hpp"

using namespace std;


int main(int argc, char** argv)
{
  ErrorReporter* errorReporter;

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

    if (errorReporter->getErrors() > 0)
    {
      errorReporter->endErrorsFile();
    }
  }
  else
  {
    cerr << "error: se esperaba un nombre de archivo a compilar" << endl;
    cerr << "uso:   vecomp <archivo>" << endl;
  }

  return errorReporter->getErrors() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

