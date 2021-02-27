#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H 1

#include <istream>
#include <string>
#include <deque>
#include <vector>
#include <optional>

namespace alegna::lexer::regex
{
    //A class that converts regular expressions from infix notation 
    //to postfix notation.
    class regex_parser
    {
        public:
            //Creates a new regex_parser that will read regex from 
            //the specified std::istream.
            //
            //@param is the std::istream the regex will be read from
            explicit regex_parser(std::istream& is) noexcept;

            //Converts the regular expression currently referenced by 
            //the stored std::istream from infix notation to postfix.
            //Advances the stored std::istream object further.
            //
            //If the stored std::istream object is past the end of its 
            //stream, returns nothing. 
            //
            //May throw a std::invalid_regex_exception if it the regex it
            //is attempting to parse is not a valid regex.
            //
            //@return a postfix representation of the regular expression
            //        currently referenced by the stored std::istream object.
            std::optional<std::vector<char>> parse_regex();

            //Converts all regular expressions in the stream represented 
            //by the stored std::istream object from infix notation to 
            //postfix notation.  
            //
            //May throw a std::invalid_regex_exception if the regex it is 
            //attempting to parse is not a vaid regex.
            //
            //@return a vector containing postfix representations of all of 
            //        the regular expressions in the stored stream.
            std::vector<std::vector<char>> parse();
        
        private:
            //Preprocesses a regex string to make it easier to 
            //analyze. Expands a character class [A-Z] into (A|B|...|Z).
            //@param str the regex to be preprocessed
            void preprocess(std::string& str);

            //Returns the priority of a character.
            //@param c the character whose priority will be returned
            //@return the priority of the specified character.
            int get_priority(char c) const;
        private:
           const static char __epsilon = '\0';
        private:
           std::istream& _M_in;
    };
}

#endif