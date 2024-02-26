#include <iostream>
#include <CLParser/CLParser.hpp>

#include <vector>
#include <string>
#include <iterator>
#include <sstream>

using namespace CLParser;

/*
    Функция принимает в качество аргумента строку, разделяет ее по "пробелу"
    и возвращает вектор полученных слов
*/
std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);

    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}


int main(int argc, char* argv[]) {
    Parser parser;

    parser.AddArgument<std::string>("param");

    if (!parser.ParseArguments(SplitString("app --param=abc"))) {
        std::cerr << "wrong input\n";
        return 1;
    }

    std::cout << parser.GetValue<std::string>("param");
}