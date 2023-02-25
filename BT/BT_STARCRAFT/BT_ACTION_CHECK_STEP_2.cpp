#include "BT_ACTION_CHECK_STEP_2.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_CHECK_STEP_2::BT_ACTION_CHECK_STEP_2(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_CHECK_STEP_2::Evaluate(void* data)
{
    return ReturnState(CheckStep2(data));
}

std::string BT_ACTION_CHECK_STEP_2::GetDescription()
{
    return "CHECK IF STEP 2 IS DONE";
}


BT_NODE::State BT_ACTION_CHECK_STEP_2::CheckStep2(void* data)
{
    Data* pData = (Data*)data;
    if (Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Spawning_Pool, BWAPI::Broodwar->self()->getUnits()) >= 1) {
        return BT_NODE::SUCCESS;
    }
    return BT_NODE::FAILURE;
}