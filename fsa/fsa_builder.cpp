#include "fsa_builder.h"

#include <string>

#include <boost/functional/hash/hash.hpp>


FsaBuilder::~FsaBuilder() {
}

Fsa FsaBuilder::build(std::istream input) {
    std::string word;
    while (input >> word) {
        add(word);
    }
    return Fsa(arcs_, 1);//TODO
}

void FsaBuilder::add(const std::string &) {
    //TODO
}


std::size_t FsaBuilder::StateHasher::operator()(const key_t & key) const {
    size_t seed = 0;
    key_t iter = key;
    // hash arcs, form first to last inclusive
    do {
        boost::hash_combine(seed, *iter);
    } while (!iter++->is_last_); // break after combining the last arc

    return seed;
}

bool FsaBuilder::StateEqual::operator()(const key_t & lhs, const key_t & rhs) const {
    auto arcEqual = [](const Arc& first, const Arc& second) -> bool {
        return  first.label_    == second.label_    &&
                first.target_   == second.target_   &&
                first.is_final_ == second.is_final_ &&
                first.is_last_  == second.is_last_;
    };
    key_t l_iter = lhs;
    key_t r_iter = rhs;

    bool last_arc;
    do {
        if (!arcEqual(*l_iter, *r_iter))
            return false;
        last_arc = l_iter->is_last_;
        l_iter++, r_iter++;
    } while (!last_arc); // break after the last arc

    return true;
}
