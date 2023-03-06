#include "BT_DECO_CONDITION_HP_OVERLORD.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_DECO_CONDITION_HP_OVERLORD::BT_DECO_CONDITION_HP_OVERLORD(std::string name, BT_NODE* parent)
    : BT_DECO_CONDITION(name, parent, HasEnoughHealth) {}


std::string BT_DECO_CONDITION_HP_OVERLORD::GetDescription()
{
    return "DECO_CONDITION_HP_OVERLORD";
}

bool BT_DECO_CONDITION_HP_OVERLORD::HasEnoughHealth(void* data)
{
    Data* pData = (Data*)data;
    float healthThresholdPercentage = 0.99;


    // If we have a sufficient amount of supply, we don't need to do anything
    return pData->myOverlord->getHitPoints() > healthThresholdPercentage * BWAPI::UnitTypes::Zerg_Overlord.maxHitPoints();

}
