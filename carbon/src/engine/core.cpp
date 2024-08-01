#include "core.h"

void createGlobalEngineInstance(c_Carbon* p_CCarbon) {
	c_Carbon engineInstance = c_Carbon();
	c_World  worldInstance = c_World();
	engineInstance.engineInstanceWorld = worldInstance;

	*p_CCarbon = engineInstance;
}