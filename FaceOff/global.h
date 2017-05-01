#ifndef GLOBAL_H_
#define GLOBAL_H_

// forward declaration
class LightManager;
class ModelManager;
class RendererManager;

struct Global
{
	LightManager* lightMgr = nullptr;
	ModelManager* modelMgr = nullptr;
	RendererManager* rendererMgr = nullptr;
};


// static LightManager	lightManager;
extern Global global;
#endif