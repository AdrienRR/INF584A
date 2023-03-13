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
    //std::string message = "Circling";
    //BWAPI::Broodwar->sendText(message.c_str());





    //if (targetPosition == myBasePosition)
    //{
    //    if (Tools::distance(overlordPosition, pData->enemyBase) > 750) {
    //        overlord->move(pData->enemyBase);
    //        std::string message = "Zigzaging towards enemy base: " + std::to_string(targetPosition.x) + ", " + std::to_string(targetPosition.y);
    //        BWAPI::Broodwar->sendText(message.c_str());
    //    }
    //}
    //else
    //{
    //    if (Tools::distance(overlordPosition, pData->enemyBase) < 250) {
    //        overlord->move(myBasePosition);
    //        std::string message = "Zigzaging towards own base: " + std::to_string(targetPosition.x) + ", " + std::to_string(targetPosition.y);
    //        BWAPI::Broodwar->sendText(message.c_str());
    //    }
    //}
    return BT_NODE::SUCCESS;
}