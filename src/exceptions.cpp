#include "exceptions/exceptions.h"

namespace alegna::exceptions
{
    file_not_found_exception::file_not_found_exception(const std::string& filename)
    {
        _M_message = "Could not open file " + filename + ".";
    }

    const char* file_not_found_exception::what() const noexcept 
    {
        return _M_message.c_str();
    }

    invalid_regex_exception::invalid_regex_exception()
    {

    }

    const char* invalid_regex_exception::what() const noexcept
    {
        std::string message = "Invalid regex expression";
        return message.c_str();
    }

}