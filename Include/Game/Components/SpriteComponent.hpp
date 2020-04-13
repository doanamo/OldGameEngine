/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#pragma once

#include <Graphics/Sprite/Sprite.hpp>
#include <Graphics/TextureView.hpp>
#include "Game/Component.hpp"

/*
    Sprite Component
*/

namespace Game
{
    class TransformComponent;

    class SpriteComponent : public Component
    {
    public:
        SpriteComponent() = default;
        ~SpriteComponent() = default;

        SpriteComponent(SpriteComponent&& other);
        SpriteComponent& operator=(SpriteComponent&& other);

        void SetTextureView(Graphics::TextureView texture);
        void SetRectangle(const glm::vec4& rectangle);
        void SetColor(const glm::vec4& color);
        void SetTransparent(bool toggle);
        void SetFiltered(bool toggle);

        TransformComponent* GetTransformComponent() const;
        const Graphics::TextureView& GetTextureView() const;
        glm::vec4 GetRectangle() const;
        glm::vec4 GetColor() const;
        bool IsTransparent() const;
        bool IsFiltered() const;

    protected:
        bool OnInitialize(ComponentSystem* componentSystem, const EntityHandle& entitySelf) override;

    private:
        TransformComponent* m_transformComponent = nullptr;
        Graphics::TextureView m_textureView;
        glm::vec4 m_rectangle = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        bool m_transparent = false;
        bool m_filtered = true;
    };
}
