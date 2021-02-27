#ifndef EXCEPTIONS_H 
#define EXCPETIONS_H 1

#include <exception>
#include <string>

namespace alegna::exceptions
{
    //An that is thrown when a file isn't found
    struct file_not_found_exception : public std::exception
    {
        explicit file_not_found_exception(const std::string& filename);

        const char* what() const noexcept;

        private:
            std::string _M_message;
    };

    struct invalid_regex_exception : public std::exception 
    {
        invalid_regex_exception();

        const char* what() const noexcept;
    };

    struct unexpected_token_exception : public std::exception 
    {
       explicit unexpected_token_exception(const std::string& tok);

       const char* what() const noexcept;

       private:
        std::string _M_message;
    };
}

#endif