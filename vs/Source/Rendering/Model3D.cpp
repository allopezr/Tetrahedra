#include "stdafx.h"
#include "Model3D.h"

// Static properties

std::string Tet::Model3D::CHECKER_PATTERN_PATH = "Assets/Textures/Checker.png";
std::unordered_set<std::string> Tet::Model3D::USED_NAMES;

// Public methods

Tet::Model3D::Model3D(): _modelMatrix(1.0f)
{
    this->overrideModelName();
}

Tet::Model3D::~Model3D()
= default;

Tet::Model3D* Tet::Model3D::copy()
{
    Model3D* newModel = new Model3D;

    newModel->_modelMatrix = this->_modelMatrix;
    newModel->_name = this->_name + " copy";
    newModel->_aabb = this->_aabb;

    for (auto& component : _components)
    {
        newModel->_components.push_back(std::unique_ptr<Component>(component->copy()));
    }

    return newModel;
}

bool Tet::Model3D::belongsModel(const Component* component) const
{
    for (auto& comp : _components)
    {
        if (comp.get() == component)
            return true;
    }

    return false;
}

void Tet::Model3D::draw(RenderingShader* shader, MatrixRenderInformation* matrixInformation, ApplicationState* appState, GLuint primitive)
{
    for (auto& component : _components)
    {
        if (component->_enabled && component->_vao)
        {
            VAO::IBO_slots rendering = VAO::IBO_TRIANGLE;

            switch (primitive)
            {
            case GL_TRIANGLES:
                if (component->_material._useUniformColor)
                {
                    shader->setUniform("Kd", component->_material._kdColor);
                    shader->setSubroutineUniform(GL_FRAGMENT_SHADER, "kadUniform", "getUniformColor");
                }
                else
                {
                    Texture* checkerPattern = TextureList::getInstance()->getTexture(CHECKER_PATTERN_PATH);
                    checkerPattern->applyTexture(shader, 0, "texKdSampler");
                    shader->setSubroutineUniform(GL_FRAGMENT_SHADER, "kadUniform", "getTextureColor");
                }

                shader->setUniform("Ks", component->_material._ksColor);
                shader->setUniform("metallic", component->_material._metallic);
                shader->setUniform("roughnessK", component->_material._roughnessK);
                shader->setUniform("mModelView", matrixInformation->multiplyMatrix(MatrixRenderInformation::VIEW, this->_modelMatrix));

                break;
            case GL_LINES:
                rendering = VAO::IBO_LINE;
                shader->setUniform("lineColor", component->_material._lineColor);
                glLineWidth(component->_lineWidth);

                break;
            case GL_POINTS:
                rendering = VAO::IBO_POINT;
                shader->setUniform("pointColor", component->_material._pointColor);
                shader->setUniform("pointSize", component->_pointSize);

                break;
            }

            if (!component->_activeRendering[rendering]) continue;
            
            shader->setUniform("mModelViewProj", matrixInformation->multiplyMatrix(MatrixRenderInformation::VIEW_PROJECTION, this->_modelMatrix));
            shader->applyActiveSubroutines();

            component->_vao->drawObject(rendering, primitive, static_cast<GLuint>(component->_indices[rendering].size()));

            matrixInformation->undoMatrix(MatrixRenderInformation::VIEW);
            matrixInformation->undoMatrix(MatrixRenderInformation::VIEW_PROJECTION);
        }
    }
}

Tet::Model3D* Tet::Model3D::moveGeometryToOrigin(const mat4& origMatrix, float maxScale)
{
    AABB aabb = this->getAABB();

    vec3 translate = -aabb.center();
    vec3 extent = aabb.extent();
    float maxScaleAABB = std::max(extent.x, std::max(extent.y, extent.z));
    vec3 scale = (maxScale < FLT_MAX) ? ((maxScale > maxScaleAABB) ? vec3(1.0f) : vec3(maxScale / maxScaleAABB)) : vec3(1.0f);

    _modelMatrix = glm::scale(glm::mat4(1.0f), scale) * glm::translate(glm::mat4(1.0f), translate) * origMatrix;

    return this;
}

