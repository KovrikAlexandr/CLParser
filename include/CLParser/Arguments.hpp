#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <iostream>

namespace CLParser {

class BaseArgument {
public:
    BaseArgument(const char* long_name, const std::type_info& type);

    BaseArgument(char short_name, const char* long_name, const std::type_info& type);

    BaseArgument(const BaseArgument& other) = delete;

    virtual ~BaseArgument() = 0;

    const std::type_info& GetTypeInfo() const;

    char GetShortName() const;

    const std::string& GetLongName() const;

    bool IsInit();
    
private:
    char m_short_name;
    std::string m_long_name;

protected:
    const std::type_info& m_type;
    bool m_is_init;
};

template <typename T>
class Argument : public BaseArgument {
public:

    Argument(const char* long_name) 
        : BaseArgument(long_name, typeid(T))
    {}

    Argument(char short_name, const char* long_name)
        : BaseArgument(short_name, long_name, typeid(T))
    {}

    ~Argument() override = default;

    inline const T& GetValue() const {
        return m_value;
    }

    inline void SetValue(const T& value) {
        m_value = value;
        m_is_init = true;
    }

private:
    T m_value;
};


template <typename T>
class MultiArgument : public BaseArgument {
public:
    MultiArgument(const char* long_name, uint32_t min_args_amount = 0) 
        : BaseArgument(long_name, typeid(T)), m_min_args_amount(min_args_amount)
    {   
        m_values.reserve(5);
        if (min_args_amount > 0) {
            m_is_init = true;
        }
    }

    MultiArgument(char short_name, const char* long_name, uint32_t min_args_amount = 0) 
        : BaseArgument(short_name, long_name, typeid(T)), m_min_args_amount(min_args_amount)
    {
        if (min_args_amount == 0) {
            m_is_init = true;
        }
    }

    ~MultiArgument() override = default;

    inline const std::vector<T>& GetValues() const {
        return m_values;
    }

   inline void AddValue(const T& value) {
        m_values.push_back(value);
        if (m_values.size() >= m_min_args_amount) {
            m_is_init = true;
        }
    }

private:
    std::vector<T> m_values;
    uint32_t m_min_args_amount; 
};

}