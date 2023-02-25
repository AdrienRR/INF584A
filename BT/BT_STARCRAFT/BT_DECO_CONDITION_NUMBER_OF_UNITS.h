#pragma once
#include <cassert>
#include "BT_DECO_CONDITION.h"

//Evaluates the specified conditional task. If the conditional task returns success then the child task is run and the child status is returned. If the conditional task does not
// return success then the child task is not run and a failure status is immediately returned.
// 
//--------------------
class BT_DECO_CONDITION_NUMBER_OF_UNITS : public BT_DECO_CONDITION {
private:
    int number_of_units;
public:
    BT_DECO_CONDITION_NUMBER_OF_UNITS(std::string name, BT_NODE* parent, int number_of_units);
    std::string GetDescription() override;

    static bool IsThereRightNumberOfUnits(void* data); 

};
//----------