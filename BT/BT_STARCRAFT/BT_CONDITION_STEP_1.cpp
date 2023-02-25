#include "BT_CONDITION_STEP_1.h"
#include "Data.h"
#include <BWAPI.h>
#include "Tools.h"

BT_CONDITION_STEP_1::BT_CONDITION_STEP_1(std::string name, BT_NODE* parent)
    : BT_CONDITION(name,parent, IsStep1Done) {}


std::string BT_CONDITION_STEP_1::GetDescription()
{
    return "BT_CONDITION_STEP_1";
}

bool BT_CONDITION_STEP_1::IsStep1Done(void *data)
{
    Data* pData = (Data*)data;

    return pData->step1;
}
