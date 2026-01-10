#pragma once
#include "../../utils/bool.h"


typedef enum BlockID {
    BLOCK_ID_NULL,

    BLOCK_ID_CATEGORY_GAS_BEGIN,
    BLOCK_ID_AIR,
    BLOCK_ID_CATEGORY_GAS_END,

    BLOCK_ID_CATEGORY_LIQUID_BEGIN,
    BLOCK_ID_WATER,
    BLOCK_ID_CATEGORY_LIQUID_END,

    BLOCK_ID_CATEGORY_SOLID_BEGIN,
    BLOCK_ID_STONE,
    BLOCK_ID_DIRT,
    BLOCK_ID_GRASS,
    BLOCK_ID_CATEGORY_SOLID_END,

    BLOCK_ID_COUNT_PLUS_ONE,
} BlockID;

#define BLOCK_ID_COUNT BLOCK_ID_COUNT_PLUS_ONE - 1


bool isBlockValid(BlockID block);
bool isBlockGas(BlockID block);
bool isBlockLiquid(BlockID block);
bool isBlockSolid(BlockID block);
