#!/bin/sh

cd parser
make
cd ..
cd code-generator
make
cd ..
mv ./parser/Parser Parser
mv ./code-generator/Code-Generator Code-Generator
