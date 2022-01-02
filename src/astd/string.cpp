#include "../../spman/astd/string.hpp"

namespace astd {

    astd::container_s split(std::string str, char to_find) {
        astd::container_s two_str;
        std::string First, Second;
        int i = 0;
        for(; str[i] != to_find && i < str.size(); i++) {
            First += str[i];
        }
        if(str[i] != to_find) {
            two_str.push(str);
            two_str.push("");
            return two_str;
        }
        i++;
        for(int j = i; j < str.size(); j++) {
            Second += str[j];
        }
        two_str.push(First);
        two_str.push(Second);
        return two_str;
    }

    astd::container_s split_end(std::string str, char to_find) {
        astd::container<std::string> two_str;
        std::string First, Second;
        int i = str.size();
        for(; str[i] != to_find && i > 0; i--);
        if(str[i] != to_find) {
            two_str.push(str);
            two_str.push("");
            return two_str;
        }
        for(int j = 0; j < i; j++) First += str[j];
        i++;
        for(int j = i; j < str.size(); j++) {
            Second += str[j];
        }
        two_str.push(First);
        two_str.push(Second);
        return two_str;
    }
}