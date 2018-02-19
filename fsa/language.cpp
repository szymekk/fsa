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

Fsa::Fsa::arc_index_t findTransition(const Fsa::Fsa::iterator& iter, const Fsa::Arc::label_t letter) {
    auto idx = iter.getIdx();
    // traverse the current state in search of a transition
    while (true) {
        if (0 == idx) { // end state should not be reached
            return 0;
        }

        auto const & transition = iter.arcs[idx];
        if (letter == transition.label_) { // found transition
            return idx;
        }

        if (transition.is_last_) { // last arc of this node reached
            return 0;
        } else { // check next arc
            ++idx;
        }
    };
}

} // namespace

namespace Fsa {

/*
traverse the automaton using iterative depth first search
to print all words belonging to the right language of the given state
the printed words are ordered lexicographically
*/
void printRightLanguage(const Fsa::iterator & language, std::ostream& stream) {

    std::stack<std::tuple<Fsa::arc_index_t, std::string, bool>> stack;
    std::string word;

    stack.push({ language.getIdx(), "", false });

    while (!stack.empty()) {
        using tuple_t = typename decltype(stack)::value_type;
        std::tuple_element<0, tuple_t>::type idx;
        std::tuple_element<2, tuple_t>::type print;
        std::tie(idx, word, print) = stack.top();
        stack.pop();
        const auto state = std::cbegin(language.arcs) + idx;

        stream << (print ? (word + '\n') : "");

        if (Fsa::TERMINAL_NODE != idx) {// don't expand from the terminal state
            pushState(stack, word, state);
        }
    }
}

void printAcceptedLanguage(const Fsa & fsa, std::ostream& stream) {
    printRightLanguage(fsa.getIterator(), stream);
}

// try advancing the iterator using given label
// if successfully advanced return index of the transition used
Fsa::arc_index_t Fsa::iterator::advance(Arc::label_t letter) {
    auto transition_idx = findTransition(*this, letter);
    if (0 == transition_idx) { // letter is not a prefix of any word in the language
        return 0;
    }

    const auto& transition = this->arcs[transition_idx];
    this->node_idx = transition.target_;

    return transition_idx;
}

bool accepts(const Fsa::iterator & language, const std::string word) {
    auto it = language;
    Fsa::arc_index_t result = 0;
    for (const char letter : word) {
        result = it.advance(letter);
        if (0 == result) {
            return false;
        }
    }
    bool is_accepting_transition = language.arcs[result].is_final_;
    return is_accepting_transition;
}

} // namespace