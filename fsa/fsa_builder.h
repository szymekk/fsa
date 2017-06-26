#ifndef FSA_FSA_BUILDER_H_
#define FSA_FSA_BUILDER_H_

#include <vector>
#include <unordered_set>

#include "arc.h"
#include "fsa.h"

class FsaBuilder {
public:
    FsaBuilder();

    void build(std::istream& input);
    Fsa getFsa();

private:

    using arc_store = std::vector<Arc>;
    using arc_index = arc_store::size_type;

    struct State {
        const arc_store* const store;
        const arc_index index;
    };
    struct StateHasher {
        std::size_t operator()(const State& key) const;
    };
    struct StateEqual {
        bool operator()(const State& lhs, const State& rhs) const;
    };

    std::string commonPrefix(const std::string &) const;
    arc_index traversePath(const size_t) const;
    bool hasChildren(arc_index) const;

    void add(const std::string&);
    void replace_or_register(arc_index);

    arc_store arcs_;
    arc_store path_;
    arc_index root_;
    std::vector<arc_index> path_index_{ 0 };
    // only states in the arcs_ vector are allowed to be register
    std::unordered_set<State, StateHasher, StateEqual> registry;
};
#endif  // FSA_FSA_BUILDER_H_
