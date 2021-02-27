#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace alegna::lexer::automata
{
    //A type to represent a finite automatum state.
    typedef short state_t;

    //A struct that represents a finite automatum (FA). Contains information 
    //about the FA's state transition function (represented as a table) and the 
    //accepting states of the FA. Can represent a non-deterministic FA or a 
    //deterministic FA.
    //@param _TokTp the type of tokens used in the automatum. 
    template<typename _TokTp>
    struct automatum
    {
        //A type representing the type of tokens used in the automatum.
        typedef _TokTp token_type;
        //Convenience type to represent the table of the FA
        typedef std::vector<std::unordered_map<_TokTp, state_t>> fa_table_t;

        //The error state 
        static const state_t ERROR = -1;

        //Constructs a new finite automatum with the specified state transition table and 
        //set of accepting states. 
        //@param transitions the state transition table for the automatum 
        //@param accepting_states the set of accepting states
        automatum(const fa_table_t& transitions, const std::unordered_set<state_t>& accepting_states)
             : _M_transitions(transitions), _M_accepting_states(_accepting_states)
        {

        }

        //Finds the next state based on the current state and the 
        //token just read. If no valid transition exists, 
        //returns the error state.
        //@param s the FA's current state
        //@param tok the token just read.
        state_t delta(state_t s, const _TokTp& tok) const
        {
            if (s >= _M_transitions.size())
                return ERROR;
            auto row = _M_transitions(row);
            auto it = row.find(tok);
            if (it != row.end())
                return it->second;
            return ERROR;
        }

        //Returns true if the state is in the accepting states of the FA.
        //
        //@param s the state to be checked
        //@return true if the state is in the accepting state
        bool is_accepting_state(state_t s) const
        {
            return _M_accepting.find(s) != _M_accepting.end();
        }

        std::vector<std::unordered_map<_TokTp, state_t>> get_table() const
        {
            return _M_transitions;
        }

        std::unordered_set<state_t> get_accepting_states() const
        {
            return _M_accepting;
        }

        private:
            //The set of states of the automatum 
            std::unordered_set<state_t> _M_states;
            //The state transition table of the automatum
            std::vector<std::unordered_map<_TokTp, state_t>> _M_transitions;
            //The accepting states of the automatum
           std::unordered_set<state_t> _M_accepting;
    };

    //Given a set of regular expressions, constructs a non-deterministic
    //finite automatum (NFA) that represents the set of regular expressions. 
    //Uses Thompson's construction. 
    //
    //@param regex the set of regular expressions 
    //@return a non-deterministic finite automatum that represents the 
    //        regular expressions
    automatum<char> construct_nfa(const std::vector<std::vector<char>>& regex);

    template<typename _TokTp>
    automatum<_TokTp> merge_nfa(const automatum<_TokTp>& lhs, const automatum<_TokTp>& rhs, const std::string& op)
    {
        if (op == "?") //Concatenation
        {
            auto table_1 = lhs.get_table();
            auto table_2 = rhs.get_table();
            std::vector<state_t> lhs_accepting_states = lhs.get_accepting_states();
            std::vector<state_t> rhs_accepting_states = rhs.get_accepting_states();        
        }
    }

    //Given a non-deterministic finite automatic, constructs a 
    //deterministic finite automatum (DFA) that represents the same 
    //set of regular expressions. The DFA can be used in a lexer or 
    //parser.
    template <typename _TokTp>
    automatum<_TokTp> construct_dfa(const automatum<_TokTp>& nfa);
}


#endif