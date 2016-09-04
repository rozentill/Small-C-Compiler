# Small-C-Compiler
The main idea and work explanation can be found in report.pdf. 
<p>All the files contain:Small.l,Small.y,treeNode.h,README.md,makefile,testcases,CodeGeneration.c,avlTree.h,checkReserved.c</p>
## How to run :
<p>Firstly,input "sh build.sh" in the root directory.</p>
<p>Secondly, ./Parser "Source File Name" "Output File Name" to generate syntax tree.</p>
<p>Thirdly, ./scc "Source File Name" "Output File Name" to generate llvm codes.</p>
<p>Last, lli xx.ll to run the program by llvm.</p>
