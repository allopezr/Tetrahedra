#include "stdafx.h"
#include "SceneContent.h"

#include "BoundaryVolumeHierarchy.h"
#include "ChronoUtilities.h"
#include "DrawAABB.h"
#include "DrawTetrahedron.h"
#include "InclDraw2D.h"
#include "InclGeom2D.h"
#include "PointCloud.h"
#include "RandomUtilities.h"


// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void Tet::SceneContent::buildScenario()
{
	// Triangle mesh
	auto model = (new DrawMesh())->loadModelOBJ("Assets/Models/3DBenchy.stl");
	model->moveGeometryToOrigin(glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 2.0f, glm::vec3(1.0, .0, .0)), FLT_MAX);
	this->addNewModel(model);

	auto newModel = model->copy();
	newModel->setModelMatrix(glm::translate(glm::mat4(1.0f), model->getAABB().min() / 2.01f) * newModel->getModelMatrix());
    this->addNewModel(newModel);

	BoundaryVolumeHierarchy bvh;
	bvh.build(model->vertices(), model->indices(VAO::IBO_TRIANGLE));
}


// ------------------------------------------------------------------------------------------


Tet::SceneContent::SceneContent()
= default;

Tet::SceneContent::~SceneContent()
{
	_camera.clear();
	_model.clear();
}

void Tet::SceneContent::addNewCamera(const ApplicationState* appState)
{
	_camera.push_back(std::make_unique<Camera>(appState->_viewportSize.x, appState->_viewportSize.y, false));
}

void Tet::SceneContent::addNewModel(Model3D* model)
{	
	_sceneAABB.update(model->getAABB());
	_model.push_back(std::unique_ptr<Model3D>(model));
}

Tet::Model3D* Tet::SceneContent::getModel(const Model3D::Component* component) const
{
	for (auto& model : _model)
	{
		if (model->belongsModel(component))
			return model.get();
	}

	return nullptr;
}
