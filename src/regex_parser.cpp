#include "lexer/regex_parser.h"
#include "exceptions/exceptions.h"
#include <fstream>
#include <regex>
#include <algorithm>
#include <iterator>

namespace alegna::lexer::regex
{
    regex_parser::regex_parser(std::istream& is)
        : _M_in(is)
    {
        
    }

    std::optional<std::vector<char>> regex_parser::parse_regex()
    {
        if(!_M_in)
            return std::optional<std::vector<char>>();
        std::deque<char> op_stack;
        std::vector<char> out;
        std::string regex;
        std::getline(_M_in, regex);
        preprocess(regex);
        for(const auto c: regex)
        {
            int priority = get_priority(c);
            if (priority == -1)
                out.push_back(c);
            else if (priority == 3)
                op_stack.push_front(c);
            else if (priority == 1)
            {
                if (op_stack.empty())
                    throw alegna::exceptions::invalid_regex_exception();
                while (op_stack.front() != ')' && !op_stack.empty())
                {
                    out.push_back(op_stack.front());
                    op_stack.pop_front();
                }
                if (op_stack.empty())
                    throw alegna::exceptions::invalid_regex_exception();
                op_stack.pop_front();
            }
            else 
            {
                while(!op_stack.empty() && get_priority(op_stack.front()) > priority)
                {
                    out.push_back(op_stack.front());
                    op_stack.pop_front();
                }
                op_stack.push_front(c);
            }
        }

        while(!op_stack.empty())
        {
            out.push_back(op_stack.front());
            op_stack.pop_front();
        }
        return std::optional<std::vector<char>>(out);
    }

    std::vector<std::vector<char>> regex_parser::parse()
    {
        std::vector<std::vector<char>> regex;
        while(_M_in)
        {
            auto ex = parse_regex();
            if (ex)
                regex.push_back(*ex);
        }
    }

    void regex_parser::preprocess(std::string& regex)
    {
        //Strings to hold the preprocessed regex
        std::string result_num;
        std::string result_lower;
        std::string result_upper;
        std::string final_result;

        //Regexes of sections to replace
        std::regex num_re("\\[0-9\\]");
        std::regex lower_re("\\[a-z\\]");
        std::regex upper_re("\\[A-Z]");
        std::regex all_re("\\[A-Za-z\\]");

        //Strings representing replacements
        std::string num_replacement = "(0|1|2|3|4|5|6|7|8|9)";
        std::string lower_replacement = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)";
        std::string upper_replacement;
        std::transform(lower_replacement.begin(), lower_replacement.end(), back_inserter(upper_replacement), [](char c) {return toupper(c);});
        std::string all_replacement = lower_replacement.substr(0, lower_replacement.size() - 1) + "|" + upper_replacement.substr(1);
        
        //Replace [0-9]
        std::regex_replace(back_inserter(result_num), regex.begin(), regex.end(), num_re, num_replacement);
        //Replace [a-z]
        std::regex_replace(back_inserter(result_lower), result_num.begin(), result_num.end(), lower_re, lower_replacement);
        //Replace [A-Z]
        std::regex_replace(back_inserter(result_upper), result_lower.begin(), result_upper.begin(), upper_re, upper_replacement);
        //Replace [A-Za-z]
        std::regex_replace(back_inserter(final_result), result_upper.begin(), result_upper.end(), all_re, all_replacement);

        //Insert concatenation operators
        for(size_t i = 0; i < final_result.size() - 1; ++i)
        {
            if(!is_operator(final_result[i]))
            {
                if(final_result[i] != '(' && final_result[i+1] != ')')
                    final_result.insert(i++, "?");
            }
            else if(final_result[i] == ')' && !is_operator(final_result[i]))
                final_result.insert(i++, "?");
            else if (final_result[i] == '*')
                final_result.insert(i++, "?");
        }
    }

    bool is_operator(char c)
    {
        return c == '*' || c == '|';
    }

    int regex_parser::get_priority(char c) const
    {
        if (c == '(')
            return 4;
        else if (c == '*')
            return 3;
        else if (c == '?')
            return 2;
        else if (c == '|')
            return 1;
        else if (c == ')')
            return 0;
        else 
            return -1;
    }
}