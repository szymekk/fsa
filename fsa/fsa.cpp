#include "fsa.h"

#include<iostream>
#include<stack>


Fsa::Fsa(std::vector<Arc> arcs, std::vector<Arc>::size_type root)
try : arcs_(arcs)
    , root_(root >= 0 && root < arcs.size() ?
            root                            :
            throw std::invalid_argument(
                "root index should be a valid index into the arcs array")
    )
{ }
catch (const std::invalid_argument& e) {
    std::cerr << e.what() << '\n';
} // exception is automatically rethrown here

Fsa::~Fsa() {
}

bool Fsa::accepts(std::string word) {
    auto i = root_;
    bool is_accepting_transition = true; // accept the empty string
    for (const char letter : word) {
        // traverse the current state in search of a transition
        while (true) {
            if (i == 0) // end state should not be reached
                return false;

            auto const & arc = arcs_[i];
            if (letter == arc.label_) {
                is_accepting_transition = arc.is_final_;
                i = arc.target_;
                break;
            }

            if (arc.is_last_)
                return false;
            else
                i++;
        };
    }
    return is_accepting_transition;
}

/*
traverse the automaton using depth first search
to print all words in the language ordered lexicographically
*/
void Fsa::printAllWords(std::ostream& stream) {

    std::stack<std::tuple<decltype(root_), std::string, bool>> stack;
    auto it = std::cbegin(arcs_) + root_;
    std::string word;

    stack.push({ root_, "", false });
    

    while (!stack.empty()) {
        using tuple_t = typename decltype(stack)::value_type;
        std::tuple_element<0, tuple_t>::type idx;
        std::tuple_element<1, tuple_t>::type ww;
        std::tuple_element<2, tuple_t>::type print;
        //std::tie(idx, ww, print) = stack.top();
        std::tie(idx, word, print) = stack.top();
        stack.pop();
        it = std::cbegin(arcs_) + idx;

        stream << (print ? (word + '\n') : "");
        
        //TODO: remove the lambda
        auto pushState = [&stack, word, &first = std::as_const(it)] () {
            auto iter = first;
            while (!iter->is_last_) {
                ++iter;
            }

            std::string w;
            while (first != iter) {
                w = word;
                w.push_back(iter->label_);
                stack.push({ iter->target_, w, iter->is_final_ });
                --iter;
            };
            w = word;
            w.push_back(first->label_);
            stack.push({ first->target_, w, first->is_final_ });// TODO: change to std::make_pair(first, w)
        };

        //TODO: enum for terminal state
        if (0 != idx) // don't expand from the terminal state
            pushState();
    }
}
