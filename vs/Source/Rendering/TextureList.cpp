#include "stdafx.h"
#include "TextureList.h"

Tet::TextureList::TextureList()
{
}

Tet::TextureList::~TextureList()
{
    for (auto& pair : _colorTexture)
    {
        delete pair.second;
    }

    for (auto& pair : _imageTexture)
    {
        delete pair.second;
    }
}

Tet::Texture* Tet::TextureList::getTexture(const vec4& color)
{
    Tet::Texture* texture = nullptr;
    auto it = _colorTexture.find(color);

    if (it == _colorTexture.end())
    {
        texture = new Tet::Texture(color);
        _colorTexture[color] = texture;
    }
    else
        texture = it->second;

    return texture;
}

Tet::Texture* Tet::TextureList::getTexture(const std::string& path)
{
    Tet::Texture* texture = nullptr;
    auto it = _imageTexture.find(path);

    if (it == _imageTexture.end())
    {
        try
        {
            texture = new Tet::Texture(new Image(path));
            _imageTexture[path] = texture;
        }
        catch (std::runtime_error& error)
        {
            return nullptr;
        }
    }
    else
    {
        texture = it->second;
    }

    return texture;
}

void Tet::TextureList::saveTexture(const vec4& color, Tet::Texture* texture)
{
    _colorTexture[color] = texture;
}

void Tet::TextureList::saveTexture(const std::string& path, Tet::Texture* texture)
{
    _imageTexture[path] = texture;
}
