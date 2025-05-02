// ServiceLocator.cpp
#include "ServiceLocator.h"
#include "NullSoundService.h"

std::unique_ptr<dae::ISoundService> dae::ServiceLocator::soundService = std::make_unique<NullSoundService>();
