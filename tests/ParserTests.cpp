#include <CLParser/CLParser.hpp>
#include <gtest/gtest.h>
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


TEST(ParserTestSuite, EmptyTest) {
    Parser parser;

    ASSERT_TRUE(parser.ParseArguments(SplitString("app")));
}


TEST(ParserTestSuite, StringTest) {
    Parser parser;
    parser.AddArgument<std::string>("param1");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetValue<std::string>("param1"), "value1");
}


TEST(ParserTestSuite, IntTest) {
    Parser parser;
    parser.AddArgument<int>("param1");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app --param1=100500")));
    ASSERT_EQ(parser.GetValue<int>("param1"), 100500);
}

TEST(ParserTestSuite, FlagsTest) {
    Parser parser;
    parser.AddArgument<bool>('a', "flag1");
    parser.AddArgument<bool>('b', "flag2").SetValue(true);

    ASSERT_TRUE(parser.ParseArguments(SplitString("app -a")));
    ASSERT_TRUE(parser.GetValue<bool>("flag1"));
    ASSERT_TRUE(parser.GetValue<bool>("flag2"));
}


TEST(ParserTestSuite, ShortNameTest) {
    Parser parser;
    parser.AddArgument<std::string>('a', "param1");
    parser.AddArgument<int>('b', "param2");
    parser.AddArgument<bool>('c', "param3");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app -a value1 -b 15 -c")));

    ASSERT_EQ(parser.GetValue<std::string>("param1"), "value1");
    ASSERT_EQ(parser.GetValue<int>("param2"), 15);
    ASSERT_TRUE(parser.GetValue<bool>("param3"));
}


TEST(ParserTestSuite, DefaultTest) {
    Parser parser;
    parser.AddArgument<std::string>("param1").SetValue("value1");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app")));
    ASSERT_EQ(parser.GetValue<std::string>("param1"), "value1");
}


TEST(ParserTestSuite, NoDefaultTest) {
    Parser parser;
    parser.AddArgument<std::string>("param1");

    ASSERT_FALSE(parser.ParseArguments(SplitString("app")));
}

TEST(ParserTestSuite, MultiValueTest) {
    Parser parser;
    parser.AddMultiArgument<int>('p', "param1");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app --param1=1 -p 2 --param1=3")));
    const std::vector<int>& values = parser.GetValues<int>("param1");
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[1], 2);
    ASSERT_EQ(values[2], 3);
}


TEST(ParserTestSuite, MinCountMultiValueTest) {
    Parser parser;
    size_t MinArgsCount = 10;
    parser.AddMultiArgument<int>('p', "param1", MinArgsCount);

    ASSERT_FALSE(parser.ParseArguments(SplitString("app --param1=1 --param1=2 --param1=3")));
}

TEST(ParserTestSuite, IntPositionalArgTest) {
    Parser parser;

    parser.AddPositional<int>("Param1");

    ASSERT_TRUE(parser.ParseArguments(SplitString("app --Param1=1 2 3 4 5")));

    const std::vector<int>& values = parser.GetValues<int>("Param1");

    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}