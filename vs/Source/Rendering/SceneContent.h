#pragma once

#include "stdafx.h"
#include "AABB.h"
#include "ApplicationState.h"
#include "Camera.h"
#include "Model3D.h"

namespace Tet
{
	class SceneContent
	{
	public:
		std::vector<std::unique_ptr<Camera>>	_camera;
		std::vector<std::unique_ptr<Model3D>>	_model;
		AABB									_sceneAABB;

		SceneContent();
		virtual ~SceneContent();

		void addNewCamera(const ApplicationState* appState);
		void addNewModel(Model3D* model);
		void buildScenario();
		Model3D* getModel(const Model3D::Component* component) const;
	};
}