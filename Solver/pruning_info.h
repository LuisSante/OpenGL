#ifndef PRUNING_INFO_H
#define PRUNING_INFO_H

#include <cstdint>
#include <string>

struct PruningInfo
{
    uint64_t state_count;
    int maxDepthBFS;
    int maxDepth;
    std::string pruning_file;
};

#endif // !PRUNING_INFO_H
