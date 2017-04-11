#ifndef FSA_ARC_H_
#define FSA_ARC_H_

#include <boost/functional/hash/hash.hpp>


class Arc {
public:
    Arc(unsigned char label, int target, bool is_final, bool is_last);
    ~Arc();

    int target_; // index of the first arc of the destination state in the State vector
    unsigned char label_;
    bool is_final_; // the Arc constitutes the last transition of at least one path forming a word
    bool is_last_; // last Arc going out of the state
};

// used by boost::hash_value
// not part of boost:: to enable overload resolution via argument dependant lookup
std::size_t hash_value(const Arc& a);

#endif  // FSA_ARC_H_
