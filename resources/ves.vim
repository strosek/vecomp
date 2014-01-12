" Vim syntax file
" Language: Ve (Spanish Go) 
" Maintainer: Erick Daniel Corona 
" Latest Revision: January 12, 2014 

if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword basicLanguageKeywords caso continua defecto desde funcion interfaz importar interrumpe ir ir_a regresa si sino selecciona tipo valor var  

" Matches
" Integer with - + or nothing in front
syn match celNumber '\d\+'
syn match celNumber '[-+]\d\+'

" Floating point number with decimal no E or e (+,-)
syn match celNumber '\d\+\.\d*'
syn match celNumber '[-+]\d\+\.\d*'

" Floating point like number with E and no decimal point (+,-)
syn match celNumber '[-+]\=\d[[:digit:]]*[eE][\-+]\=\d\+'
syn match celNumber '\d[[:digit:]]*[eE][\-+]\=\d\+'

" Floating point like number with E and decimal point (+,-)
syn match celNumber '[-+]\=\d[[:digit:]]*\.\d*[eE][\-+]\=\d\+'
syn match celNumber '\d[[:digit:]]*\.\d*[eE][\-+]\=\d\+'

" Regions
syn region syntaxElementRegion start='{' end='}'

