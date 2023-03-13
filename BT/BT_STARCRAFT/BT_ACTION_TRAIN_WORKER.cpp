#include "BT_ACTION_TRAIN_WORKER.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_TRAIN_WORKER::BT_ACTION_TRAIN_WORKER(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_TRAIN_WORKER::Evaluate(void* data)
{
    return ReturnState(TrainWorker(data));
}

std::string BT_ACTION_TRAIN_WORKER::GetDescription()
{
    return "TRAIN WORKER";
}


BT_NODE::State BT_ACTION_TRAIN_WORKER::TrainWorker(void* data)
{
    Data* pData = (Data*)data;

    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    const BWAPI::Unit myDepot = Tools::GetDepot();
    bool startedBuilding = false;

    for (BWAPI::Unit unit : BWAPI::Broodwar->self()->getUnits())
    {
        if ((unit->getType() == BWAPI::UnitTypes::Zerg_Egg && (unit->getTrainingQueue().at(0) == BWAPI::UnitTypes::Zerg_Drone)))
        {
            return BT_NODE::SUCCESS;
        }
    }
    
    if (myDepot != NULL) {
        startedBuilding = myDepot->train(workerType);
    }

    if (startedBuilding) {
        pData->step1 = true;
        return BT_NODE::SUCCESS;
    }
    return BT_NODE::RUNNING;
}