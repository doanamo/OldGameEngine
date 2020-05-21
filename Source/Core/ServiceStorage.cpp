/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#include "Precompiled.hpp"
#include "Core/ServiceStorage.hpp"
#include <System/Platform.hpp>
#include <System/Timer.hpp>
#include <System/FileSystem.hpp>
#include <System/ResourceManager.hpp>
#include <System/InputManager.hpp>
#include <System/Window.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Sprite/SpriteRenderer.hpp>
#include <Renderer/StateRenderer.hpp>
#include <Game/GameFramework.hpp>
#include <Editor/EditorSystem.hpp>
using namespace Core;

ServiceStorage::ServiceStorage() = default;
ServiceStorage::~ServiceStorage() = default;

void ServiceStorage::Provide(std::unique_ptr<System::Platform>&& platform)
{
    ASSERT(!m_platform);
    m_platform = std::move(platform);
}

void ServiceStorage::Provide(std::unique_ptr<System::FileSystem>&& fileSystem)
{
    ASSERT(!m_fileSystem);
    m_fileSystem = std::move(fileSystem);
}

void ServiceStorage::Provide(std::unique_ptr<System::Window>&& window)
{
    ASSERT(!m_window);
    m_window = std::move(window);
}

void ServiceStorage::Provide(std::unique_ptr<System::Timer>&& timer)
{
    ASSERT(!m_timer);
    m_timer = std::move(timer);
}

void ServiceStorage::Provide(std::unique_ptr<System::InputManager>&& inputManager)
{
    ASSERT(!m_inputManager);
    m_inputManager = std::move(inputManager);
}

void ServiceStorage::Provide(std::unique_ptr<System::ResourceManager>&& resourceManager)
{
    ASSERT(!m_resourceManager);
    m_resourceManager = std::move(resourceManager);
}

void ServiceStorage::Provide(std::unique_ptr<Graphics::RenderContext>&& renderContext)
{
    ASSERT(!m_renderContext);
    m_renderContext = std::move(renderContext);
}

void ServiceStorage::Provide(std::unique_ptr<Graphics::SpriteRenderer>&& spriteRenderer)
{
    ASSERT(!m_spriteRenderer);
    m_spriteRenderer = std::move(spriteRenderer);
}

void ServiceStorage::Provide(std::unique_ptr<Renderer::StateRenderer>&& stateRenderer)
{
    ASSERT(!m_stateRenderer);
    m_stateRenderer = std::move(stateRenderer);
}

void ServiceStorage::Provide(std::unique_ptr<Game::GameFramework>&& gameFramework)
{
    ASSERT(!m_gameFramework);
    m_gameFramework = std::move(gameFramework);
}

void ServiceStorage::Provide(std::unique_ptr<Editor::EditorSystem>&& editorSystem)
{
    ASSERT(!m_editorSystem);
    m_editorSystem = std::move(editorSystem);
}

System::Platform* ServiceStorage::GetPlatform() const
{
    ASSERT(m_platform);
    return m_platform.get();
}

System::FileSystem* ServiceStorage::GetFileSystem() const
{
    ASSERT(m_fileSystem);
    return m_fileSystem.get();
}

System::Window* ServiceStorage::GetWindow() const
{
    ASSERT(m_window);
    return m_window.get();
}

System::Timer* ServiceStorage::GetTimer() const
{
    ASSERT(m_timer);
    return m_timer.get();
}

System::InputManager* ServiceStorage::GetInputManager() const
{
    ASSERT(m_inputManager);
    return m_inputManager.get();
}

System::ResourceManager* ServiceStorage::GetResourceManager() const
{
    ASSERT(m_resourceManager);
    return m_resourceManager.get();
}

Graphics::RenderContext* ServiceStorage::GetRenderContext() const
{
    ASSERT(m_renderContext);
    return m_renderContext.get();
}

Graphics::SpriteRenderer* ServiceStorage::GetSpriteRenderer() const
{
    ASSERT(m_spriteRenderer);
    return m_spriteRenderer.get();
}

Renderer::StateRenderer* ServiceStorage::GetStateRenderer() const
{
    ASSERT(m_stateRenderer);
    return m_stateRenderer.get();
}

Game::GameFramework* ServiceStorage::GetGameFramework() const
{
    ASSERT(m_gameFramework);
    return m_gameFramework.get();
}

Editor::EditorSystem* ServiceStorage::GetEditorSystem() const
{
    ASSERT(m_editorSystem);
    return m_editorSystem.get();
}