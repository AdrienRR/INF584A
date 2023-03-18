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


class Squad : public BWAPI::Unitset 
{
public:
    
    Squad() : BWAPI::Unitset(){};

    void maintainCoherence() {
        for (auto &z : *this) {
            if (this->getPosition().getDistance(z->getPosition()) > 100)
                z->move(this->getPosition());
        }
    }

    void surroundAndAttack(BWAPI::Unit enemy) {
        using namespace BWAPI;

        // compute the enemy position
        Position enemyPos = enemy->getPosition();


        // separate the zerglings into two groups: those that can attack the enemy, and those that cannot
        std::vector<Unit> attackUnits, moveUnits;
        for (auto& unit : *this) {
            if (unit->getDistance(enemy) <= unit->getType().groundWeapon().maxRange()) {
                attackUnits.push_back(unit);
            }
            else {
                moveUnits.push_back(unit);
            }
        }

        // move the zerglings that cannot attack the enemy to their target positions

        int i = 0;
        for (auto& unit : moveUnits) {
            int constexpr dist = 32;
            Position const targetPos = enemyPos + Position(dist * cos(2*3.14 * float(i)/float(size())), dist * sin(2 * 3.14 * float(i) / float(size())));
            unit->move(targetPos);
            i++;
        }

        // attack the enemy with the zerglings that can attack
        for (auto& unit : attackUnits) {
            unit->attack(enemy);
        }
    }

};

BT_NODE::State BT_ACTION_SEND_ZERGLINGS::SendZerglings(void* data)
{
    Data* pData = (Data*)data;

    BWAPI::Position targetPosition;

    const BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();


    // If there is not enough units, do nothing
    if (Tools::CountUnitsOfType(BWAPI::UnitTypes::Zerg_Zergling, myUnits) <= 1)
        return BT_NODE::SUCCESS;

    // ---
    // construction of squads :
    // ---
    BWAPI::Unitset myZerglings;
    std::vector<Squad> zerglingSquads;

    // get list of zerglings
    for (BWAPI::Unit u : myUnits)
    {
        if (u->getType() == BWAPI::UnitTypes::Zerg_Zergling)
        {
            myZerglings.insert(u);
        }
    }
    const int size = myZerglings.size();

    // vector storing if a zergling is in a squad
    std::vector<bool> inSquad;
    inSquad.resize(size);
    for (auto i = 0; i < size; i++) {
        inSquad.at(i) = false;
    }

    // assign a zergling to a squad iif the squad is small and the zerglings are close to each other
    int counter = 0;
    for (auto zerglingIterator = myZerglings.begin(); zerglingIterator != myZerglings.end(); zerglingIterator++) {
        if (!inSquad.at(counter)) {
            BWAPI::Position pose = (*zerglingIterator)->getPosition();
            Squad squad;
            squad.insert((*zerglingIterator));
            inSquad.at(counter) = true;
            int counter2 = counter;
            for (auto j = zerglingIterator; j != myZerglings.end(); j++) {
                if (!inSquad.at(counter2) && squad.size() < 6 && (*j)->getPosition().getDistance(pose) < 300) {
                    squad.insert((*j));
                    inSquad.at(counter2) = true;
                }
                counter2++;
            }
            zerglingSquads.push_back(squad);
        }
        counter++;
    }

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

        return BT_NODE::SUCCESS;
    }

    // sending squads to battle
    int nSquad = 0;
    for (auto squadIt = zerglingSquads.begin(); squadIt != zerglingSquads.end(); squadIt++) {
        auto squad = *squadIt;
        nSquad++;
        std::cout << "Squad " + std::to_string(nSquad) + " has " + std::to_string(squad.size()) + " zerglings" << "\n";

        // choose an enemy
        BWAPI::Unit priorityEnnemyUnit = *ennemyUnits.begin();
        int priorityLife = 9999999;
        bool priorityIsDangerous = false;
        bool priorityIsWorker = false;
        double priorityDist = 99999999999999.0;
        constexpr double dangerRadius = 3500.0;

        constexpr double PrioLowLifeRadius = 100.0;

        for (auto u = ennemyUnits.begin(); u != ennemyUnits.end(); u++) {
            double dist = squad.getPosition().getDistance((*u)->getPosition());
            int totalLife = (*u)->getHitPoints() + (*u)->getShields();
            auto ennemyUnitType = (*u)->getType();
            if (ennemyUnitType.canAttack() && !ennemyUnitType.isFlyer()) {
                if (!ennemyUnitType.isWorker()) {
                    if (dist < dangerRadius && (!priorityIsDangerous || dist < priorityDist)) {
                        priorityEnnemyUnit = *u;
                        priorityIsDangerous = true;
                        priorityDist = dist;
                        priorityLife = totalLife;
                        priorityIsWorker = false;
                    }
                    if (dist < PrioLowLifeRadius && totalLife < priorityLife) {
                        priorityEnnemyUnit = *u;
                        priorityIsDangerous = true;
                        priorityDist = dist;
                        priorityLife = totalLife;
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

        // attack it
        if (!priorityIsDangerous) {
            squad.maintainCoherence();
        }
        if (priorityEnnemyUnit->isFlying() || priorityEnnemyUnit->getType() == BWAPI::UnitTypes::Unknown) {
            squad.attack(pData->enemyBase, true);
        }
        else {
            squad.surroundAndAttack(priorityEnnemyUnit);
                        
            //squad.attack(priorityEnnemyUnit, false);
            std::string message = priorityEnnemyUnit->getType().getName();
            std::cout << "Squad " + std::to_string(nSquad) +" attacking " + message + "at dist " + std::to_string(priorityDist) << "\n";
        }
    }

    return BT_NODE::SUCCESS;
}