#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <stdlib.h>
#include <cassert>
#include <algorithm>
#include "container.hpp"
#include <memory>

namespace astd
{
class string
{
private:
    char* current_data;
    size_t length;
public:
    string(string&& other) noexcept;
    string(const string& other);
    //string(const astd::string &str);
    string(const char *data); // \/
    string(size_t size_ = 0); // \/
    ~string(); // \/
    string &operator+=(char ch); // \/
    string &operator+=(const char* ch); // \/
    string &operator+=(string str); // \/
    //string &operator=(string str); //  \/
    string& operator=(const astd::string& other);
    string& operator=(astd::string&& other) noexcept;
    string operator+(string str); // \/
    string operator+(const char* str); // X
    bool operator==(astd::string str);
    bool operator!=(astd::string str);
    char &operator[](const int index); // \/
    char *data() const; // \/
    int len() const; // \/
    const char* c_str() const; // \/
};
using container_s = astd::container<string>;
astd::container_s split(astd::string str, char to_find);
astd::container_s split_end(astd::string str, char to_find);
} // namespace astd

#endif //STRING_H