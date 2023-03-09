#include "BT_ACTION_BUILD_POOL.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_BUILD_POOL::BT_ACTION_BUILD_POOL(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_BUILD_POOL::Evaluate(void* data)
{
    return ReturnState(BuildPool(data));
}

std::string BT_ACTION_BUILD_POOL::GetDescription()
{
    return "Build Pool";
}


BT_NODE::State BT_ACTION_BUILD_POOL::BuildPool(void* data)
{
    Data* pData = (Data*)data;

    if (Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Spawning_Pool, BWAPI::Broodwar->self()->getUnits()) >= 1) {
        return BT_NODE::SUCCESS;
    }

    // get the unit to build spawning pool
    BWAPI::UnitType builderType = BWAPI::UnitTypes::Zerg_Spawning_Pool.whatBuilds().first;

    BWAPI::Unit builder = Tools::GetUnitOfType(builderType);

    bool startedBuilding = false;
    if (builder)
    {
        //Location to build
        BWAPI::TilePosition startPos = BWAPI::Broodwar->self()->getStartLocation();
        int maxBuildRange = 64;
        bool buildingOnCreep = BWAPI::UnitTypes::Zerg_Spawning_Pool.requiresCreep();
        BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Zerg_Spawning_Pool, startPos, maxBuildRange, buildingOnCreep);
        startedBuilding = builder->build(BWAPI::UnitTypes::Zerg_Spawning_Pool, buildPos);
    }


    return BT_NODE::RUNNING;
}