Tet::Model3D* Tet::Model3D::overrideModelName()
{
    std::string className = typeid(*this).name();
    std::string classTarget = "class ";
    size_t classIndex = className.find(classTarget);
    if (classIndex != std::string::npos)
    {
        className = className.substr(classIndex + classTarget.size(), className.size() - classIndex - classTarget.size());
    }

    unsigned modelIdx = 0;
    bool nameValid = false;

    while (!nameValid)
    {
        this->_name = className + " " + std::to_string(modelIdx);
        nameValid = !USED_NAMES.contains(this->_name);
        ++modelIdx;
    }

    USED_NAMES.insert(this->_name);

    return this;
}

Tet::Model3D* Tet::Model3D::setLineColor(const vec3& color)
{
    for (auto& component : _components)
    {
        component->_material._lineColor = color;
    }

    return this;
}

Tet::Model3D* Tet::Model3D::setPointColor(const vec3& color)
{
    for (auto& component : _components)
    {
        component->_material._pointColor = color;
    }

    return this;
}

Tet::Model3D* Tet::Model3D::setTriangleColor(const vec4& color)
{
    for (auto& component : _components)
    {
        component->_material._kdColor = color;
    }

    return this;
}

Tet::Model3D* Tet::Model3D::setLineWidth(float width)
{
    for (auto& component : _components)
    {
        component->_lineWidth = width;
    }

    return this;
}

Tet::Model3D* Tet::Model3D::setPointSize(float size)
{
    for (auto& component : _components)
    {
        component->_pointSize = size;
    }

    return this;
}

Tet::Model3D* Tet::Model3D::setTopologyVisibility(VAO::IBO_slots topology, bool visible)
{
    for (auto& component : _components)
    {
        component->_activeRendering[topology] = visible;
    }

    return this;
}

std::vector<Tet::VAO::Vertex> Tet::Model3D::vertices() const
{
    std::vector<VAO::Vertex> vertices;

    for (const auto& component : _components)
        for (const VAO::Vertex& vertex : component->_vertices)
        {
            VAO::Vertex vertexCopy = vertex;
            vertexCopy._position = vec3(_modelMatrix * vec4(vertex._position, 1.0f));
            vertexCopy._normal = glm::normalize(vec3(_modelMatrix * vec4(vertex._normal, 0.0f)));
            vertices.push_back(vertexCopy);
        }

    return vertices;
}

std::vector<GLuint> Tet::Model3D::indices(VAO::IBO_slots ibo) const
{
    std::vector<GLuint> indices;

    for (const auto& component : _components)
        for (GLuint index : component->_indices[ibo])
            indices.push_back(index);

    return indices;
}

// Private methods

void Tet::Model3D::buildVao(Component* component)
{
    VAO* vao = new VAO(true);
    vao->setVBOData(component->_vertices);
    vao->setIBOData(VAO::IBO_POINT, component->_indices[VAO::IBO_POINT]);
    vao->setIBOData(VAO::IBO_LINE, component->_indices[VAO::IBO_LINE]);
    vao->setIBOData(VAO::IBO_TRIANGLE, component->_indices[VAO::IBO_TRIANGLE]);
    component->_vao = std::shared_ptr<VAO>(vao);
}

