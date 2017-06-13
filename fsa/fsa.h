#ifndef FSA_FSA_H_
#define FSA_FSA_H_

#include <vector>
#include <iostream>
#include <stdexcept>

#include "arc.h"

class Fsa {
public:
    using arc_store_t = typename std::vector<Arc>;
    using arc_index_t = arc_store_t::size_type;

    const arc_store_t arcs_;
    const arc_index_t root_;

    static const arc_index_t TERMINAL_NODE = 0;

    Fsa(std::vector<Arc>, std::vector<Arc>::size_type);
    ~Fsa();

};

bool accepts(const Fsa &, std::string);
void printRightLanguage(const Fsa &, Fsa::arc_index_t, std::ostream & = std::cout);
void printAcceptedLanguage(const Fsa &, std::ostream & = std::cout);

#endif  // FSA_FSA_H_
