#include <CLParser/CLParser.hpp>
#include <CLParser/Arguments.hpp>


CLParser::Parser::Parser() 
    : m_positional(nullptr)
{
    m_single_arguments.reserve(5);
    m_multi_arguments.reserve(5);
}

CLParser::Parser::~Parser() {
    for (BaseArgument* base_argument : m_single_arguments) {
        delete base_argument;
    }

    for (BaseArgument* base_argument : m_multi_arguments) {
        delete base_argument;
    }

    if (m_positional != nullptr) {
        delete m_positional;
    }
}

CLParser::BaseArgument* CLParser::Parser::FindSingleArgument(char key) const {
    for (BaseArgument* argument : m_single_arguments) {
        if (key == argument->GetShortName()) {

            return argument;
        }
    }

    return nullptr;
}

CLParser::BaseArgument* CLParser::Parser::FindSingleArgument(const char* key) const {
    for (BaseArgument* argument : m_single_arguments) {
        if (std::strcmp(key, argument->GetLongName().c_str()) == 0) {
            
            return argument;
        }
    }

    return nullptr;
}

CLParser::BaseArgument* CLParser::Parser::FindMultiArgument(char key) {
    for (BaseArgument* argument : m_multi_arguments) {
        if (key == argument->GetShortName()) {
            
            return argument;
        }
    }

    if (m_positional == nullptr) {

        return nullptr;
    }

    if (m_positional->GetShortName() ==  key) {

        return m_positional;
    }

    return nullptr;
}

CLParser::BaseArgument* CLParser::Parser::FindMultiArgument(const char* key) {
    for (BaseArgument* argument : m_multi_arguments) {
        if (std::strcmp(key, argument->GetLongName().c_str()) == 0) {
            
            return argument;
        }
    }

    if (m_positional == nullptr) {

        return nullptr;
    }

    if (std::strcmp(m_positional->GetLongName().c_str(), key) == 0) {

        return m_positional;
    }

    return nullptr;
}

bool CLParser::Parser::AllInitialized() {
    for (BaseArgument* base_argument : m_single_arguments) {
        if (!base_argument->IsInit()) {

            return false;
        }
    }

    for (BaseArgument* base_argument : m_multi_arguments) {
        if (!base_argument->IsInit()) {

            return false;
        }
    }

    if (m_positional != nullptr) {
        if (!m_positional->IsInit()) {

            return false;
        }
    }

    return true;
}

bool CLParser::Parser::SetSingleArgument(BaseArgument* base_ptr, const char* value) {
    if (base_ptr->GetTypeInfo() == typeid(int)) {
        Argument<int>* arg_ptr = dynamic_cast<Argument<int>*>(base_ptr);
        arg_ptr->SetValue(std::stoi(value));

        return true;

    } else if (base_ptr->GetTypeInfo() == typeid(std::string)) {
        Argument<std::string>* arg_ptr = dynamic_cast<Argument<std::string>*>(base_ptr);
        arg_ptr->SetValue(value);

        return true;
    }

    return false;
}

bool CLParser::Parser::SetMultiArgument(BaseArgument* base_ptr, const char* value) {
    
    if (base_ptr->GetTypeInfo() == typeid(int)) {
        MultiArgument<int>* arg_ptr = dynamic_cast<MultiArgument<int>*>(base_ptr);
        arg_ptr->AddValue(std::stoi(value));

        return true;

    } else if (base_ptr->GetTypeInfo() == typeid(std::string)) {
        MultiArgument<std::string>* arg_ptr = dynamic_cast<MultiArgument<std::string>*>(base_ptr);
        arg_ptr->AddValue(value);

        return true;
    }

    return false;
}

bool CLParser::Parser::ParseArguments(int argc, char* argv[]) {
    std::vector<std::string> arguments(argc - 1);
    for (int arg_idx = 1; arg_idx < argc; arg_idx++) {
        arguments.emplace_back(argv[arg_idx]);
    }

    return ParseArguments(arguments);
}

bool CLParser::Parser::ParseArguments(const std::vector<std::string>& arguments) {
    for (int arg_idx = 1; arg_idx < arguments.size(); arg_idx++) {
        std::string argument(arguments[arg_idx]);
        
        if (argument[0] == '-') {
            // Not positional
            
            if (argument[1] == '-') {
                // long name

                int eq_idx = argument.find("=");
                if (eq_idx == argument.size()) {

                    return false;
                }
                if (eq_idx != std::string::npos) {
                    // not flag --param=<value>
                    
                    std::string key = argument.substr(2, eq_idx - 2);    
                    std::string value = argument.substr(eq_idx + 1);

                    BaseArgument* base_ptr = FindSingleArgument(key.c_str());

                    if (base_ptr != nullptr) {
                        SetSingleArgument(base_ptr, value.c_str());

                    } else {
                        // Search for multivalue argument

                        base_ptr = FindMultiArgument(key.c_str());
                        if (base_ptr == nullptr) {

                            return false;
                        }
                        SetMultiArgument(base_ptr, value.c_str());
                    }
                } else {
                    // --flag

                    std::string key = argument.substr(2);
                    BaseArgument* base_ptr = FindSingleArgument(key.c_str());

                    if (base_ptr == nullptr || base_ptr->GetTypeInfo() != typeid(bool)) {

                        return false;
                    }
                    Argument<bool>* arg_ptr = dynamic_cast<Argument<bool>*>(base_ptr);
                    arg_ptr->SetValue(true);
                }
            } else {
                // -p ?<value>

                if (argument.size() > 2) {
                    // -abc (flags)
                    for (uint32_t flag_num = 1; flag_num < argument.size(); flag_num++) {
                        BaseArgument* base_ptr = FindSingleArgument(argument[flag_num]);
                        if ((base_ptr == nullptr) || base_ptr->GetTypeInfo() != typeid(bool)) {

                            if (base_ptr == nullptr) {
                                
                                return false;
                            }
                        }
                        Argument<bool>* arg_ptr = dynamic_cast<Argument<bool>*>(base_ptr);
                        arg_ptr->SetValue(true);
                    }
                } else {
                    // -p ?<value>
                    char key = argument[1];

                    BaseArgument* base_ptr = FindSingleArgument(key);
                    if (base_ptr != nullptr) {
                        if (base_ptr->GetTypeInfo() == typeid(bool)) {
                            Argument<bool>* arg_ptr = dynamic_cast<Argument<bool>*>(base_ptr);
                            arg_ptr->SetValue(true);
                        } else {
                            arg_idx++;
                            SetSingleArgument(base_ptr, arguments[arg_idx].c_str());
                        }
                    } else {
                        base_ptr = FindMultiArgument(key);
                        if (base_ptr == nullptr) {

                            return false;
                        }
                        arg_idx++;
                        SetMultiArgument(base_ptr, arguments[arg_idx].c_str());
                    }
                }
            }
        } else {
            // Positional

            std::string value = argument;
            if (m_positional != nullptr) {
                SetMultiArgument(m_positional, arguments[arg_idx].c_str());
            } else {

                return false;
            }
        }
    }

    return AllInitialized();
}