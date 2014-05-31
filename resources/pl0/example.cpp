#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
#include <sstream>
#include <cstring>
using namespace std;


struct simbolos {
    string nom;
    string clase;
    string tipo;
    string dimen1;
    string dimen2;
};

struct locales {
    string clase;
    string tipo;
    string dimen1;
    string dimen2;
    string procp;
};

struct codigo {
    string mnemo;
    string dir1;
    string dir2;
};

codigo progra[10000];
simbolos tabSim[10000];

fstream aFuente;
ofstream aObjeto;

string lexema= "", token = "", lex= "", nomf="";
char fileArray[800000], linea[100];
const int ERR = -1, ACP = 999;
int conVar=0, conE=0;
int indx = 0, reng = 1, colo = 1, codn=1, simn=0;
bool banP=false, banMain=false;
string dim1, dim2;
const int matrans[36][30] = {
//      let      0         1-9      oct      hex      +        -        *         /          %      &       =       <        >       .           :      ;           ,        {      }        [       ]     sp      tab      nl        |      "       !     (        )
/*X       ACP,  ACP,    ACP,    ERR,    ERR,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP, */
/*0*/   1,       9,         2,      ERR,     ERR,    8,        8,       8,       30,        8,      26,		18,		14,     16,		 13,		20,      13,        13,     13,     13,     13,     13,     0,      0,      0,      28,      24,    22,     13,     13,
/*1*/   1,       1,         1,      ERR,     ERR,    ACP,      ACP,	    ACP,     ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,     ACP,		ACP,	ACP,	ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,     ACP,	ACP,	ACP,    ACP,
/*2*/   5,       2,         2,      ERR,     ERR,    ACP,	   ACP,		ACP,     ACP,		ACP,	ACP,    ACP,	ACP,    ACP,	 3,			ACP,     ACP,		ACP,    ACP,	ACP,    ACP,    ACP,    ACP,    ACP,    ACP,    ACP,     ACP,   ACP,	ACP,	ACP,
/*3*/   ERR,     4,         4,      ERR,     ERR,    ERR,	   ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*4*/   5,       4,         4,      ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*5*/   ERR,     7,         7,      ERR,	 ERR,    6,		   6,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*6*/   ERR,     7,         7,      ERR,     ERR,    ERR,	   ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*7*/   ACP,     7,		    7,      ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*8*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	    ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*9*/   10,		 2,		    2,		12,      2,		 2,		   2,		2,		 2,			2,		2,		2,		2,		2,		 2,			2,		 2,			2,		2,		2,		2,		2,		2,		2,		2,		2,		 2,		2,		2,		2,
/*10*/  ERR,	 ERR,		ERR,	ERR,	 11,	 ERR,	   ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*11*/  ACP,	 ACP,		ACP,	ERR,	 11,     ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*12*/  ACP,	 ACP,		ACP,	12,      ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*13*/  ACP,	 ACP,		ACP,	ERR,	 ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*14*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	15,		ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*15*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*16*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	17,		ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*17*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*18*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,	   ACP,		ACP,	 ACP,		ACP,	ACP,	19,		ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*19*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*20*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	21,		ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*21*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*22*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	23,		ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,    ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*23*/  ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*24*/   24,     24,		24,		ERR,     ERR,    24,       24,		24,		 24,		24,		24,		24,		24,		24,		 24,		24,		 24,		24,		24,		24,		24,		24,		24,		24,		24,		24,		 25,	24,		24,		24,
/*25*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*26*/   ERR,	 ERR,		ERR,	ERR,     ERR,    ERR,      ERR,		ERR,	 ERR,		ERR,	27,		ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*27*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ERR,	 ERR,	ERR,	ERR,	ERR,
/*28*/   ERR,	 ERR,		ERR,	ERR,     ERR,	 ERR,      ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	 ERR,		ERR,	 ERR,		ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	ERR,	29,		 ERR,	ERR,	ERR,	ERR,
/*29*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,     ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*30*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		33,		 31,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*31*/   31,	 31  ,		31,     ERR,     ERR,    31,       31,		31,		 31,		31,		31,		31,		31,		31,		 31,		31,		 31,		31,		31,		31,		31,		31,		31,		31,		32,		31,		 31,	31,		31,		31,
/*32*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP,
/*33*/   33,	 33,		33,     ERR,     ERR,    33,       33,		34,		 33,		33,		33,		33,		33,		33,		 33,		33,		 33,		33,		33,		33,		33,		33,		33,		33,		33,		33,		 33,	33,		33,		33,
/*34*/   33,	 33,		33,     ERR,     33,     33,       33,		34,		 35,		33,		33,		33,		33,		33,		 33,		33,		 33,		33,		33,		33,		33,		33,		33,		33,		33,		33,		 33,	33,		33,		33,
/*35*/   ACP,	 ACP,		ACP,	ERR,     ERR,    ACP,      ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,		ACP,	 ACP,		ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	ACP,	 ACP,	ACP,	ACP,	ACP
};

