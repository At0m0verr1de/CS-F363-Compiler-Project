cd ./src

gcc lexer.c ../DataStructures/dict.c -o lexer

echo "Enter the source code file name:"
read source_file

./lexer "$source_file"
