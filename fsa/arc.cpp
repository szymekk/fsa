#include "arc.h"

#include <boost/functional/hash/hash.hpp>

namespace Fsa {

Arc::Arc(unsigned char label, int target, bool is_final, bool is_last)
    : target_(target)
    , label_(label)
    , is_final_(is_final)
    , is_last_(is_last)
{ }


Arc::~Arc()
{ }

std::size_t hash_value(const Arc& a) {
    std::size_t seed = 0;
    boost::hash_combine(seed, a.target_);
    boost::hash_combine(seed, a.label_);
    boost::hash_combine(seed, a.is_final_);
    return seed;
}

} // namespace Fsa
