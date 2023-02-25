#include "BT_SELECTOR.h"

BT_SELECTOR::BT_SELECTOR(std::string name, BT_NODE* parent, size_t childrenMaxCount) : BT_NODE(name,parent, childrenMaxCount) {};
BT_SELECTOR::BT_SELECTOR(std::string name, size_t childrenMaxCount) : BT_NODE(name,childrenMaxCount) {};

BT_NODE::State BT_SELECTOR::Evaluate(void* data) {
    if (!HasBeenEvaluatedAtLeastOnce) Log("1st Evaluate");

    BT_NODE::Evaluate(data);

    for (size_t childIdx = 0; childIdx < ChildrenCount; ++childIdx) {
        BT_NODE::State  childState = Children[childIdx]->Evaluate(data);
        if (childState == BT_NODE::State::SUCCESS) {
            return BT_NODE::State::SUCCESS;
        }
        if (childState == BT_NODE::State::RUNNING) {
            return BT_NODE::State::RUNNING;
        }
    }
    return BT_NODE::State::FAILURE;

}

void BT_SELECTOR::Reset()
{
    BT_NODE::Reset();
}

std::string BT_SELECTOR::GetDescription()
{
    std::string str = "SELECTOR (";
    for (size_t i = 0; i < ChildrenCount; i++)
    {
        str += " " + Children[i]->Name;
        if (i < ChildrenCount - 1)str += ",";
    }
    str += ")";
    return str;
}