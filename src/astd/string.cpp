#include "../../spman/astd/string.hpp"

namespace astd {
    //Constructors:
    // Constructor with size:
    string::string(size_t size) : current_data(new char[size + 1]), length(size)
    {
        current_data[0] = '\0'; // Only setting the first char
    }

    // Constructor with str;
    string::string(const string &str) : string(str.current_data) {}
    string::string(string&& other) noexcept: current_data(std::exchange(other.current_data, nullptr)), length(other.length) {}
    // Constructor with str.
    string::string(const char* data) {
        length = strlen(data);
        current_data = new char[length + 1];
        std::memcpy(current_data, data, length + 1);
    }

    // Operators
    // Operator =, swap "this", str, to other.
    string& string::operator=(const astd::string& other) // copy assignment
    {
         return *this = astd::string(other);
    }
    string& string::operator=(astd::string&& other) noexcept // move assignment
    {
        length = other.length;
        std::swap(current_data, other.current_data);
        return *this;
    }
    // Operator +=(char)
    string &string::operator+=(char ch) {
        char* tmp = new char[length + 2];
        sprintf(tmp, "%s%c", current_data, ch);
        //std::move(current_data, current_data + length, tmp);
        delete[] current_data;
        current_data = new char[length + 2];
        sprintf(current_data, "%s", tmp);
        length++;
        delete[] tmp;
        /*char *ret = new char[len()+2];
        std::move(current_data, current_data + length, ret);
        ret[len()] = ch;
        ret[len()+1] = '\0';

        std::swap(ret, current_data);
        //delete[] ret;
        length++;*/
        return *this;
    }
    // Operator +=(const char*)
    string &string::operator+=(const char* data) {
        for(int i = 0; i < strlen(data); i++) {
            *this += data[i];
        }
        //std::memcpy(current_data, data, strlen(data));
        
        return *this;
    }
    // Operator +=(astd::string)
    string &string::operator+=(string data) {
        for(int i = 0; i < data.len(); i++) {
            *this += data[i];
        }
        return *this;
    }

    // Operator []. Return char on the string index.
    char &string::operator[](const int index) {
        if(0 <= index && index <= length) {
            return current_data[index];
        } else {
            printf("Error: wrong index, index is %s, index: %d, in %s\n", (index <= 0?"less than 0":"more than size()"), index, __BASE_FILE__);
            //printf("Your index is not correct. Error: %s in line: %d, length: %d", __FILE__, length);
            exit(1); 
        }
    }
    // Utils 
    // Return char*, i.e current_data.
    char *string::data() const {
        return current_data;
    }
    // Return int, without null-terminated char(\0).
    int string::len() const {
        return length;
    }
    // Return const char*, i.e (const char*)current_data.
    const char* string::c_str() const{
        return current_data;
    }
    
    // Destructor
    string::~string() { delete[] current_data; }
    string string::operator+(string str) {
        string result(*this);
        result += str;
        return result;
    }
    string string::operator+(const char* str) {
        string result(*this);
        result += str;
        return result;
    }

    bool string::operator==(astd::string str) {
        if(str.len() != len()) return false;
        for(int i = 0; i < length; i++) {
            if(str.current_data[i] != current_data[i]) return false;
        }
        return true;
    }
    bool string::operator!=(astd::string str) {
        return !(*this==str);
    }

    container<string> split(astd::string str, char to_find) {
        astd::container<string> two_str;
        astd::string First, Second;
        int i = 0;
        for(; str[i] != to_find && i < str.len(); i++) {
            First += str[i];
        }
        if(str[i] != to_find) {
            two_str.push(str);
            two_str.push("");
            return two_str;
        }
        i++;
        for(int j = i; j < str.len(); j++) {
            Second += str[j];
        }
        two_str.push(First);
        two_str.push(Second);
        return two_str;
    }

    astd::container_s split_end(astd::string str, char to_find) {
        astd::container<string> two_str;
        astd::string First, Second;
        int i = str.len();
        for(; str[i] != to_find && i > 0; i--);
        if(str[i] != to_find) {
            two_str.push(str);
            two_str.push("");
            return two_str;
        }
        for(int j = 0; j < i; j++) First += str[j];
        i++;
        for(int j = i; j < str.len(); j++) {
            Second += str[j];
        }
        two_str.push(First);
        two_str.push(Second);
        return two_str;
    }
}