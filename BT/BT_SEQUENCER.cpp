#include "BT_SEQUENCER.h"
#include <format>
#include <iostream>

BT_SEQUENCER:: BT_SEQUENCER(std::string name, BT_NODE* parent, size_t childrenMaxCount) : BT_NODE(name,parent, childrenMaxCount) {};

BT_SEQUENCER::BT_SEQUENCER(std::string name, size_t childrenMaxCount) : BT_NODE(name,childrenMaxCount) {};

BT_NODE::State BT_SEQUENCER::Evaluate(void* data) {
    if (!HasBeenEvaluatedAtLeastOnce) Log("1st Evaluate");

    BT_NODE::Evaluate(data);

    for (size_t childIdx = 0; childIdx < ChildrenCount; ++childIdx) {
        BT_NODE::State  childState = Children[childIdx]->Evaluate(data);
        if (childState == BT_NODE::State::FAILURE) {
            return BT_NODE::State::FAILURE;
        }
        if (childState == BT_NODE::State::RUNNING) {
            return BT_NODE::State::RUNNING;
        }
    }
    return BT_NODE::State::SUCCESS;
}

void BT_SEQUENCER::Reset()
{
    BT_NODE::Reset();
}

std::string BT_SEQUENCER::GetDescription()
{
    std::string str = "SEQUENCER (";
    for (size_t i = 0; i < ChildrenCount; i++)
    {
        str += " " + Children[i]->Name;
        if (i < ChildrenCount - 1)str += ",";
    }
    str += ")";
    return str;
}