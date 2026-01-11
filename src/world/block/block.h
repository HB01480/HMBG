#pragma once
#include "blockID.h"


typedef struct Block {
    BlockID id;
    
} Block;


Block block_init(BlockID id);
void block_free(Block *block);
