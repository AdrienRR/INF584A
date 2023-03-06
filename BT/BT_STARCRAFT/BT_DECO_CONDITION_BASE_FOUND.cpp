#include "BT_DECO_CONDITION_BASE_FOUND.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_DECO_CONDITION_BASE_FOUND::BT_DECO_CONDITION_BASE_FOUND(std::string name, BT_NODE* parent)
    : BT_DECO_CONDITION(name, parent, FoundEnemyBase) {}


std::string BT_DECO_CONDITION_BASE_FOUND::GetDescription()
{
    return "DECO_CONDITION_BASE_FOUND";
}

bool BT_DECO_CONDITION_BASE_FOUND::FoundEnemyBase(void* data)
{
    Data* pData = (Data*)data;
    const char* message;
    if (pData->enemyBaseFound) {
        BWAPI::Broodwar->sendText("Base found");
    }
    else {
        BWAPI::Broodwar->sendText("Base NOT found");
    }
    return pData->enemyBaseFound;

}
