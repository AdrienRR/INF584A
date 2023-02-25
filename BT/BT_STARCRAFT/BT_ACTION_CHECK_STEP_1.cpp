#include "BT_ACTION_CHECK_STEP_1.h"
#include "Tools.h"
#include "Data.h"

BT_ACTION_CHECK_STEP_1::BT_ACTION_CHECK_STEP_1(std::string name,BT_NODE* parent)
    :  BT_ACTION(name,parent) {}

BT_NODE::State BT_ACTION_CHECK_STEP_1::Evaluate(void* data)
{
    return ReturnState(CheckStep1(data));
}

std::string BT_ACTION_CHECK_STEP_1::GetDescription()
{
    return "CHECK IF STEP 1 IS DONE";
}


BT_NODE::State BT_ACTION_CHECK_STEP_1::CheckStep1(void* data)
{
    Data* pData = (Data*)data;
    if (pData->step1) {
        return BT_NODE::SUCCESS;
    }
    return BT_NODE::FAILURE;
}