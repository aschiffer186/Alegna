#include "lexer/lexer.h"

namespace alegna::lexer
{
    using state_t = automata::state_t;

    void lexer::set_src(const std::string& src)
    {
        _M_pos = 0;
        _M_line = 0;
        _M_col = 0;
        _M_src = src;
    }

    void lexer::set_src(std::string&& src) noexcept
    {
        _M_pos = 0;
        _M_line = 0;
        _M_col = 0;
        _M_src = src;
    }

    lexer::token lexer::next_token()
    {
        automata::state_t curr_state = 0;
        std::string value = "";
        while (true)
        {
            char c = _M_src[_M_pos];
            if (!isspace(c))
                value += c;
            curr_state = _M_dfa.delta(curr_state, c);
            if (_M_dfa.is_accepting_state(curr_state))
                return make_token(curr_state, value);
            if(curr_state == automata::automatum<char>::ERROR)
                int i = 0; //Handle error
            else if (_M_pos == _M_src.length())
                int i = 0; //Missing characters to make a token 
            advance();
        }
    }

    lexer::token lexer::make_token(state_t s, const std::string& value) const 
    {
       auto tok_type_it = _M_tok_types.find(s);
       if (tok_type_it != _M_tok_types.end())
       {
           auto tok_type = tok_type_it->second;
           switch (tok_type)
           {
                case lexer::tok_type::eInt:
                    return lexer::token{tok_type, _M_line, _M_col - value.length(), _M_col, std::stoi(value)};
                case lexer::tok_type::eFloat:
                    return lexer::token{tok_type, _M_line, _M_col - value.length(), _M_col, std::stoi(value)};
                default:
                    return lexer::token{tok_type, _M_line, _M_col - value.length(), _M_col, value};
           }
       }
    }

    char lexer::lookahead(index_t amt) const 
    {
        if (_M_pos + amt >= _M_src.length())
            return 0;
        return _M_src[_M_pos + amt];
    }

    void lexer::advance()
    {
        if (_M_pos >= _M_src.length()) return;
        if (_M_src[_M_pos] == '\n')
        {
           ++_M_line;
            _M_col = 0;
        }
        else 
        {
            ++_M_col;
        }
        ++_M_pos;
    }
}