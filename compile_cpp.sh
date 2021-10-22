echo -e "\e[45mCompiling maptel module...\e[0m"
g++ -c -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o
echo -e "\e[45mCompiling maptel_test2 using g++...\e[0m"
g++ -c -Wall -Wextra -O2 -std=c++17 maptel_test2.cc -o maptel_test2.o
echo -e "\e[45mLinking 1->2...\e[0m"
g++ maptel.o maptel_test2.o -o maptel_test12
echo -e "\e[45mLinking 2->1...\e[0m"
g++ maptel_test2.o maptel.o -o maptel_test21
