#include "BT_CONDITION_STEP_2B.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_CONDITION_STEP_2B::BT_CONDITION_STEP_2B(std::string name, BT_NODE* parent)
    : BT_CONDITION(name,parent, IsStep2BDone) {}


std::string BT_CONDITION_STEP_2B::GetDescription()
{
    return "BT_CONDITION_STEP_2B";
}

bool BT_CONDITION_STEP_2B::IsStep2BDone(void *data)
{
    Data* pData = (Data*)data;
    int alreadyTrained = Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Broodwar->self()->getUnits());
    int training = 0;
    for (BWAPI::Unit unit : BWAPI::Broodwar->self()->getUnits())
    {
        if ((unit->getType() == BWAPI::UnitTypes::Zerg_Egg && (unit->getTrainingQueue().at(0) == BWAPI::UnitTypes::Zerg_Drone)))
        {
            training++;
        }
    }

    return alreadyTrained+training >= 6;
}
