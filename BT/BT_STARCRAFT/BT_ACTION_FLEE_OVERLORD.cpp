#include "BT_ACTION_FLEE_OVERLORD.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_FLEE_OVERLORD::BT_ACTION_FLEE_OVERLORD(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_FLEE_OVERLORD::Evaluate(void* data)
{
    return ReturnState(MoveOverlord(data));
}

std::string BT_ACTION_FLEE_OVERLORD::GetDescription()
{
    return "FLEE OVERLORD";
}


BT_NODE::State BT_ACTION_FLEE_OVERLORD::MoveOverlord(void* data)
{
    Data* pData = (Data*)data;

    BWAPI::Unit overlord = pData->myOverlord;

    BWAPI::Position targetPosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation().makeValid());

    // Issue the command to move the unit
    overlord->move(targetPosition);
    return BT_NODE::SUCCESS;
}