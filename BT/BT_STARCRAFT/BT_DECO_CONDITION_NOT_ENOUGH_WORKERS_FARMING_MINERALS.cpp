#include "BT_DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS::BT_DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS(std::string name, BT_NODE* parent)
    : BT_DECO_CONDITION(name,parent, IsThereNotEnoughWorkersFarmingMinerals) {}


std::string BT_DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS::GetDescription()
{
    return "DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS";
}

bool BT_DECO_CONDITION_NOT_ENOUGH_WORKERS_FARMING_MINERALS::IsThereNotEnoughWorkersFarmingMinerals(void *data)
{
    Data* pData = (Data*)data;
    
    // We want to harvest gas to unlock metabolic boost
    bool hasMetabolicBoost = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Metabolic_Boost) > 0;
    if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Zerg_Extractor) > 0 && !hasMetabolicBoost && BWAPI::Broodwar->self()->gas() < 100) {
        int numGasHarvesters = 0;
        BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
        for (auto& u : myUnits) {
            if (u->getType() == BWAPI::UnitTypes::Zerg_Drone && u->isGatheringGas()) {
                numGasHarvesters++;
            }
        }
        return !(numGasHarvesters > 0) && ((int)pData->unitsFarmingMinerals.size() < pData->nWantedWorkersFarmingMinerals - 1);
    }

    return (int)pData->unitsFarmingMinerals.size() < pData->nWantedWorkersFarmingMinerals;
}
