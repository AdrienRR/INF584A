#include "BT_CONDITION_STEP_3.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_CONDITION_STEP_3::BT_CONDITION_STEP_3(std::string name, BT_NODE* parent)
    : BT_CONDITION(name,parent, IsStep3Done) {}


std::string BT_CONDITION_STEP_3::GetDescription()
{
    return "BT_CONDITION_STEP_3";
}

bool BT_CONDITION_STEP_3::IsStep3Done(void *data)
{
    Data* pData = (Data*)data;
    int alreadyTrained = Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Broodwar->self()->getUnits());
    int training = 0;
    for (BWAPI::Unit unit : BWAPI::Broodwar->self()->getUnits())
    {
        if ((unit->getType() == BWAPI::UnitTypes::Zerg_Egg && (unit->getTrainingQueue().at(0) == BWAPI::UnitTypes::Zerg_Zergling)))
        {
            training++;
        }
    }

    return alreadyTrained+training >= 3;
}
