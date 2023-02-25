#include "BT_ACTION_TRAIN_ZERGLINGS.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_TRAIN_ZERGLINGS::BT_ACTION_TRAIN_ZERGLINGS(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_TRAIN_ZERGLINGS::Evaluate(void* data)
{
    return ReturnState(TrainZerglings(data));
}

std::string BT_ACTION_TRAIN_ZERGLINGS::GetDescription()
{
    return "TRAIN ZERGLINGS";
}


BT_NODE::State BT_ACTION_TRAIN_ZERGLINGS::TrainZerglings(void* data)
{
    Data* pData = (Data*)data;


    // let's build a supply provider
    const BWAPI::Unit myDepot = Tools::GetDepot();
    bool startedTraining = myDepot->train(BWAPI::UnitTypes::Zerg_Zergling);

    return startedTraining ? BT_NODE::SUCCESS:BT_NODE::RUNNING;
}