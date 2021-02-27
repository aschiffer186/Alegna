#ifndef LEXER_H
#define LEXER_H

#include "finite_automata.h"
#include <string>
#include <ostream>
#include <variant>

namespace alegna::lexer
{
    class lexer
    {
        typedef automata::automatum<char> dfa_t;
        typedef unsigned int index_t;
        public:
            enum class tok_type 
            {
                eInt, 
                eFloat, 
                ePlus, 
                eMinus, 
                eStar,
                eSlash, 
                eCarrot, 
                eLpar, 
                eRpar,
                eEOF = 0,
                eIdentifier, 
                eEq,
                eVar,
                eError = -1
            };

            struct token
            {
                typedef std::variant<int, double, bool, char, std::string> value_type;
                typedef tok_type token_type;

                tok_type _M_type;
                index_t _M_line;
                index_t _M_start_col;
                index_t _M_end_col;
                value_type _M_value;

                friend std::ostream& operator<<(std::ostream& os, const token& t);
            };
        public:
            //Creates a new lexer that uses the specified 
            //deterministic finite automatum (DFA) to lex source text.
            //
            //@param dfa the DFA used to lex the source text.
            lexer(const dfa_t& dfa, const std::unordered_map<state_t, tok_type>& tok_types);

            //Creates a new lexer that uses the specified 
            //deterministic finite automatum (DFA) to lex the specified 
            //source text.
            //
            //@param dfa the DFA used to lex the source text. 
            //@param src the source text to be lexed
            lexer(const dfa_t& dfa, const std::unordered_map<state_t, tok_type>& tok_types, const std::string& src);

            //Sets the source text for the lexer to source 
            //and resets the lexer to the beginning of the source.
            //
            //@param src the source to lex
            void set_src(const std::string& src);

            //Sets the source text for the lexer to source 
            //and resets the lexer to the beginning of the source.
            //Uses move semantics.
            //
            //@param src the source to lex
            void set_src(std::string&& src) noexcept;

            //Lexes the source text and returns a vector 
            //contaning the tokens in the source text.
            //
            //@return a vector containing the tokens of the source text
            std::vector<token> lex();
        private:
            //Determines the next token in the src text.
            //
            //@return the next token in the source text
            token next_token();

            token make_token(automata::state_t s, const std::string& value) const;

            //Returns the character amt ahead of the current 
            //position in the lexer (or the EOF token if 
            //amt + the lexer's current position >= than 
            //the length of the source text). Does not 
            //advance the lexer.
            //
            //@param amt the number of characters to look ahead.
            //@return the character amt ahead of the current 
            //        position of the lexer.
            char lookahead(index_t amt) const;

        private:
            index_t _M_pos;
            index_t _M_col;
            index_t _M_line;
            std::string _M_src;
            std::unordered_map<automata::state_t, tok_type> _M_tok_types;
            dfa_t _M_dfa;
    };
}

#endif