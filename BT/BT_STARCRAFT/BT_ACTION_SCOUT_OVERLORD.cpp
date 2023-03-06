#include "BT_ACTION_SCOUT_OVERLORD.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_SCOUT_OVERLORD::BT_ACTION_SCOUT_OVERLORD(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_SCOUT_OVERLORD::Evaluate(void* data)
{
    return ReturnState(MoveOverlord(data));
}

std::string BT_ACTION_SCOUT_OVERLORD::GetDescription()
{
    return "SCOUT OVERLORD";
}


BT_NODE::State BT_ACTION_SCOUT_OVERLORD::MoveOverlord(void* data)
{
    Data* pData = (Data*)data;

    BWAPI::Unit overlord = pData->myOverlord;

    // Get the position of the unit
    BWAPI::Position unitPosition = overlord->getPosition();
    BWAPI::Position targetPosition = unitPosition;

    // Choose a target position to move to
    //BWAPI::Position targetPosition = BWAPI::Position(unitPosition.x + 10, unitPosition.y - 10);

    // Get all of the start locations on the map
    BWAPI::TilePosition::list startLocations = BWAPI::Broodwar->getStartLocations();

    // Iterate through all of the start locations
    for (BWAPI::TilePosition startLocation : startLocations)
    {
        // Check if the start location is not your own starting position
        if (!(std::find(pData->visistedStartingPositions.begin(), pData->visistedStartingPositions.end(), startLocation) != pData->visistedStartingPositions.end()))
        {
            targetPosition = BWAPI::Position(startLocation.makeValid());

            if (Tools::distance(targetPosition, unitPosition) < 250) {
                pData->visistedStartingPositions.push_back(startLocation);


                //Check if the enemy base is found by retrieving the units around the base
                BWAPI::Unitset unitsInRadius = BWAPI::Broodwar->getUnitsInRadius(unitPosition, 300.0);
                // Convert the size of the Unitset to a string
                std::string message = "Found " + std::to_string(unitsInRadius.size()) + " units in radius";
                // Send the message to the chat
                BWAPI::Broodwar->sendText(message.c_str());
                // Iterate over all units in the search radius
                for (auto& unit : unitsInRadius) {
                    // Check if the unit is an enemy unit
                    if (unit->getType().isBuilding()) {
                        // Convert the size of the Unitset to a string
                        std::string message = "Found Enemy Base !";
                        BWAPI::Broodwar->sendText(message.c_str());

                        pData->enemyBaseFound = true;
                        pData->enemyBase = targetPosition;
                    }
                }


            }
        }
    }



    // Issue the command to move the unit
    overlord->move(targetPosition);
    return BT_NODE::SUCCESS;



    //const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    //for (auto& unit : myUnits)
    //{
    //    // Check the unit type, if it is an idle worker, then we want to send it somewhere
    //    if (unit->getType().isWorker() && unit->isIdle())
    //    {
    //        // Get the closest mineral to this worker unit
    //        BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

    //        // If a valid mineral was found, right click it with the unit in order to start harvesting
    //        if (closestMineral) {
    //            unit->rightClick(closestMineral);
    //            pData->unitsFarmingMinerals.insert(unit);
    //            return BT_NODE::SUCCESS;
    //        }
    //    }
    //}

    //return BT_NODE::FAILURE;
}