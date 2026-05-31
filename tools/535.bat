bison135.exe --no-lines --defines -o parser.c SolveIt.y
copy  parser.c parser.cpp
flex5 -R -o"_scan.cpp"  -L SolveIt.lex
copy  __scan.cpp+_scan.cpp scan.cpp