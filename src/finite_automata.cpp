#include "lexer/finite_automata.h"
#include <deque>

namespace alegna::lexer::automata
{
    automatum<char> construct_nfa(const std::vector<std::vector<char>>& regex)
    {
        
    }

    automatum<char> construct_sub_nfa(std::vector<char>& regex)
    {
        std::deque<automatum<char>> nfa_stack;
    }
}