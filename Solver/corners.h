#ifndef CORNERS_H
#define CORNERS_H

#include "corner_orientation.h"
#include "corner_permutation.h"
#include "pruning_info.h"

class Corners {
public:
    Corners();
    void apply_move(int move);
    uint32_t to_index() const;
    void from_index(uint32_t state);
    PruningInfo get_pruning_info() const;
private:
    uint32_t coState;//estado corner orientation
    uint32_t cpState;//estado corner permutation
    TransitionTable transition_table_orientation;
    TransitionTable transition_table_permutation;
    PruningInfo pruning_info;
};

#endif // !CORNERS_H
