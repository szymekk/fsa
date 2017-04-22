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
    //TODO: construct from FsaBuilder
    //TODO: define methods for traversing the automaton and checking whether the automaton accepts/rejects given words
    bool accepts(std::string);
    void printAllWords(std::ostream& = std::cout);
private:
    std::vector<Arc> arcs_;
    decltype(arcs_)::size_type root_;

};
#endif  // FSA_FSA_H_
