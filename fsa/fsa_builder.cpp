#include "fsa_builder.h"

#include <string>
#include <algorithm>
#include <cassert>
#include <tuple>

#include <boost/functional/hash/hash.hpp>

namespace {
const size_t PATH_OFFSET = 1;
const size_t DUMMY_STATE_INDEX = 0;
} // namespace

namespace Fsa {
FsaBuilder::FsaBuilder()
    : arcs_{ Arc('T', 1337, true,  true) }
    , path_{ Arc('D', 2222,false,true) }
    , path_index_{ DUMMY_STATE_INDEX, PATH_OFFSET } {
}

void FsaBuilder::build(std::istream& input) {
    std::string word;
    while (input >> word) {
        add(word);
    }
    replace_or_register(DUMMY_STATE_INDEX);
    root_ = path_[DUMMY_STATE_INDEX].target_;
}

Fsa FsaBuilder::getFsa() {
    return Fsa(arcs_, root_);
}

// find common prefix of given string and the last inserted string
std::string FsaBuilder::commonPrefix(const std::string& new_word) const {
    assert(path_index_.size() >= 2);
    if (2 == path_index_.size()) { // only dummy state on path
        return "";
    }

    auto getLastLabel = [this](const int common) {
        //last arc of the common state
        const auto last_arc = this->traversePath(common + 1) - 1;
        return this->path_[last_arc].label_;
    };

    size_t common = 0;
    // number of states in path
    // (skip dummy state and last index for the next potential state)
    const auto state_count = path_index_.size() - 2;
    while (getLastLabel(common) == new_word[common] && ++common < state_count) {
    }

    return new_word.substr(0, common);
}

// traverse path skipping the dummy state and specified number of states
FsaBuilder::arc_index FsaBuilder::traversePath(const size_t to_skip) const {
    return path_index_[to_skip + 1];
}

// state is an index into path_
bool FsaBuilder::hasChildren(const arc_index state) const {
    assert(std::find(path_index_.cbegin(), path_index_.cend(), state) != path_index_.cend());
    if (path_index_.size() <= 2) {
        return false;
    }
    auto it = path_index_.cend();
    // an element has children if
    // it is not one of the two last elements of path_index_
    // last element of path_index is equal to path_.size()
    // the element before last denotes the first arc of the last state on path_
    return *--it != state && *--it != state;
}

void FsaBuilder::add(const std::string & word) {
    std::string common_prefix = commonPrefix(word);
    std::string suffix = word.substr(common_prefix.size());
    assert(common_prefix + suffix == word);

    const auto last_common_state = traversePath(common_prefix.size());// index into path_
    // common_prefix == last_added_word
    bool make_new_state = (path_.size() == last_common_state);
    if (hasChildren(last_common_state)) {
        replace_or_register(last_common_state);
    }
    //addSuffixToPath
    if (make_new_state) {//when commonPrefix == last added word
        path_index_.push_back(path_.size() + 1); // for state to be added
    } else {//expand last common state
        path_.back().is_last_ = false;
        ++path_index_.back();
    }
    path_.push_back(Arc(suffix[0], Fsa::TERMINAL_NODE, false, true));

    for (const auto ch : suffix.substr(1)) {
        path_.push_back(Arc(ch, Fsa::TERMINAL_NODE, false, true));
        path_index_.push_back(path_index_.back() + 1);
        assert(path_.size() == path_index_.back());
    }
    path_.back().is_final_ = true;
}

void FsaBuilder::replace_or_register(const arc_index state) {
    auto it = std::find(path_index_.cbegin(), path_index_.cend(), state);
    assert(it != path_index_.cend() || state == DUMMY_STATE_INDEX);
    assert(hasChildren(state));
    auto child_idx = *++it;
    if (hasChildren(child_idx)) {
        replace_or_register(child_idx);
    }

    auto child_first = path_.cbegin() + child_idx;
    auto child_last = path_.cend();

    auto last_arc_idx = child_idx - 1;
    auto& last_arc = path_[last_arc_idx];
    // check for child_idx >= path_.size() for empty string or no input
    auto search = registry.find({ &path_, child_idx });
    if (registry.cend() != search) {
        last_arc.target_ = search->index;
    } else {
        auto idx = arcs_.size();
        last_arc.target_ = idx;
        arcs_.insert(arcs_.cend(), child_first, child_last);

        bool success;
        std::tie(std::ignore, success) = registry.insert({ &arcs_, idx });
        assert(success);
    }
    assert(path_.cend() == child_last);
    // remove child from path
    path_.erase(child_first, child_last);
    path_index_.pop_back();
    assert(path_.size() == path_index_.back());
    assert(path_.begin() + last_arc_idx == path_.end() - 1);
}

std::size_t FsaBuilder::StateHasher::operator()(const State & key) const {
    size_t seed = 0;
    auto iter = std::cbegin(*key.store) + key.index;
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
    auto l_iter = std::cbegin(*lhs.store) + lhs.index;
    auto r_iter = std::cbegin(*rhs.store) + rhs.index;

    bool last_arc;
    do {
        if (!arcEqual(*l_iter, *r_iter))
            return false;
        last_arc = l_iter->is_last_;
        l_iter++, r_iter++;
    } while (!last_arc); // break after the last arc

    return true;
}
} // namespace Fsa
