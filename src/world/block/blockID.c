#include "blockID.h"


bool isBlockIDValid(BlockID block) {
    return block > BLOCK_ID_NULL || block < BLOCK_ID_COUNT;
}

bool isBlockIDGas(BlockID block) {
    bool gas = block > BLOCK_ID_CATEGORY_GAS_BEGIN && block < BLOCK_ID_CATEGORY_GAS_END;

    return isBlockIDValid(block) && gas;
}

bool isBlockIDLiquid(BlockID block) {
    bool liquid = block > BLOCK_ID_CATEGORY_LIQUID_BEGIN && block < BLOCK_ID_CATEGORY_LIQUID_END;

    return isBlockIDValid(block) && liquid;
}

bool isBlockIDSolid(BlockID block) {
    bool solid = block > BLOCK_ID_CATEGORY_SOLID_BEGIN && block < BLOCK_ID_CATEGORY_SOLID_END;

    return isBlockIDValid(block) && solid;
}