const string palres[] = {
    "break", "case", "chan", "const", "continue", "default", "defer", "else", "fallthrough", "for", "func", "go", "goto", "if", "import", "interface",
    "map", "package", "range", "return", "select", "struct", "switch", "type", "var", "bool", "int", "int8", "uint8", "float32", "nil",
    "string","div","mod"
};

const string ctelog[] = {"true", "false"};

int carCol(int estado, char c) {
        if (c == '_' || c == '\'' || c == '\\') return 0;
        if (c == '+' ) return 5;
        if (c == '-' ) return 6;
        if (c == '*' ) return 7;
        if (c == '/' ) return 8;
        if (c == '%' ) return 9;
        if (c == '&' ) return 10;
        if (c == '=' ) return 11;
        if (c == '<' ) return 12;
        if (c == '>' ) return 13;
        if (c == '.' ) return 14;
        if (c == ':' ) return 15;
        if (c == ';' ) return 16;
        if (c == ',' ) return 17;
        if (c == '{' ) return 18;
        if (c == '}' ) return 19;
        if (c == '[' ) return 20;
        if (c == ']' ) return 21;
        if (c == ' ' ) return 22;
        if (c == '\t' ) return 23;
        if (c == 13 ) return 24;
        if (c == 10) return 24;
        if (c == '\n' ) return 24;
        if (c == '|' ) return 25;
        if (c == 34 ) return 26;
        if (c == '!' ) return 27;
    if( c == '(') return 28;
    if( c == ')') return 29;
    if( (estado == 2 || estado == 4)&& toupper(c) == 'E') return 1;
    else if( estado == 9 || estado == 12) {
            if( estado == 9 && toupper(c) == 'X' ) return 0;
            if( c >=48 && c <= 55) return 3;
    }
    else if( estado == 10 || estado == 11) {
        if( c >= 48 && c<=57 ) return 4;
        c = toupper(c);
        if( c >=65 && c <=70) return 4;
        if( c >=71 && c <=90) return 1;
    }
    else if ( estado != 9 && estado != 10 && estado != 11 and estado != 12) {
        c = toupper(c);
        if( c >= 65 && c <=90 ) return 0;
        if ( c == '0' ) return 1;
        if ( c >=49 && c<=57 ) return 2;
    }
    if( estado == 24) return 0;
    cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Simbolo ilegal " << c << endl;
    return ERR;
}
bool esPalRes() {
    for(int i=0; i < 32; i++)
        if( palres[i] == lexema ) return true;
    return false;
}
bool esCteLog() {
    if(ctelog[0] == lexema) return true;
    else if(ctelog[1] == lexema) return true;
}
void loadFile() {
    char archE[80], archS[80];
    int idx = 0;
    char c;

    cout << "Archivo a Compilar: ";
    cin >> archE;
    strncpy(archS, archE, strlen(archE) -  2);
    archS[strlen(archE)-2] = '\0';
    strcat(archS, "eje");

    aFuente.open(archE, ios::in | ios::binary );
    aObjeto.open(archS);
    if( aFuente == NULL ) return;
    if( aObjeto == NULL ) return;
    while(  !aFuente.eof() )
        fileArray[idx++] = aFuente.get();

    fileArray[idx] = EOF;
    int i =  0, j=0;
    while(fileArray[i]!=EOF) {
        if (colo == 1) cout << "[" << reng << "] ";
        linea[j++] = fileArray[i];
        colo++;
        if ( fileArray[i] == 10 || fileArray[i + 1] == EOF) {
            colo = 1;
            reng++;
            linea[j++] = '\0';
            cout << linea;
            linea[0] = '\0';
            j = 0;
        }
        i++;
    }
    cout << endl << endl;
}

