#!/bin/sh

rm Parser Code-Generator
cd parser
make clean
make
cd ..
cd code-generator
make clean
make
cd ..
mv ./parser/Parser Parser
mv ./code-generator/Code-Generator scc
