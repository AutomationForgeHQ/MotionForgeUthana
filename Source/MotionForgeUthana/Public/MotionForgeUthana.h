// The module that carries Uthana into MotionForge.

#pragma once

#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

class FUthanaProvider;

MOTIONFORGEUTHANA_API DECLARE_LOG_CATEGORY_EXTERN(LogMotionForgeUthana, Log, All);

/**
 * Registers the Uthana provider with MotionForge and nothing else.
 *
 * This plugin used to be a folder inside the core, which made MotionForge's settings default to a
 * vendor by name and its module register one on startup. Extracted, the core registers nothing,
 * defaults to nothing, and a project that wants Uthana enables this plugin - the same shape as
 * MotionForgeKimodo, FaceForgeACE and MeshForgeCloud, and the reason all of them can be swapped
 * without touching a core.
 */
class FMotionForgeUthanaModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TSharedPtr<FUthanaProvider> Provider;
};
