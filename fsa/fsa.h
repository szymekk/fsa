#ifndef FSA_FSA_H_
#define FSA_FSA_H_

#include <vector>
#include <iostream>
#include <stdexcept>

#include "arc.h"

class Fsa {
public:
    Fsa(std::vector<Arc>, std::vector<Arc>::size_type);
    ~Fsa();
    bool accepts(std::string);
    void printAllWords(std::ostream& = std::cout);
    static const size_t TERMINAL_NODE = 0;
private:
    std::vector<Arc> arcs_;
    decltype(arcs_)::size_type root_;

};
#endif  // FSA_FSA_H_
