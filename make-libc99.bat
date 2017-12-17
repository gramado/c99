:Setup
echo off

rem
rem Compila a biblioteca padrão C99 para o ambiente user mode.
rem

:Compiling
    echo { Compiling C library for 32bit User Mode ...
	gcc -c stdio.c  -o stdio.o
	gcc -c stdlib.c -o stdlib.o
	gcc -c string.c -o string.o
	rem ...
	
:Moving
    echo { Moving objects ...
    move stdio.o obj/stdio.o
    move stdlib.o obj/stdlib.o
    move string.o obj/string.o
    rem ...
	
:End
    pause
