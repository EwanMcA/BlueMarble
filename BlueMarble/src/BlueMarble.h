#pragma once

// For use by BlueMarble Applications

#include "BlueMarble/application.h"
#include "BlueMarble/layer.h"
#include "BlueMarble/Log.h"

#include "BlueMarble/ecs.h"
#include "BlueMarble/component.h"

#include "BlueMarble/Core/timeStep.h"

#include "BlueMarble/input.h"
#include "BlueMarble/keyCodes.h"
#include "BlueMarble/mouseCodes.h"

#include "BlueMarble/imgui/imguilayer.h"

// Renderer ------------------------------
#include "BlueMarble/Renderer/renderer.h"
#include "BlueMarble/Renderer/renderCommand.h"

#include "BlueMarble/Renderer/buffer.h"
#include "BlueMarble/Renderer/shader.h"
#include "BlueMarble/Renderer/texture.h"
#include "BlueMarble/Renderer/vertexArray.h"

#include "BlueMarble/Renderer/camera.h"

// Objects -------------------------------
#include "BlueMarble/terrain.h"

// Entry Point ---------------------------
// TODO: Have to include this manually at the moment
//#include "BlueMarble/entryPoint.h"
