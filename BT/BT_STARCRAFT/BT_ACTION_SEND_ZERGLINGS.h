#pragma onceBT_ACTION_SEND_ZERGLINGS
#include <cassert>
#include "BT_ACTION.h"
#include <vector>
#include <BWAPI.h>

//--------------------
class BT_ACTION_SEND_ZERGLINGS : public BT_ACTION {
public:
    BT_ACTION_SEND_ZERGLINGS(std::string name, BT_NODE* parent);

private:
    State Evaluate(void* data) override;
    std::string GetDescription() override;
    static BT_NODE::State SendZerglings(void* data);
};
//----------