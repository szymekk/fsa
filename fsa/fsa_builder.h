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

    void build(std::istream& input);
    Fsa getFsa();

private:
    std::vector<Arc> arcs_;
    std::vector<Arc> path_;

    using arc_store = decltype(path_);
    using arc_index = arc_store::size_type;

    arc_index root_;
    
    std::vector<arc_index> path_index_{ 0 };
    
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
    // only states in the arcs_ vector are allowed to be register
    std::unordered_set<State, StateHasher, StateEqual> registry;

    void add(const std::string&);
    std::string commonPrefix(const std::string & new_word) const;
    arc_index traversePath(const std::string &) const;
    bool hasChildren(arc_index) const;

    void replace_or_register(arc_index);

};
#endif  // FSA_FSA_BUILDER_H_