string lexico() {
    int estado = 0, estAnt = 0;
    lexema = "";
    while( estado != ERR && estado != ACP
          && fileArray[indx] != EOF) {
         char c = fileArray[indx++];
         if ( c == 10 ) {
            colo = 0;
            reng++;
         }
         else if( c != 13 ) colo++;
         int col = carCol(estado, c);
         if (estado != ERR && estado != ACP) {
                estAnt = estado;
                estado = matrans[estado][col];
                if (estado == 31 || estado == 33 ) lexema = "";
                if (estado != ERR && estado != ACP &&
                    estado != 0  && estado != 31 && estado != 33 &&
                    estado != 32 && estado != 34 && estado != 35) lexema = lexema + c;
         }
         if ( (estAnt == 32 || estAnt == 35) && estado == ACP) estado = 0;
    }
    if (estado == ACP || estado == ERR) {
            indx--;
            colo--;
    }
    token = "NoToke";
    if (estado == 0) lexema = "** End Of File **";
    switch( estAnt) {
        case 1: token = "identi";
                    if ( esPalRes() )  token = "palres";
                    else if( esCteLog() ) token = "conlog";
                    break;
        case 2: case 9: case 12: token="conent";
                                                     break;
        case 4: case 7:  token="conrea";
                                     break;
        case 25: token = "conalf";
                                      break;
        case 13: token = "delimi";
                                    break;
        case 8: case 14: case 15: case 16: case 17:
        case 22: case 23: case 27: case 29: case 30:
        case 19: case 21:token = "operad";
                        break;
    }
    return lexema;
}
//Generacion de Código
void genCod(string m, string d1, string d2) {
    progra[codn].mnemo = m;
    progra[codn].dir1 = d1;
    progra[codn++].dir2 = d2;

}
void insTabSim(string nmb, string cla, string tip, string d1, string d2) {
    tabSim[simn].nom = nmb;
    tabSim[simn].clase = cla;
    tabSim[simn].tipo = tip;
    tabSim[simn].dimen1 = d1;
    tabSim[simn++].dimen2 = d2;
}
//Verificacion Semántica
stack<string> ptipos;
const string cTipo[] = { "E:=E", "A:=A", "R:=R", "L:=L", "R:=E",
                                         "E+E", "E+R", "R+E", "R+R", "A+A",
                                         "E-E", "E-R", "R-E", "R-R",
                                         "E*E", "E*R", "R*E", "R*R",
                                         "E/E", "E/R", "R/E", "R/R",
                                         "E%E", "-E", "-R",
                                         "L&&L", "L||L", "!L",
                                         "E>E", "R>E", "E>R", "R>R",
                                         "E<E", "R<E", "E<R", "R<R",
                                         "E>=E", "R>=E", "E>=R", "R>=R",
                                         "E<=E", "R<=E", "E<=R", "R<=R",
                                         "E!=E", "R!=E", "E!=R", "R!=R", "A!=A",
                                         "E==E", "R==E", "E==R", "R==R", "A==A"
};
const string tipoR[] = { "", "", "", "", "",
                                          "E", "R", "R", "R", "A",
                                          "E", "R", "R", "R",
                                          "E", "R", "R", "R",
                                          "R", "R", "R", "R",
                                          "E", "E", "R",
                                          "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L", "L",
                                          "L", "L", "L", "L", "L"
};

int buscaTabSim(string nomv) {
     for(int i=0; i < simn; i++)
         if( tabSim[i].nom == nomv) return i;
     return -1;
}
int buscaTipo(string cadt) {
    for(int i=0; i < 54; i++)
        if(cTipo[i]==cadt) return i;
    return -1;
}
//Analizador Sintáctico
void expr(); //Prototipo de Expr
string tvar;
string nvar;

