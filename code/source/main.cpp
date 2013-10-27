/* Main function definition for vecomp program */

#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "../headers/main.hpp"
#include "../headers/Parser.hpp"

using namespace std;


int main(int argc, char** argv) {
  Scanner scanner;
  int errors = 0, warnings = 0;

  if (argc == 2) {
#ifdef DEBUG
    cout << argc << " "  << argv[0] << " " << argv[1] << endl;
#endif

    Parser parser;
    parser.parse(argv[1]);

    errors += parser.getErrors();
    warnings += parser.getWarnings();

    if (errors > 0 || warnings > 0)
      cout << "\nCompilacion terminada con " << errors << 
          " errores, " << warnings << " advertencias" << endl;

  }
  else {
    cerr << "error: se esperaba un nombre de archivo a compilar" << endl;
    cerr << "uso:   vecomp <filename>" << endl;
    errors++;
  }

  return errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

