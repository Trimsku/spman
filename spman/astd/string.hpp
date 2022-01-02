#ifndef STRING_H
#define STRING_H

#include "container.hpp"
#include <string>

namespace astd {
    using container_s = astd::container<std::string>;
    astd::container_s split(std::string str, char to_find);
    astd::container_s split_end(std::string str, char to_find);
} // namespace astd

#endif //STRING_H