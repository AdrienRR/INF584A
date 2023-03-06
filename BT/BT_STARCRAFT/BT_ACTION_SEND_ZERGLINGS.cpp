#include "BT_ACTION_SEND_ZERGLINGS.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_SEND_ZERGLINGS::BT_ACTION_SEND_ZERGLINGS(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_SEND_ZERGLINGS::Evaluate(void* data)
{
    return ReturnState(SendZerglings(data));
}

std::string BT_ACTION_SEND_ZERGLINGS::GetDescription()
{
    return "SEND ZERGLINGS TO ENNEMY BASE";
}


BT_NODE::State BT_ACTION_SEND_ZERGLINGS::SendZerglings(void* data)
{
    Data* pData = (Data*)data;

    BWAPI::Position targetPosition;

    const BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();


    // Check if there is enough unit to attack
    if(Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Zergling, myUnits) > 0)
    {

        // If the overlord has already found the ennemy base, the zerglings can directly attack it
        if (pData->enemyBaseFound) {
            targetPosition = pData->enemyBase;
            for (BWAPI::Unit u : myUnits)
            {
                if (u->getType() == BWAPI::UnitTypes::Zerg_Zergling)
                {
                    u->attack(targetPosition, true);
                }
            }
        }

        // If not, they will scout the map in order to find tha ennemy base, following the same rules than the overlord
        else
        {
            BWAPI::TilePosition::list startLocations = BWAPI::Broodwar->getStartLocations();

            for (BWAPI::TilePosition startLocation : startLocations)
            {
                if (startLocation != BWAPI::Broodwar->self()->getStartLocation())
                {
                    pData->enemyBaseFound = true;
                    pData->enemyBase = BWAPI::Position(startLocation.makeValid());

                    for (BWAPI::Unit u : myUnits)
                    {
                        if (u->getType() == BWAPI::UnitTypes::Zerg_Zergling)
                        {
                            u->attack(pData->enemyBase, true);
                        }
                    }
                }
            }
        }
    }

    return BT_NODE::SUCCESS;
}