Tet::Model3D::Component* Tet::Model3D::getVoxel()
{
    Component* component = new Component;

    // Geometry
    {
        constexpr vec3 minPosition(-.5f), maxPosition(.5f);
        const std::vector<vec3> points
        {
            vec3(minPosition[0], minPosition[1], maxPosition[2]),		vec3(maxPosition[0], minPosition[1], maxPosition[2]),
            vec3(minPosition[0], minPosition[1], minPosition[2]),	    vec3(maxPosition[0], minPosition[1], minPosition[2]),
            vec3(minPosition[0], maxPosition[1], maxPosition[2]),		vec3(maxPosition[0], maxPosition[1], maxPosition[2]),
            vec3(minPosition[0], maxPosition[1], minPosition[2]),		vec3(maxPosition[0], maxPosition[1], minPosition[2])
        };
        const std::vector<vec3> normals
        {
            glm::normalize(vec3(-0.5f, -0.5f, 0.5f)),	glm::normalize(vec3(0.5f, -0.5f, 0.5f)),
            glm::normalize(vec3(-0.5f, -0.5f, -0.5f)),	glm::normalize(vec3(0.5f, -0.5f, -0.5f)),
            glm::normalize(vec3(-0.5f, 0.5f, 0.5f)),	glm::normalize(vec3(0.5f, 0.5f, 0.5f)),
            glm::normalize(vec3(-0.5f, 0.5f, -0.5f)),	glm::normalize(vec3(0.5f, 0.5f, -0.5f))
        };
        const std::vector<vec2> textCoords{ vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f) };

        for (int pointIdx = 0; pointIdx < points.size(); ++pointIdx)
        {
            component->_vertices.push_back(VAO::Vertex{ points[pointIdx], normals[pointIdx], textCoords[pointIdx] });
        }
    }

    // Topology
    {
        component->_indices[VAO::IBO_TRIANGLE] = std::vector<GLuint>
        {
            0, 1, 2, RESTART_PRIMITIVE_INDEX, 1, 3, 2, RESTART_PRIMITIVE_INDEX, 4, 5, 6, RESTART_PRIMITIVE_INDEX,
            5, 7, 6, RESTART_PRIMITIVE_INDEX, 0, 1, 4, RESTART_PRIMITIVE_INDEX, 1, 5, 4, RESTART_PRIMITIVE_INDEX,
            2, 0, 4, RESTART_PRIMITIVE_INDEX, 2, 4, 6, RESTART_PRIMITIVE_INDEX, 1, 3, 5, RESTART_PRIMITIVE_INDEX,
            3, 7, 5, RESTART_PRIMITIVE_INDEX, 3, 2, 6, RESTART_PRIMITIVE_INDEX, 3, 6, 7, RESTART_PRIMITIVE_INDEX
        };

        component->generatePointCloud();
        component->generateWireframe();
    }

    return component;
}

