#pragma once

#include "ApplicationState.h"
#include "Camera.h"
#include "FBOScreenshot.h"
#include "GUI.h"
#include "InclDraw2D.h"
#include "InclGeom2D.h"
#include "InputManager.h"
#include "Model3D.h"
#include "RenderingShader.h"
#include "SceneContent.h"
#include "Singleton.h"

namespace Tet
{
	class Renderer : public Singleton<Renderer>, public Tet::ResizeListener, public Tet::ScreenshotListener
	{
		friend class Singleton<Renderer>;

	private:	
		ApplicationState*							_appState;
		SceneContent*								_content;
		GUI*										_gui;
		FBOScreenshot*								_screenshoter;
		RenderingShader*							_triangleShader, *_lineShader, *_pointShader;

	private:
		Renderer();

		void renderLine(Model3D::MatrixRenderInformation* matrixInformation) const;
		void renderPoint(Model3D::MatrixRenderInformation* matrixInformation) const;
		void renderTriangle(Model3D::MatrixRenderInformation* matrixInformation) const;
		void transferLightUniforms(RenderingShader* shader) const;

	public:
		virtual ~Renderer();
		void createCamera() const;
		void createModels() const;
		void createShaderProgram();
		Camera* getCamera() const { return _content->_camera[_appState->_selectedCamera].get(); }
		void prepareOpenGL(uint16_t width, uint16_t height, ApplicationState* appState);
		void removeModel() const;
		void render(float alpha = 1.0f, bool renderGui = true, bool bindScreenshoter = false) const;
		virtual void resizeEvent(uint16_t width, uint16_t height);
		virtual void screenshotEvent(const ScreenshotEvent& event);
	};
}

