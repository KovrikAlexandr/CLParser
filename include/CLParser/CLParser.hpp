#pragma once

#include <CLParser/Arguments.hpp>
#include <iostream>
#include <algorithm>
#include <sstream>

namespace CLParser {

/*
    The function takes a string as an argument, separates it by "space"
    and returns the vector of received words
*/
std::vector<std::string> SplitString(const std::string& str);


class Parser {
public:

    Parser();

    ~Parser();

    bool Parse(int argc, char* argv[]);

    bool Parse(const std::vector<std::string>& arguments);

    // Single arguments

    template <typename T>
    Argument<T>& AddArgument(const char* long_name) {
        CLParser::Argument<T>* argument = new CLParser::Argument<T>(long_name);
        m_single_arguments.push_back(argument);

        return *argument;
    }

    template <typename T>
    Argument<T>& AddArgument(char short_name, const char* long_name) {
        CLParser::Argument<T>* argument = new CLParser::Argument<T>(short_name, long_name);
        m_single_arguments.push_back(argument);
        
        return *argument;
    }

    template <typename T>
    const T& GetValue(char short_name) {
        BaseArgument* base_ptr = FindSingleArgument(short_name);
        if (base_ptr == nullptr || base_ptr->GetTypeInfo() != typeid(T)) {
            // exception
            std::cerr << "error!!\n";
            exit(EXIT_FAILURE);

        } else {
            Argument<T>* argument = dynamic_cast<Argument<T>*>(base_ptr);

            return argument->GetValue();
        }
    }

    template <typename T>
    const T& GetValue(const char* long_name) {
        BaseArgument* base_ptr = FindSingleArgument(long_name);
        if (base_ptr == nullptr || base_ptr->GetTypeInfo() != typeid(T)) {
            // exception
            std::cerr << "error!!\n";
            exit(EXIT_FAILURE);
            
        } else {
            Argument<T>* argument = dynamic_cast<Argument<T>*>(base_ptr);

            return argument->GetValue();
        }
    }

    // Multivalue arguments

    template <typename T>
    MultiArgument<T>& AddMultiArgument(const char* long_name, uint32_t min_args_amount = 0) {
        MultiArgument<T>* argument = new MultiArgument<T>(long_name, min_args_amount);
        m_multi_arguments.push_back(argument);

        return *argument;
    }

    template <typename T>
    MultiArgument<T>& AddMultiArgument(char short_name, const char* long_name, uint32_t min_args_amount = 0) {
        MultiArgument<T>* argument = new MultiArgument<T>(short_name, long_name, min_args_amount);
        m_multi_arguments.push_back(argument);

        return *argument;
    }

    template <typename T>
    const std::vector<T>& GetValues(const char* long_name) {
        BaseArgument* base_ptr = FindMultiArgument(long_name);

        if (base_ptr == nullptr || base_ptr->GetTypeInfo() != typeid(T)) {
            // exception
            std::cerr << "error!!\n";
            exit(EXIT_FAILURE);

        } else {
            MultiArgument<T>* argument = dynamic_cast<MultiArgument<T>*>(base_ptr);

            return argument->GetValues();
        }
    }

    template <typename T>
    const std::vector<T>& GetValues(char short_name) {
        BaseArgument* base_ptr = FindMultiArgument(short_name);
        if (base_ptr == nullptr || base_ptr->GetTypeInfo() != typeid(T)) {
            // exception
            std::cerr << "error!!\n";
            exit(EXIT_FAILURE);

        } else {
            MultiArgument<T>* argument = dynamic_cast<MultiArgument<T>*>(base_ptr);

            return argument->GetValues();
        }
    }

    // Positional

    template <typename T>
    void AddPositional(const char* long_name, uint32_t min_args_amount = 0) {
        if (m_positional != nullptr) {
            std::cerr << "Can`t store more than one positional\n";
            exit(EXIT_FAILURE);
        }
        m_positional = new MultiArgument<T>(long_name, min_args_amount);
    }

    template <typename T>
    void AddPositional(char short_name, const char* long_name, uint32_t min_args_amount = 0) {
        if (m_positional != nullptr) {
            std::cerr << "Can`t store more than one positional\n";
            exit(EXIT_FAILURE);
        }
        m_positional = new MultiArgument<T>(short_name, min_args_amount);
    }

private:

    std::vector<BaseArgument*> m_single_arguments;
    std::vector<BaseArgument*> m_multi_arguments;
    BaseArgument* m_positional;

    BaseArgument* FindSingleArgument(char key) const;

    BaseArgument* FindSingleArgument(const char* key) const;

    bool SetSingleArgument(BaseArgument* base_ptr, const char* value);

    bool SetMultiArgument(BaseArgument* base_ptr, const char* value);

    BaseArgument* FindMultiArgument(char key);

    BaseArgument* FindMultiArgument(const char* key);

    bool AllInitialized();
};

}