string convertInt(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

void tipo() {
    if( lex == "int" or lex == "int8" or lex == "float32" or lex == "string" or
         lex == "float64" or lex == "uint8" or lex == "bool" ) {
             if(lex == "int" || lex == "int8" || lex == "uint8") tvar="E";
             else if( lex == "float32" || lex == "float64" ) tvar = "R";
             else if( lex == "string") tvar = "A";
             else if( lex == "bool") tvar = "L";

             lex = lexico();
    }
    else {
              cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [tipo de dato] y encontro " + lex << endl;
            lex = lexico();
    }
}

void params(){
    stack<string> pilap;
    string nomp;
    do {
        if( lex == ",") lex = lexico();
        if( token != "identi") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [identificador ] y encontro " + lex << endl;
        }
        nomp = lex;
        pilap.push(nomp);
        lex = lexico();
        tipo();
        insTabSim(nomp, "P", tvar, "0", "0");
    } while (lex == ",");
    while(!pilap.empty()) {
        string par = pilap.top();
        pilap.pop();
        genCod("STO", "0", par);
    }
}
int conDim = 0;
void dimens() {
    do {
         lex = lexico();
         if( token != "conent") {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [constante entera] y encontro " + lex << endl;
         }
         else {
                conDim++;
                if( conDim == 1) dim1 = lex;
                else if (conDim == 2) dim2 = lex;
         }
         lex = lexico();
         if ( lex != "]" ) {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [cerrar corchete ']' y encontro " + lex << endl;
         }
         lex = lexico();
    } while ( lex == "[");
}

void vars() {
    lex = lexico();
    string nvar;
    if ( token == "identi" ) {
        nvar = lex;
        lex = lexico();
        dim1 = "0"; dim2 = "0";
        conDim = 0;
        if ( lex == "[") {
                dimens();
        }
        tipo();
        insTabSim(nvar, "V", tvar, dim1, dim2);
    }
    else if ( lex == "(") {
        lex = lexico();
        do {
            if( token != "identi") {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [identificador] y encontro " + lex << endl;
            }
            nvar = lex;
            lex = lexico();
            dim1="0"; dim2 = "0";
            conDim=0;
            if ( lex == "[") dimens();
            tipo();
            insTabSim(nvar, "V", tvar, dim1, dim2);
        } while( token == "identi" );
        if ( lex != ")" ) {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [parentesis ')'] y encontro " + lex << endl;
        }
        lex = lexico();
    } else {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [identificador o '(' ] y encontro " + lex << endl;
    }
}

void uparams() {
    do {
        if(lex == ",") {
            if (banP) genCod("OPR", "0", "20");
            lex = lexico();
        }
        expr();
    }while( lex == ",");
}

void callf() {
    lex = lexico();
    if( lex != ")")  uparams();
    if( lex != ")") {
          cout << "[" << reng << "]" << "[" << colo << "] "
                    << " Se esperaba [cerrar parentesis ')'] y encontro " + lex << endl;
    }
    lex = lexico();
}
void udimen() {
    lex = lexico();
    expr();
    if( lex != "]" ) {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [ cerrar corchetes ']' ] y encontro " + lex << endl;
    }
    lex = lexico();
}
void asigna() {
    if( lex == "[") udimen();
    if( lex != ":=") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [ := ] y encontro " + lex << endl;
    }
    lex = lexico();
    expr();
    string tp = ptipos.top(), tp1;
    ptipos.pop();
    tp1=ptipos.top();
    ptipos.pop();
    tp = tp1+":="+tp;
    int i;
    if( (i = buscaTipo(tp) ) >= 0)
        ptipos.push(tipoR[i]);
    else {
             ptipos.push("I");
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Conflicto en tipos en asignacion <"<< tp << ">" << endl;
    }
    genCod("STO", "0", nvar);
}
void term() {
     if( lex == "(") {
        lex = lexico();
        expr();
        if( lex != ")" ) {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [cerrar parentesis ')' ] y encontro " + lex << endl;
        }
        lex = lexico();
     }
     else if ( token == "conlog" || token == "conent" ||
                    token == "conrea" || token == "conalf") {
            if( token == "conent" || token == "conrea") {
                 if(lex == "0 ") lex = "0";
                 genCod("LIT", lex, "0");
            }
            if(token=="conalf")
                 genCod("LIT", lex, "0");
            if(token == "conlog") ptipos.push("L");
            else if( token == "conent") ptipos.push("E");
            else if( token == "conrea") ptipos.push("R");
            else if( token == "conalf") ptipos.push("A");
            if(token == "conlog") {
                  if(lex == "false") genCod("LIT", "F", "0");
                  else if(lex == "true") genCod("LIT", "V", "0");
            }
            lex = lexico();
     }
     else if( token == "identi") {
            string ide = lex;
            int i;
            if( (i = buscaTabSim(ide) ) >=0) {
                 ptipos.push(tabSim[i].tipo);
            }
            else {
                  cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Identificador " + lex + " NO ha sido declarado" << endl;
                  ptipos.push("I");
            }
            lex = lexico();
            if (lex == "(" ) {
                 string Ex = "_E" + convertInt(conE++);
                 genCod("LOD", Ex, "0");
                 callf();
                 genCod("CAL", nomf, "0");
                 insTabSim(Ex, "I", "I", convertInt(codn), "0");
            }
            genCod("LOD", ide, "0");
     } else {
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Se esperaba [se esperaba abrir '(', constante, lamada a Funcion, true o false ] y encontro " + lex << endl;
                lex = lexico();
     }
}

