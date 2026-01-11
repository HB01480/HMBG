#include "block.h"

#include "../../common.h"


Block block_init(BlockID id) {
    Block block; SDL_zero(block);
    block.id = id;

    return block;
}

void block_free(Block *block) {
    // Nothing for now
}
