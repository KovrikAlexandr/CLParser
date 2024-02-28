#include <iostream>
#include <CLParser/CLParser.hpp>

using namespace CLParser;

int main(int argc, char* argv[]) {
    Parser parser;
    parser.AddArgument<std::string>('p', "param");

    if (!parser.Parse(argc, argv)) {
        std::cerr << "error: wrong input\n";
        return 1;
    }

    std::cout << parser.GetValue<std::string>("param");
}