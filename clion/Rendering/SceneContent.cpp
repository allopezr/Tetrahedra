#include "stdafx.h"
#include "SceneContent.h"

#include "ChronoUtilities.h"
#include "DrawMesh.h"
#include "DrawPointCloud.h"
#include "DrawSegment.h"
#include "DrawTriangle.h"
#include "PointCloud.h"
#include "RandomUtilities.h"
#include "SegmentLine.h"
#include "Triangle.h"


// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void AlgGeom::SceneContent::buildScenario()
{
    vec2 minBoundaries = vec2(-1.5, -.5), maxBoundaries = vec2(-minBoundaries);

    // Triangle mesh
    auto model = (new DrawMesh())->loadModelOBJ("Assets/Models/Ajax.obj");
    model->moveGeometryToOrigin(model->getModelMatrix(), 10.0f)->setModelMatrix(glm::translate(model->getModelMatrix(), vec3(.0f, .0f, .5f)));
    this->addNewModel(model);


    // Spheric randomized point cloud
    int numPoints = 800, numPointClouds = 6;

    for (int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        PointCloud* pointCloud = new PointCloud;
        vec3 sphereCenter = vec3(
            RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
            RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
            RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x));

        for (int idx = 0; idx < numPoints; ++idx)
        {
            vec3 rand = RandomUtilities::getUniformRandomInUnitSphere() / 6.0f + sphereCenter;
            pointCloud->addPoint(Point(rand.x, rand.y));
        }

        this->addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName());
        delete pointCloud;
    }

    // Random segments
    int numSegments = 8;

    for (int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
    {
        Point a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Point b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        SegmentLine* segment = new SegmentLine(a, b);

        this->addNewModel((new DrawSegment(*segment))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName());
        delete segment;
    }

    // Random triangles
    int numTriangles = 30;
    float alpha = 2 * glm::pi<float>() / static_cast<float>(numTriangles);

    for (int triangleIdx = 0; triangleIdx < numTriangles; ++triangleIdx)
    {
        float rand_b = RandomUtilities::getUniformRandom(.5f, .9f), rand_c = RandomUtilities::getUniformRandom(.6f, .8f);
        Vect2d a(.0f, .0f);
        Vect2d b(glm::cos(alpha * triangleIdx) * rand_b, glm::sin(alpha * triangleIdx) * rand_b);
        Vect2d c(glm::cos(alpha * (triangleIdx + 1)) * rand_c, glm::sin(alpha * (triangleIdx + 1)) * rand_c);
        Triangle* triangle = new Triangle(a, b, c);

        this->addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))
            ->overrideModelName());
        delete triangle;
    }
}


// ------------------------------------------------------------------------------------------


AlgGeom::SceneContent::SceneContent()
{
}

AlgGeom::SceneContent::~SceneContent()
{
	_camera.clear();
	_model.clear();
}

void AlgGeom::SceneContent::addNewCamera(ApplicationState* appState)
{
	_camera.push_back(std::unique_ptr<Camera>(new Camera(appState->_viewportSize.x, appState->_viewportSize.y)));
}

void AlgGeom::SceneContent::addNewModel(Model3D* model)
{	
	_sceneAABB.update(model->getAABB());
	_model.push_back(std::unique_ptr<Model3D>(model));
}

AlgGeom::Model3D* AlgGeom::SceneContent::getModel(Model3D::Component* component)
{
	for (auto& model : _model)
	{
		if (model->belongsModel(component))
			return model.get();
	}

	return nullptr;
}
