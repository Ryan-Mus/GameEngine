#include "ServiceLocator.h"

namespace dae
{
    std::unique_ptr<SoundManager> ServiceLocator::soundManager = nullptr;
}
