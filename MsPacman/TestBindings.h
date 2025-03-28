#pragma once
#include "GameObject.h"
namespace dae
{
    // Declarations only (no implementation)
    void AddControllerInteraction(int playerIndex, GameObject* pOwner);
    void AddKeyboardInteraction(GameObject* pOwner);
}
