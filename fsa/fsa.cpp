#include "fsa.h"

#include<iostream>
#include<stack>
#include<tuple>
#include<string>

namespace {
void pushState(std::stack<std::tuple<size_t, std::string, bool>>& stack,
        const std::string word,
        const std::vector<Fsa::Arc>::const_iterator first) {
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
    // TODO: consider changing to std::make_pair(first, w)
    stack.push({ first->target_, w, first->is_final_ });
};
} // namespace

namespace Fsa {

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

bool accepts(const Fsa & fsa, const std::string word) {
    auto i = fsa.root_;
    bool is_accepting_transition = true; // accept the empty string
    for (const char letter : word) {
        // traverse the current state in search of a transition
        while (true) {
            if (i == 0) // end state should not be reached
                return false;

            auto const & arc = fsa.arcs_[i];
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
traverse the automaton using iterative depth first search
to print all words belonging to the right language of the given state
the printed words are ordered lexicographically
the state is given by an index to its first Arc
*/
void printRightLanguage(const Fsa & fsa, const Fsa::arc_index_t state, std::ostream& stream) {

    std::stack<std::tuple<Fsa::arc_index_t, std::string, bool>> stack;
    std::string word;

    stack.push({ state, "", false });

    while (!stack.empty()) {
        using tuple_t = typename decltype(stack)::value_type;
        std::tuple_element<0, tuple_t>::type idx;
        std::tuple_element<2, tuple_t>::type print;
        std::tie(idx, word, print) = stack.top();
        stack.pop();
        const auto first = std::cbegin(fsa.arcs_) + idx;

        stream << (print ? (word + '\n') : "");

        if (Fsa::TERMINAL_NODE != idx) {// don't expand from the terminal state
            pushState(stack, word, first);
        }
    }
}

void printAcceptedLanguage(const Fsa & fsa, std::ostream& stream) {
    printRightLanguage(fsa, fsa.root_, stream);
}

} // namespace