void opSig() {
         string op="";
         if (lex == "-") {
                op = "-";
                lex = lexico();
         }
         term();
        if( op == "-" ) {
            string tp = ptipos.top();
            ptipos.pop();
            tp = op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
        }
         if( op == "-") genCod("OPR", "0", "8");
}

void opMul() {
    string op;
    do {
        if( lex == "*" || lex == "/" || lex == "%" ) {
                op = lex;
                lex = lexico();
        }
         opSig();
        if( op == "*" || op == "/" || op == "%") {
            string tp = ptipos.top(), tp1;
            ptipos.pop();
            tp1=ptipos.top();
            ptipos.pop();
            tp = tp1+op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
        }
        if( op == "*" ) genCod("OPR", "0", "4");
        if( op == "/" ) genCod("OPR", "0", "5");
        if( op == "%" ) genCod("OPR", "0", "6");
    } while( lex == "*" || lex == "/" || lex == "%");
}
void opSum() {
    string op="";
    bool bin=false;
    do {
        if( (lex == "+" || lex == "-") and bin ) {
                op = lex;
                lex = lexico();
         }
         opMul();
        if( op == "+" || op == "-") {
            string tp = ptipos.top(), tp1;
            ptipos.pop();
            tp1=ptipos.top();
            ptipos.pop();
            tp = tp1+op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
        }
        if( op == "+" ) genCod("OPR", "0", "2");
        else if (op == "-" ) genCod("OPR", "0", "3");
        if (lex == "+"|| lex == "-") bin = true;
    } while( lex == "+" || lex == "-");
}
void opRel() {
         string op;
         opSum();
         if( lex == "<" || lex == ">" || lex == "<=" || lex == ">=" ||
              lex == "==" || lex == "!=") {
              op = lex;
              lex = lexico();
              opSum();
              if( op == "<" || op == ">=" || op == "<=" || op == ">=" ||
                   op == "!=" || op == "==") {
                    string tp = ptipos.top(), tp1;
                    ptipos.pop();
                    tp1=ptipos.top();
                    ptipos.pop();
                    tp = tp1+op+tp;
                    int i;
                    if( (i = buscaTipo(tp) ) >= 0)
                        ptipos.push(tipoR[i]);
                    else {
                        ptipos.push("I");
                        cout << "[" << reng << "]" << "[" << colo << "] "
                                 << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
                    }
              }
              if ( op == "<") genCod("OPR", "0", "9");
              else if ( op == ">")   genCod("OPR", "0", "10");
              else if ( op == "<=") genCod("OPR", "0", "11");
              else if ( op == ">=") genCod("OPR", "0", "12");
              else if ( op == "!=")  genCod("OPR", "0", "13");
              else if ( op == "==") genCod("OPR", "0", "14");
        }
}
void opN() {
         string op = "";
         if( lex == "!" ) {
                op = "!";
                lex = lexico();
         }
         opRel();
        if( op == "!" ) {
            string tp = ptipos.top();
            ptipos.pop();
            tp = op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
        }
         if (op == "!") {
            genCod("OPR", "0", "17");
         }
}
void opy() {
    string op="";
    do {
        if( lex == "&&") {
                op = lex;
                lex = lexico();
        }
        opN();
        if( op == "&&") {
            string tp = ptipos.top(), tp1;
            ptipos.pop();
            tp1=ptipos.top();
            ptipos.pop();
            tp = tp1+op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
        }
        if( op == "&&") genCod("OPR", "0", "15");
    } while( lex == "&&");
}
void expr() {
    string op;
    do {
         if( lex == "||") {
                op = lex;
                lex = lexico();
         }
         opy();
         if( op == "||" ) {
            string tp = ptipos.top(), tp1;
            ptipos.pop();
            tp1=ptipos.top();
            ptipos.pop();
            tp = tp1+op+tp;
            int i;
            if( (i = buscaTipo(tp) ) >= 0)
                ptipos.push(tipoR[i]);
            else {
                ptipos.push("I");
                cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Conflicto en tipos en la operacion " << op <<" <"<< tp << ">" << endl;
            }
         }
         if( op == "||") {
            genCod("OPR", "0", "15");
         }
    } while( lex == "||");
}
void regresa() {
    lex = lexico();
    if( token == "identi" || token == "conent" ||
         token == "conlog" || token == "conrea" ||
         token == "conalf" ||
         lex == "(" || lex == "false" || lex == "true") {
             expr();
             genCod("STO", "0", nomf);
    }
    genCod("OPR", "0", "1");
}
void libF() {
    lex = lexico();
    if( token != "identi") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [Identificador de Funcion Librerira] y encontro " + lex << endl;
    }
    if( lex == "Println") banP = true;
    lex = lexico();
    if( lex != "(") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [abrir parentesis '('] y encontro " + lex << endl;
            lex = lexico();
    }
    else {
            lex = lexico();
            uparams();
    }
    if( lex != ")") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [cerrar parentesis ')'] y encontro " + lex << endl;
    }
    if ( banP )
        genCod("OPR", "0", "21");
    banP = false;
    lex = lexico();
}
void estatuto();
void block();


