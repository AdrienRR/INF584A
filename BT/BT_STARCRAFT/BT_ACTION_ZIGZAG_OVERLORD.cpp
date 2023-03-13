#include "BT_ACTION_ZIGZAG_OVERLORD.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_ZIGZAG_OVERLORD::BT_ACTION_ZIGZAG_OVERLORD(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_ZIGZAG_OVERLORD::Evaluate(void* data)
{
    return ReturnState(MoveOverlord(data));
}

std::string BT_ACTION_ZIGZAG_OVERLORD::GetDescription()
{
    return "FLEE OVERLORD";
}


BT_NODE::State BT_ACTION_ZIGZAG_OVERLORD::MoveOverlord(void* data)
{
    Data* pData = (Data*)data;

    BWAPI::Unit overlord = pData->myOverlord;

    BWAPI::Position overlordPosition = overlord->getPosition();

    BWAPI::Position base2overlord = overlordPosition - pData->enemyBase;

    BWAPI::Position perpendicular = BWAPI::Position(-base2overlord.y, base2overlord.x);

    BWAPI::Position targetPosition = overlordPosition + perpendicular;

    if (Tools::distance(overlordPosition, pData->enemyBase) > 200) {
        targetPosition = pData->enemyBase;
    }
    else if (Tools::distance(overlordPosition, pData->enemyBase) < 100) {
        targetPosition += base2overlord;
    }

    overlord->move(targetPosition);
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 30), "Main overlord is circling\n");
    return BT_NODE::SUCCESS;
}