#ifndef FSA_FSA_BUILDER_H_
#define FSA_FSA_BUILDER_H_

#include <vector>
#include <unordered_set>

#include "arc.h"
#include "fsa.h"

class FsaBuilder {
public:
    FsaBuilder();
    ~FsaBuilder();

    Fsa build(std::istream input);

private:
    const std::vector<Arc> arcs_;
    const std::vector<Arc> path_;

    using arc_index = decltype(path_)::size_type;
    
    std::vector<arc_index> states_on_path_;
    
    using key_t = std::vector<Arc>::const_iterator;
    struct StateHasher {
        std::size_t operator()(const key_t& key) const;
    };
    struct StateEqual {
        bool operator()(const key_t& lhs, const key_t& rhs) const;
    };
    // only states in the arcs_ vector are allowed to be register
    std::unordered_set<std::vector<Arc>::const_iterator, StateHasher, StateEqual> registry;

    void add(const std::string&);

};
#endif  // FSA_FSA_BUILDER_H_
