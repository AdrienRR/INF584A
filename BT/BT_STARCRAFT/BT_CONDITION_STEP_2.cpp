#include "BT_CONDITION_STEP_2.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_CONDITION_STEP_2::BT_CONDITION_STEP_2(std::string name, BT_NODE* parent)
    : BT_CONDITION(name,parent, IsStep2Done) {}


std::string BT_CONDITION_STEP_2::GetDescription()
{
    return "BT_CONDITION_STEP_2";
}

bool BT_CONDITION_STEP_2::IsStep2Done(void *data)
{
    Data* pData = (Data*)data;

    return Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Spawning_Pool, BWAPI::Broodwar->self()->getUnits()) >= 1;

}
