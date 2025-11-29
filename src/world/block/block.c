#include "block.h"


bool isBlockValid(BlockID block) {
    return block > BLOCK_ID_NULL || block < BLOCK_ID_COUNT;
}

bool isBlockGas(BlockID block) {
    bool gas = block > BLOCK_ID_CATEGORY_GAS_BEGIN && block < BLOCK_ID_CATEGORY_GAS_END;

    return isBlockValid(block) && gas;
}

bool isBlockLiquid(BlockID block) {
    bool liquid = block > BLOCK_ID_CATEGORY_LIQUID_BEGIN && block < BLOCK_ID_CATEGORY_LIQUID_END;

    return isBlockValid(block) && liquid;
}

bool isBlockSolid(BlockID block) {
    bool solid = block > BLOCK_ID_CATEGORY_SOLID_BEGIN && block < BLOCK_ID_CATEGORY_SOLID_END;

    return isBlockValid(block) && solid;
}
