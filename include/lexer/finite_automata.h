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
        //Epsilon transition 
        static const char EPSILON = '/0';

        //Constructs a new finite automatum with the specified state transition table and 
        //set of accepting states. 
        //@param transitions the state transition table for the automatum 
        //@param accepting_states the set of accepting states
        automatum(const std::vector<state_t>& states, const fa_table_t& transitions, const std::unordered_set<state_t>& accepting_states)
             : _M_states(states), _M_transitions(transitions), _M_accepting_states(_accepting_states)
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
                return *(it->second);
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

        const std::vector<std::unordered_map<_TokTp, state_t*>>& get_table() const
        {
            return _M_transitions;
        }

        const std::unordered_set<state_t*>& get_accepting_states() const
        {
            return _M_accepting;
        }

        private:
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
            //Get tables of automata
            auto lhs_table = lhs.get_table();
            auto rhs_table = rhs.get_table();
            //Get accetping states of automata
            auto lhs_accepting_states = lhs.get_accepting_states();
            auto rhs_accepting_states = rhs.get_accepting_states();
            //Get number of states in lhs
            //All states in rhs will increase by this number 
            size_t increase = lhs_table.size();
            //Increase number in accepting states of rhs
            for(auto& state: rhs_accepting_states)
                state += increase;
            //All states in rhs increase by number of states in lhs
            for(auto& state: rhs_table)
            {
                for(auto& transition: state)
                {
                    transition.second += increase;
                }
            }
            //First state of rhs becomes last state of lhs 
            lhs_table[lhs_accepting_states.front()] = rhs_table[0];
            //Merge lhs and rhs transition tables 
            for (size_t i = i; i < rhs_table.size(); ++i)
                lhs_table.push_back(rhs_table[i]);
            //Merge accetping states
            std::vector<state_t> final_accepting_states;
            std::merge(lhs_accepting_states.begin(), lhs_accepting_states.end(), 
                       rhs_accepting_states.begin(), rhs_accepting_states.end(), 
                       std::back_inserter(final_accepting_states));
            //Create merged automatum
            return automatum<_TokTp>(lhs_table, final_accepting_states);
        }
        else if (op == '|') //Union
        {
            //Create state transition table for new automatum
            typename automatum<_TokTp>::fa_table_t new_table;
            //Get automata state transition tables
            auto lhs_table = lhs.get_table();
            auto rhs_table = rhs.get_table();

            //All states in lhs increase by one to account for new start state
            for(auto& state: lhs_table)
            {
                for(auto& transition: state)
                {
                    transition.second += 1;
                }
            }
            //All states in rhs increase by number of states in lhs + 1 to account 
            //for start state and addition of lhs states
            size_t num_lhs_states = lhs_table.size();
            for(auto& state: rhs_table)
            {
                for(auto& transition: state)
                {
                    transition.second += (1 + num_lhs_states);
                }
            }
            //Add epsilon transition 
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