void ife() {
    lex = lexico();
    expr();
     string ne = convertInt(conE++), Ex, Ey;
     ne = "_E"+ ne;
     Ex = ne;
     genCod("JMC", "F", Ex );
    if(lex != "else") {
        block();
    }

    ostringstream codnBuilder;
    if(lex == "else") {
        ne = convertInt(conE++);
        ne = "_E"+ ne;
        Ey = ne;
        char s[12];
        genCod("JMP", "0", Ey);
        codnBuilder << codn;
        insTabSim(Ex, "I", "I", codnBuilder.str(), "0" );
        lex = lexico();
        block();
        insTabSim(Ey, "I", "I", codnBuilder.str(), "0" );
    }
    else {
        char s[12];
        insTabSim(Ex, "I", "I", codnBuilder.str(), "0" );
    }
}

void fore() {
    lex = lexico();
    if ( lex != ";") estatuto();
    if(lex != ";") {
                     cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [punto y coma ';'] y encontro " + lex << endl;
    }
    lex = lexico();
    if( lex != ";") expr();
    if(lex != ";") {
                     cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [punto y coma ';'] y encontro " + lex << endl;
    }
    lex = lexico();
    if (lex != "{") estatuto();
    block();
}
void estatuto() {
    if ( token == "identi")  {
         nvar = lex;
         lex = lexico();
         if(lex == ".")
             insTabSim(nvar, "L", "I", "0", "0");
        int i;
        if( (i = buscaTabSim(nvar) ) >=0) {
                if(lex != ".") ptipos.push(tabSim[i].tipo);
         } else {
                  cout << "[" << reng << "]" << "[" << colo << "] "
                         << " Identificador " + nvar + " NO ha sido declarado" << endl;
                  ptipos.push("I");
         }
         if ( lex == "(" )  callf();
         else if ( lex == "[" || lex == ":=") {
            asigna();
         }
         else if ( lex == "." ) {
            libF();
         }
    }
    else if( lex == "return" ) regresa();
    else if( lex == "for" ) fore();
    else if( lex == "if") ife();
}
void comandos(){
    do {
         if( lex == ";") lex = lexico();
         if( lex != ";") estatuto();
    } while ( lex == ";");
}
void block() {
        if( lex != "{") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [llave {] y encontro " + lex << endl;
         }
         lex = lexico();
         if(lex != "}") comandos();
        if( lex != "}") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [llave }] y encontro " + lex << endl;
         }
         lex = lexico();
}
void funcs() {
    do {
        lex = lexico();
        if( token != "identi") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [Identificador] y encontro " + lex << endl;
         }
         if(lex == "main") {
            banMain = true;
            char *s = new char(20);
            ostringstream codnBuilder;
            codnBuilder << codn;
            insTabSim("_P", "I", "I", codnBuilder.str(), "0");
         }
         else {
            nomf = lex;
         }
         lex = lexico();
        if( lex != "(") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [parentesis (] y encontro " + lex << endl;
         }
         lex = lexico();
         int linf = codn;
         if( lex != ")") params();
         if( lex != ")") {
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [parengtesis )] y encontro " + lex << endl;
         }
         lex = lexico();
         if( lex == "{" ) {
            block();
         }
         else {
                char s[5];
                tipo();
                ostringstream linfBuilder;
                linfBuilder << linf;
                insTabSim(nomf, "F", tvar, linfBuilder.str(), "0");
                block();
         }
    } while (lex == "func" );
    if (banMain)  genCod("OPR", "0", "0");
}

