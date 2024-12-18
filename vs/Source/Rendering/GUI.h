#pragma once

#include "ApplicationState.h"
#include "CameraGuiAdapter.h"
#include "ImGuizmo.h"
#include "SceneContent.h"
#include "Singleton.h"

namespace Tet
{
	class GUI: public Singleton<GUI>
	{
		friend class Singleton<GUI>;

	protected:
		enum MenuButtons { RENDERING, MODELS, CAMERA, LIGHT, SCREENSHOT, NUM_GUI_MENU_BUTTONS };
		enum FileDialog { OPEN_MESH, NONE };
		inline static std::string DEFAULT_DIRECTORY = ".";
		inline static std::vector<std::string> FILE_DIALOG_TEXT { "Choose Model" };
		inline static std::vector<std::string> FILE_DIALOG_EXTENSION { ".obj, .gltf, .fbx" };

		ApplicationState*									_appState;
		CameraGuiAdapter*									_cameraGuiAdapter;
		FileDialog											_fileDialog;
		Model3D::Component*									_modelCompSelected;
		std::string											_path, _lastDirectory;
		bool*												_showMenuButtons;

		// ImGuizmo
		ImGuizmo::OPERATION									_currentGizmoOperation;
		ImGuizmo::MODE										_currentGizmoMode;

	protected:
        static void editTransform(ImGuizmo::OPERATION& operation, ImGuizmo::MODE& mode);
        static void loadFonts();
		void loadImGUIStyle() const;
        static void processSelectedFile(FileDialog fileDialog, const std::string& filename, SceneContent* sceneContent);
		void renderGuizmo(const Model3D::Component* component, SceneContent* sceneContent);
		void showCameraMenu(const SceneContent* sceneContent) const;
		void showFileDialog(SceneContent* sceneContent);
		void showLightMenu(const SceneContent* sceneContent);
		void showModelMenu(const SceneContent* sceneContent);
		void showRenderingMenu(SceneContent* sceneContent);
		void showScreenshotMenu(SceneContent* sceneContent);

	protected:
		GUI();

	public:
		virtual ~GUI();

		void initialize(GLFWwindow* window, const int openGLMinorVersion) const;
		void render(SceneContent* sceneContent);

		uint16_t getFrameRate() { return static_cast<uint16_t>(ImGui::GetIO().Framerate); }
		bool isMouseActive() { return ImGui::GetIO().WantCaptureMouse; }
	};
}

