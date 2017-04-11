#ifndef FSA_FSA_BUILDER_H_
#define FSA_FSA_BUILDER_H_

#include <vector>
#include <istream>
#include <unordered_map>

#include "arc.h"
#include "fsa.h"

class FsaBuilder {
public:
    FsaBuilder();
    ~FsaBuilder();

    Fsa build(std::istream input);

private:
    std::vector<Arc> arcs_;
    std::vector<Arc> path_;

    using arc_index = decltype(path_)::size_type;
    
    struct State {
        std::vector<Arc>& store;
        arc_index first;
        arc_index last;
    };
    struct StateHasher {
        std::size_t operator()(const State& key) const;
    };
    struct StateEqual {
        bool operator()(const State& lhs, const State& rhs);
    };
    
    std::vector<arc_index> states_on_path_;
    std::unordered_map<State, arc_index, StateHasher, StateEqual> registry;

    void add(const std::string&);

};
#endif  // FSA_FSA_BUILDER_H_
