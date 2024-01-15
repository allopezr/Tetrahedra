cmake_minimum_required(VERSION 3.24)
project(AG2223_CLion)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

set(FONTS ./Fonts)
set(GEOMETRY ./Geometry)
set(LIBRARIES ./Libraries)
set(PATTERNS ./Patterns)
set(PRECOMPILED_HEADERS ./PrecompiledHeaders)
set(RENDERING ./Rendering)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

include_directories(${FONTS})
include_directories(${GEOMETRY})
include_directories(${LIBRARIES})
include_directories(${PATTERNS})
include_directories(${PRECOMPILED_HEADERS})
include_directories(${RENDERING})
include_directories(${LIBRARIES})
include_directories(${LIBRARIES}/imguifiledialog)
include_directories(${LIBRARIES}/imgizmo)
include_directories(${LIBRARIES}/imgui)
include_directories(${OPENGL_INCLUDE_DIR})

set_source_files_properties(stdafx.cpp
        PROPERTIES
        COMPILE_FLAGS "-include stdafx.h")

#link_directories(${VCPKG}/lib/x86_64-linux-gnu/cmake)

set(SOURCE_FILES
        main.cpp
        ${GEOMETRY}/AABB.h
        ${GEOMETRY}/AABB.cpp
        ${GEOMETRY}/BasicGeometry.h
        ${GEOMETRY}/ChronoUtilities.h
        ${GEOMETRY}/Circle.h
        ${GEOMETRY}/Circle.cpp
        ${GEOMETRY}/Line.h
        ${GEOMETRY}/Line.cpp
        ${GEOMETRY}/Point.h
        ${GEOMETRY}/Point.cpp
        ${GEOMETRY}/PointCloud.h
        ${GEOMETRY}/PointCloud.cpp
        ${GEOMETRY}/PolygonGeo.h
        ${GEOMETRY}/PolygonGeo.cpp
        ${GEOMETRY}/RandomUtilities.h
        ${GEOMETRY}/RayLine.h
        ${GEOMETRY}/RayLine.cpp
        ${GEOMETRY}/SegmentLine.h
        ${GEOMETRY}/SegmentLine.cpp
        ${GEOMETRY}/Triangle.h
        ${GEOMETRY}/Triangle.cpp
        ${GEOMETRY}/Vect2d.h
        ${GEOMETRY}/Vect2d.cpp
        ${GEOMETRY}/Vertex.h
        ${GEOMETRY}/Vertex.cpp
        ${PATTERNS}/Singleton.h
        ${PRECOMPILED_HEADERS}/InclDraw2D.h
        ${PRECOMPILED_HEADERS}/InclGeom2D.h
        ${PRECOMPILED_HEADERS}/stdafx.h
        ${PRECOMPILED_HEADERS}/stdafx.cpp
        ${RENDERING}/ApplicationState.h
        ${RENDERING}/Camera.h
        ${RENDERING}/Camera.cpp
        ${RENDERING}/CameraGuiAdapter.h
        ${RENDERING}/CameraGuiAdapter.cpp
        ${RENDERING}/CameraProjection.h
        ${RENDERING}/CameraProjection.cpp
        ${RENDERING}/DrawLine.h
        ${RENDERING}/DrawLine.cpp
        ${RENDERING}/DrawMesh.h
        ${RENDERING}/DrawMesh.cpp
        ${RENDERING}/DrawPoint.h
        ${RENDERING}/DrawPoint.cpp
        ${RENDERING}/DrawPointCloud.h
        ${RENDERING}/DrawPointCloud.cpp
        ${RENDERING}/DrawPolygon.h
        ${RENDERING}/DrawPolygon.cpp
        ${RENDERING}/DrawRay.h
        ${RENDERING}/DrawRay.cpp
        ${RENDERING}/DrawSegment.h
        ${RENDERING}/DrawSegment.cpp
        ${RENDERING}/DrawTriangle.h
        ${RENDERING}/DrawTriangle.cpp
        ${RENDERING}/FBO.h
        ${RENDERING}/FBO.cpp
        ${RENDERING}/FBOScreenshot.h
        ${RENDERING}/FBOScreenshot.cpp
        ${RENDERING}/GUI.h
        ${RENDERING}/GUI.cpp
        ${RENDERING}/GuiUtilities.h
        ${RENDERING}/GuiUtilities.cpp
        ${RENDERING}/Image.h
        ${RENDERING}/Image.cpp
        ${RENDERING}/InputManager.h
        ${RENDERING}/InputManager.cpp
        ${RENDERING}/InterfaceAdapter.h
        ${RENDERING}/Model3D.h
        ${RENDERING}/Model3D.cpp
        ${RENDERING}/RandomUtilities.h
        ${RENDERING}/Renderer.h
        ${RENDERING}/Renderer.cpp
        ${RENDERING}/RenderingShader.h
        ${RENDERING}/RenderingShader.cpp
        ${RENDERING}/SceneContent.h
        ${RENDERING}/SceneContent.cpp
        ${RENDERING}/ShaderProgram.h
        ${RENDERING}/ShaderProgram.cpp
        ${RENDERING}/ShaderProgramDB.h
        ${RENDERING}/ShaderProgramDB.cpp
        ${RENDERING}/Texture.h
        ${RENDERING}/Texture.cpp
        ${RENDERING}/TextureList.h
        ${RENDERING}/TextureList.cpp
        ${RENDERING}/VAO.h
        ${RENDERING}/VAO.cpp
        ${RENDERING}/Window.h
        ${RENDERING}/Window.cpp
        ${LIBRARIES}/lodepng.h
        ${LIBRARIES}/lodepng.cpp
        ${LIBRARIES}/imgui/imgui.h
        ${LIBRARIES}/imgui/imgui.cpp
        ${LIBRARIES}/imgui/imgui_draw.cpp
        ${LIBRARIES}/imgui/examples/imgui_impl_glfw.h
        ${LIBRARIES}/imgui/examples/imgui_impl_glfw.cpp
        ${LIBRARIES}/imgui/examples/imgui_impl_opengl3.cpp
        ${LIBRARIES}/imgui/imgui_tables.cpp
        ${LIBRARIES}/imgui/imgui_widgets.cpp
        ${LIBRARIES}/imgui/imgui_internal.h
        ${LIBRARIES}/imgui/imstb_truetype.h
        ${LIBRARIES}/imgizmo/ImZoomSlider.h
        ${LIBRARIES}/imgizmo/ImSequencer.h
        ${LIBRARIES}/imgizmo/ImSequencer.cpp
        ${LIBRARIES}/imgizmo/ImGuizmo.h
        ${LIBRARIES}/imgizmo/ImGuizmo.cpp
        ${LIBRARIES}/imgizmo/ImCurveEdit.h
        ${LIBRARIES}/imgizmo/ImCurveEdit.cpp
        ${LIBRARIES}/imgizmo/ImGradient.h
        ${LIBRARIES}/imgizmo/ImGradient.cpp
        ${LIBRARIES}/imguifiledialog/ImGuiFileDialog.h
        ${LIBRARIES}/imguifiledialog/ImGuiFileDialog.cpp
        ${LIBRARIES}/imguifiledialog/ImGuiFileDialogConfig.h
        ${FONTS}/lato.hpp
        ${FONTS}/lato.cpp
        ${FONTS}/IconsFontAwesome5.h
        ${FONTS}/font_awesome.hpp
        ${FONTS}/font_awesome.cpp
        ${FONTS}/font_awesome_2.cpp)
add_executable(AG2223_CLion ${SOURCE_FILES})

target_link_libraries(AG2223_CLion glfw GLEW OpenGL assimp tbb)