void Tet::Model3D::loadModelBinaryFile(const std::string& path)
{
    std::ifstream fin(path, std::ios::in | std::ios::binary);
    if (!fin.is_open())
    {
        std::cout << "Failed to open the binary file " << path << "!" << '\n';
        return;
    }

    size_t numComponents = _components.size();
    fin.read(reinterpret_cast<char*>(&numComponents), sizeof(size_t));
    _components.resize(numComponents);

    for (size_t compIdx = 0; compIdx < numComponents; ++compIdx)
    {
        Component* component = new Component;
        size_t numVertices, numIndices;

        fin.read(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
        component->_vertices.resize(numVertices);
        fin.read(reinterpret_cast<char*>(component->_vertices.data()), sizeof(VAO::Vertex) * numVertices);

        for (int topology = 0; topology < VAO::NUM_IBOS; ++topology)
        {
            fin.read(reinterpret_cast<char*>(&numIndices), sizeof(size_t));
            if (numIndices)
            {
                component->_indices[topology].resize(numIndices);
                fin.read(reinterpret_cast<char*>(component->_indices[topology].data()), sizeof(GLuint) * numIndices);
            }
        }

        fin.read(reinterpret_cast<char*>(&component->_aabb), sizeof(AABB));

        _components[compIdx] = std::unique_ptr<Component>(component);
        _aabb.update(_components[compIdx]->_aabb);
    }
}

void Tet::Model3D::updateAABB()
{
    _aabb = AABB();

    for (auto& component: _components)
    {
        component->updateAABB();
        _aabb.update(component->_aabb);
    }
}

void Tet::Model3D::writeBinaryFile(const std::string& path) const
{
    std::ofstream fout(path, std::ios::out | std::ios::binary);
    if (!fout.is_open())
    {
        std::cout << "Failed to write the binary file!" << std::endl;
    }

    size_t numComponents = _components.size();
    fout.write((char*)&numComponents, sizeof(size_t));

    for (auto& component: _components)
    {
        size_t numVertices = component->_vertices.size();

        fout.write((char*)&numVertices, sizeof(size_t));
        fout.write((char*)component->_vertices.data(), numVertices * sizeof(VAO::Vertex));

        for (int topology = 0; topology < VAO::NUM_IBOS; ++topology)
        {
            size_t numIndices = component->_indices[topology].size();
            fout.write((char*)&numIndices, sizeof(size_t));
            if (numIndices) 
                fout.write((char*)component->_indices[topology].data(), numIndices * sizeof(GLuint));
        }

        fout.write((char*)(&component->_aabb), sizeof(AABB));
    }

    fout.close();
}

Tet::Model3D::MatrixRenderInformation::MatrixRenderInformation()
{
    for (mat4& matrix : _matrix)
    {
        matrix = mat4(1.0f);
    }
}

void Tet::Model3D::MatrixRenderInformation::undoMatrix(MatrixType type)
{
    if (_heapMatrices[type].empty())
    {
        _matrix[type] = mat4(1.0f);
    }
    else
    {
        _matrix[type] = *(--_heapMatrices[type].end());
        _heapMatrices[type].erase(--_heapMatrices[type].end());
    }
}

Tet::Model3D::Component* Tet::Model3D::Component::copy() const
{
    Component* newComponent = new Component;

    newComponent->_enabled = this->_enabled;
    newComponent->_name = this->_name + " copy";

    newComponent->_activeRendering[VAO::IBO_POINT] = this->_activeRendering[VAO::IBO_POINT];
    newComponent->_activeRendering[VAO::IBO_LINE] = this->_activeRendering[VAO::IBO_LINE];
    newComponent->_activeRendering[VAO::IBO_TRIANGLE] = this->_activeRendering[VAO::IBO_TRIANGLE];

    newComponent->_vertices = this->_vertices;
    newComponent->_indices[VAO::IBO_POINT] = this->_indices[VAO::IBO_POINT];
    newComponent->_indices[VAO::IBO_LINE] = this->_indices[VAO::IBO_LINE];
    newComponent->_indices[VAO::IBO_TRIANGLE] = this->_indices[VAO::IBO_TRIANGLE];

    newComponent->_vao = this->_vao;
    newComponent->_aabb = this->_aabb;

    newComponent->_material = this->_material;
    newComponent->_lineWidth = this->_lineWidth;
    newComponent->_pointSize = this->_pointSize;

    return newComponent;
}

void Tet::Model3D::Component::completeTopology()
{
    if (!this->_indices[VAO::IBO_TRIANGLE].empty() && this->_indices[VAO::IBO_LINE].empty())
    {
        this->generateWireframe();
    }

    if (!this->_indices[VAO::IBO_LINE].empty() && this->_indices[VAO::IBO_POINT].empty())
    {
        this->generatePointCloud();
    }
}

void Tet::Model3D::Component::generateWireframe()
{
    std::unordered_map<int, std::unordered_set<int>>* segmentIncluded = new std::unordered_map<int, std::unordered_set<int>>;
    auto isIncluded = [&](int index1, int index2) -> bool
    {
        std::unordered_map<int, std::unordered_set<int>>::iterator it;

        if ((it = segmentIncluded->find(index1)) != segmentIncluded->end())
        {
            if (it->second.find(index2) != it->second.end())
            {
                return true;
            }
        }

        if ((it = segmentIncluded->find(index2)) != segmentIncluded->end())
        {
            if (it->second.find(index1) != it->second.end())
            {
                return true;
            }
        }

        return false;
    };

    const size_t numIndices = this->_indices[VAO::IBO_TRIANGLE].size();

    for (size_t i = 0; i < numIndices; i += 3)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            if (!isIncluded(this->_indices[VAO::IBO_TRIANGLE][i + j], this->_indices[VAO::IBO_TRIANGLE][(j + 1) % 3 + i]))
            {
                this->_indices[VAO::IBO_LINE].push_back(this->_indices[VAO::IBO_TRIANGLE][i + j]);
                this->_indices[VAO::IBO_LINE].push_back(this->_indices[VAO::IBO_TRIANGLE][(j + 1) % 3 + i]);
                this->_indices[VAO::IBO_LINE].push_back(RESTART_PRIMITIVE_INDEX);
            }
        }
    }
}

void Tet::Model3D::Component::generatePointCloud()
{
    this->_indices[VAO::IBO_POINT].resize(this->_vertices.size());
    std::iota(this->_indices[VAO::IBO_POINT].begin(), this->_indices[VAO::IBO_POINT].end(), 0);
}

void Tet::Model3D::Component::updateAABB()
{
    _aabb = AABB();

    for (const VAO::Vertex& vertex : _vertices)
    {
        _aabb.update(vertex._position);
    }
}
