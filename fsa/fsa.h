#ifndef FSA_FSA_H_
#define FSA_FSA_H_

#include <vector>
#include <iostream>
#include <stdexcept>

#include "arc.h"

namespace Fsa {

class Fsa {
public:
    using arc_store_t = typename std::vector<Arc>;
    using arc_index_t = arc_store_t::size_type;

    static const arc_index_t TERMINAL_NODE = 0;

    Fsa(std::istream& input);
    Fsa(std::vector<Arc>, std::vector<Arc>::size_type);

    struct iterator {
        const arc_store_t & arcs;

        iterator(const arc_store_t & in_arcs, const arc_index_t in_idx)
            : arcs{ in_arcs }, node_idx{ in_idx }
        { }

        arc_index_t advance(Arc::label_t label);
        arc_index_t getIdx() const { return node_idx; };
    private:
        arc_index_t node_idx;
    };

    iterator getIterator() const;

    const arc_store_t arcs_;
    const arc_index_t root_;
};

bool accepts(const Fsa::iterator & language, const std::string word);
void printRightLanguage(const Fsa::iterator & language, std::ostream& stream = std::cout);
void printAcceptedLanguage(const Fsa &, std::ostream & = std::cout);

} // namespace Fsa

#endif  // FSA_FSA_H_
