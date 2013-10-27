/* Main function definition for vecomp program */

#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "../headers/main.hpp"
#include "../headers/Scanner.hpp"

using namespace std;


int main(int argc, char** argv) {
  Scanner scanner;
  int errors = 0, warnings = 0;

  if (argc == 2) {
#ifdef DEBUG
    cout << argc << " "  << argv[0] << " " << argv[1] << endl;
#endif
    scanner.scan(argv[1]);

    TokenLexeme tokenLexeme;
    size_t nTokens = scanner.getMaxTokens();
    if (nTokens > 0) {
      for (size_t i = 0; i < nTokens; ++i) {
        tokenLexeme = scanner.getNextTokenLexeme();
        cout << "Token: " << 
            setw(15) << tokenLexeme.getTokenString(tokenLexeme.getToken()) <<
            "  Lexema:   " << tokenLexeme.getLexeme() << endl;
      }
    }
  }

  errors += scanner.getErrors();

  if (errors > 0 || warnings > 0)
    cout << "\nCompilacion terminada con " << errors << 
        " errores, " << warnings << " advertencias" << endl;

  return errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

