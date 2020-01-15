#pragma once

/*
*	RTR.h is used by client applications for accessing engine specific code
*/

//-----------------------------------
// Engine Core
//-----------------------------------

#include "RTREngine/Core/App.h"
#include "RTREngine/Core/Log.h"
#include "RTREngine/Core/Time.h"
#include "RTREngine/Input/Input.h"
#include "RTREngine/Input/KeyCodes.h"
#include "RTREngine/Input/MouseCodes.h"
#include "RTREngine/Layers/Layer.h"
#include "RTREngine/Layers/LayerStack.h"
#include "RTREngine/ImGui/ImGuiLayer.h"

//-----------------------------------
// Renderer
//-----------------------------------

#include "RTREngine/Renderer/Renderer.h"
#include "RTREngine/Renderer/Buffer.h"
#include "RTREngine/Renderer/Shader.h"
#include "RTREngine/Renderer/VertexArray.h"