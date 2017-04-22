#include "fsa_builder.h"

#include <string>

#include <boost/functional/hash/hash.hpp>


FsaBuilder::~FsaBuilder() {
}

void FsaBuilder::build(std::istream& input) {
    std::string word;
    while (input >> word) {
        add(word);
    }
    //add(""); //TODO: add the root state to the automaton
}

Fsa FsaBuilder::getFsa() {
    return Fsa(arcs_, root_);
}

void FsaBuilder::add(const std::string &) {
    //TODO
}


std::size_t FsaBuilder::StateHasher::operator()(const State & key) const {
    size_t seed = 0;
    auto iter = std::cbegin(key.store) + key.index;
    // hash arcs, form first to last inclusive
    do {
        boost::hash_combine(seed, *iter);
    } while (!iter++->is_last_); // break after combining the last arc

    return seed;
}

bool FsaBuilder::StateEqual::operator()(const State & lhs, const State & rhs) const {
    auto arcEqual = [](const Arc& first, const Arc& second) -> bool {
        return  first.label_    == second.label_    &&
                first.target_   == second.target_   &&
                first.is_final_ == second.is_final_ &&
                first.is_last_  == second.is_last_;
    };
    auto l_iter = std::cbegin(lhs.store) + lhs.index;
    auto r_iter = std::cbegin(rhs.store) + rhs.index;

    bool last_arc;
    do {
        if (!arcEqual(*l_iter, *r_iter))
            return false;
        last_arc = l_iter->is_last_;
        l_iter++, r_iter++;
    } while (!last_arc); // break after the last arc

    return true;
}

void replace_or_register(/*State*/) {
    //TODO;
}