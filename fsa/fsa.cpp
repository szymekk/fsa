#include "fsa.h"

#include<iostream>


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
