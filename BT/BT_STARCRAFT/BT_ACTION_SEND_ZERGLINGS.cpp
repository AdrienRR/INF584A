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


        BWAPI::Unitset ennemyUnits = BWAPI::Broodwar->enemy()->getUnits();

        if (ennemyUnits.empty()) {

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
        else {
            BWAPI::Unitset myZerglings;
            BWAPI::Unit ZerglingCommander;
            double best_dist_to_base_ennemy = 9999999999.0;
            std::vector<BWAPI::Unitset> zerglingSquads;

            for (BWAPI::Unit u : myUnits)
            {
                if (u->getType() == BWAPI::UnitTypes::Zerg_Zergling)
                {
                    myZerglings.insert(u);
                    double dist_to_base_ennemy = (*u).getPosition().getDistance(pData->enemyBase);
                    if (dist_to_base_ennemy < best_dist_to_base_ennemy) {
                        best_dist_to_base_ennemy = dist_to_base_ennemy;
                        ZerglingCommander = u;
                    }
                }
            }
            auto zerglingIterator = myZerglings.begin();
            while (zerglingIterator != myZerglings.end()) {
                BWAPI::Position pose = (*zerglingIterator)->getPosition();
                BWAPI::Unitset squad;
                squad.insert((*zerglingIterator));
                zerglingIterator++;
                while (zerglingIterator != myZerglings.end() && squad.size()<=6 && (*zerglingIterator)->getPosition().getDistance(pose) < 300) {
                    squad.insert((*zerglingIterator));
                    zerglingIterator++;
                }
                zerglingSquads.push_back(squad);
            }

            int nSquad = 0;
            for (auto squadIt = zerglingSquads.begin(); squadIt != zerglingSquads.end(); squadIt++) {
                auto squad = *squadIt;
                nSquad++;
                std::cout << "Squad " + std::to_string(nSquad) + " has " + std::to_string(squad.size()) + " zerglings" << "\n";

                BWAPI::Unit priorityEnnemyUnit = *ennemyUnits.begin();
                bool priorityIsDangerous = false;
                bool priorityIsWorker = false;
                double priorityDist = 99999999999999.0;
                constexpr double dangerRadius = 3500.0;
                for (auto u = ennemyUnits.begin(); u != ennemyUnits.end(); u++) {
                    //double dist = ZerglingCommander->getPosition().getDistance((*u)->getPosition());
                    double dist = squad.getPosition().getDistance((*u)->getPosition());
                    auto ennemyUnitType = (*u)->getType();
                    if (ennemyUnitType.canAttack() && !ennemyUnitType.isFlyer()) {
                        if (!ennemyUnitType.isWorker()) {
                            if (dist < dangerRadius && (!priorityIsDangerous || dist < priorityDist)) {
                                priorityEnnemyUnit = *u;
                                priorityIsDangerous = true;
                                priorityDist = dist;
                                priorityIsWorker = false;
                            }
                        }
                        else if (!priorityIsDangerous && (!priorityIsWorker || dist < priorityDist)) {
                            priorityEnnemyUnit = *u;
                            priorityDist = dist;
                            priorityIsWorker = true;
                        }
                    }
                    else if (!priorityIsDangerous && !priorityIsWorker && dist < priorityDist && !ennemyUnitType.isFlyer()) {
                        priorityEnnemyUnit = *u;
                        priorityDist = dist;
                    }
                }
                if (priorityEnnemyUnit->isFlying() || priorityEnnemyUnit->getType() == BWAPI::UnitTypes::Unknown) {
                    squad.attack(pData->enemyBase, true);
                    std::cout << "Squad " + std::to_string(nSquad) +" attacking ennemy base at dist " + std::to_string(best_dist_to_base_ennemy) << "\n";
                }
                else {
                    squad.attack(priorityEnnemyUnit, false);
                    std::string message = priorityEnnemyUnit->getType().getName();
                    std::cout << "Squad " + std::to_string(nSquad) +" attacking " + message + "at dist " + std::to_string(priorityDist) << "\n";
                }
            }
        }
    }

    return BT_NODE::SUCCESS;
}