@ECHO OFF

cl.exe /nologo /c /Od /MT /W0 /GS- /Tc screenshot-dx.c
move /y screenshot-dx.obj screenshot-dx.o