void imports() {
    lex = lexico();
    if( token == "conalf") lex = lexico();
    else if ( lex == "(" ) {
        lex = lexico();
        do {
             if( token != "conalf") {
                   cout << "[" << reng << "]" << "[" << colo << "] "
                             << " Se esperaba [constante Alfabetica] y encontro " + lex << endl;
             }
             lex = lexico();
        } while ( token == "conalf");
        if( lex != ")" ){
             cout << "[" << reng << "]" << "[" << colo << "] "
                      << " Se esperaba [parentesis )] y encontro " + lex << endl;
         }
         lex = lexico();
    }
    else {
                   cout << "[" << reng << "]" << "[" << colo << "] "
                             << " Se esperaba [constante Alfabetica] y encontro " + lex << endl;
    }
}

void prgm() {
    lex = lexico();
    if( lex != "package") {
        cout << "[" << reng << "]" << "[" << colo << "] "
                  << " Se esperaba [package] y encontro " + lex << endl;
    }
    lex = lexico();
    if( lex != "main") {
        cout << "[" << reng << "]" << "[" << colo << "] "
                  << " Se esperaba [main] y encontro " + lex << endl;
    }
    lex = lexico();
    do {
         if( lex == "import" ) imports();
    }while ( lex == "import");
    do {
            if( lex == "var" ) vars();
    } while( lex == "var");
    do {
      if( lex == "func") funcs();
    } while ( lex == "func");
}


void Sintaxis() {
    prgm();
}

void grabaTabla() {
    for(int i=0; i < simn; i++)
    {
        aObjeto << tabSim[i].nom << ","
                << tabSim[i].clase << ","
                << tabSim[i].tipo << ","
                << tabSim[i].dimen1 << ","
                << tabSim[i].dimen2 << ",#,"
                << endl;
    }
    aObjeto << "@" << endl;
}

void grabaCod() {
    for(int i=1; i < codn; i++)
        aObjeto << i << " " << progra[i].mnemo << " " << progra[i].dir1 << ", " << progra[i].dir2 << endl;
}

int main() {
    loadFile();
    reng = 1; colo = 1;
    Sintaxis();
    grabaTabla();
    grabaCod();
    aObjeto.flush();
    aObjeto.close();
    return EXIT_SUCCESS;
}

