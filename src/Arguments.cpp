#include <CLParser/Arguments.hpp>

// BaseArgument

CLParser::BaseArgument::~BaseArgument() {};

CLParser::BaseArgument::BaseArgument(const char *long_name, const std::type_info& type)
    : m_short_name(0), m_long_name(long_name), m_type(type), m_is_init(false)
{}

CLParser::BaseArgument::BaseArgument(char short_name, const char *long_name, const std::type_info& type)
    : m_short_name(short_name), m_long_name(long_name), m_type(type), m_is_init(false)
{}

const std::type_info& CLParser::BaseArgument::GetTypeInfo() const {
    return m_type;
}

char CLParser::BaseArgument::GetShortName() const {
    return m_short_name;
}

const std::string &CLParser::BaseArgument::GetLongName() const {
    return m_long_name;
}

bool CLParser::BaseArgument::IsInit() {
    return m_is_init;
}






