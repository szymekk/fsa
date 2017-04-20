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

    using arc_store = decltype(path_);
    using arc_index = decltype(path_)::size_type;
    
    std::vector<arc_index> states_on_path_;
    
    struct State {
        arc_store& store;
        arc_index index;
    };
    struct StateHasher {
        std::size_t operator()(const State& key) const;
    };
    struct StateEqual {
        bool operator()(const State& lhs, const State& rhs) const;
    };
    // only states in the arcs_ vector are allowed to be register
    std::unordered_set<State, StateHasher, StateEqual> registry;

    void add(const std::string&);

};
#endif  // FSA_FSA_BUILDER_H_
