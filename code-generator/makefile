Code-Generator:lex.yy.c y.tab.c treeNode.h
	gcc -o Code-Generator lex.yy.c y.tab.c 
lex.yy.c:y.tab.h Small-C.l
	flex Small-C.l
y.tab.c y.tab.h:Small-C.y treeNode.h
	yacc -d Small-C.y
clean:
	rm -rf *.o Parser

