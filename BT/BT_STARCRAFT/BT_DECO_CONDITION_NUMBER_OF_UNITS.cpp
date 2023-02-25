#include "BT_DECO_CONDITION_NUMBER_OF_UNITS.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_DECO_CONDITION_NUMBER_OF_UNITS::BT_DECO_CONDITION_NUMBER_OF_UNITS(std::string name, BT_NODE* parent, int number_of_units)
    : BT_DECO_CONDITION(name, parent, IsThereRightNumberOfUnits) {
    this->number_of_units = number_of_units;

}


std::string BT_DECO_CONDITION_NUMBER_OF_UNITS::GetDescription()
{
    return "BT_DECO_CONDITION_NUMBER_OF_UNITS "+ std::to_string(this->number_of_units);
}

bool BT_DECO_CONDITION_NUMBER_OF_UNITS::IsThereRightNumberOfUnits(void* data)
{
    Data* pData = (Data*)data;

    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    const int supplyUsed = BWAPI::Broodwar->self()->supplyUsed();

    log(supplyUsed);
            
    return supplyUsed <= 4;
}
