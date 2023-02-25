#pragma once
#include <cassert>
#include "BT_CONDITION.h"

//Evaluates the specified conditional task. If the conditional task returns true then return success, else return failure
// no running state 
//--------------------
class BT_CONDITION_STEP_1: public BT_CONDITION {
public:
    BT_CONDITION_STEP_1(std::string name, BT_NODE* parent);
    std::string GetDescription() override;

    static bool IsStep1Done(void *data);

};
//----------