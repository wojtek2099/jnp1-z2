echo -e "\e[45mCompiling maptel module...\e[0m"
g++ -c -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o
echo -e "\e[45mCompiling maptel_test1 using gcc...\e[0m"
gcc -c -Wall -Wextra -O2 -std=c11 maptel_test1.c -o maptel_test1.o
echo -e "\e[45mLinking...\e[0m"
g++ maptel.o maptel_test1.o -o maptel_test1
