#pragma once

#include "Model3D.h"

namespace Tet
{
	class DrawMesh: public Model3D
	{
	protected:
		Assimp::Importer _assimpImporter;

	protected:
        static Model3D::Component* processMesh(const aiMesh* mesh, const aiScene* scene, const std::string& folder);
		void processNode(const aiNode* node, const aiScene* scene, const std::string& folder);

	public:
		DrawMesh();
		virtual ~DrawMesh();

		DrawMesh* loadModelOBJ(const std::string& path);
	};
}

