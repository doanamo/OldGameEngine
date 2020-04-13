/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#include "Editor/EditorRenderer.hpp"
#include <System/Window.hpp>
#include <System/ResourceManager.hpp>
using namespace Editor;

EditorRenderer::EditorRenderer(EditorRenderer&& other) :
    EditorRenderer()
{
    *this = std::move(other);
}

EditorRenderer& EditorRenderer::operator=(EditorRenderer&& other)
{
    std::swap(m_window, other.m_window);
    std::swap(m_renderContext, other.m_renderContext);

    std::swap(m_vertexBuffer, other.m_vertexBuffer);
    std::swap(m_indexBuffer, other.m_indexBuffer);
    std::swap(m_vertexArray, other.m_vertexArray);
    std::swap(m_fontTexture, other.m_fontTexture);
    std::swap(m_sampler, other.m_sampler);
    std::swap(m_shader, other.m_shader);

    std::swap(m_initialized, other.m_initialized);

    return *this;
}

bool EditorRenderer::Initialize(const InitializeFromParams& params)
{
    LOG("Initializing editor renderer...");
    LOG_SCOPED_INDENT();

    // Make sure class instance has not been initialized yet.
    ASSERT(!m_initialized, "Editor renderer has already been initialized!");

    // Initialization cleanup guard.
    SCOPE_GUARD_IF(!m_initialized, *this = EditorRenderer());

    // Validate engine reference.
    if(params.window == nullptr)
    {
        LOG_ERROR("Invalid argument - \"window\" is invalid!");
        return false;
    }

    if(params.fileSystem == nullptr)
    {
        LOG_ERROR("Invalid argument - \"fileSystem\" is invalid!");
        return false;
    }

    if(params.resourceManager == nullptr)
    {
        LOG_ERROR("Invalid argument - \"resourceManager\" is invalid!");
        return false;
    }

    if(params.renderContext == nullptr)
    {
        LOG_ERROR("Invalid argument - \"renderContext\" is invalid!");
        return false;
    }

    m_window = params.window;
    m_renderContext = params.renderContext;

    // We are expecting ImGui context to be set by the caller.
    ASSERT(ImGui::GetCurrentContext() != nullptr, "Editor interface context is not set!");

    // Create vertex buffer.
    Graphics::BufferInfo vertexBufferInfo;
    vertexBufferInfo.usage = GL_STREAM_DRAW;
    vertexBufferInfo.elementSize = sizeof(ImDrawVert);

    if(!m_vertexBuffer.Initialize(m_renderContext, vertexBufferInfo))
    {
        LOG_ERROR("Could not initialize vertex buffer!");
        return false;
    }

    // Create index buffer.
    Graphics::BufferInfo indexBufferInfo;
    indexBufferInfo.usage = GL_STREAM_DRAW;
    indexBufferInfo.elementSize = sizeof(ImDrawIdx);

    if(!m_indexBuffer.Initialize(m_renderContext, indexBufferInfo))
    {
        LOG_ERROR("Could not initialize index buffer!");
        return false;
    }

    // Create input layout.
    const Graphics::VertexAttribute inputAttributes[] =
    {
        { &m_vertexBuffer, Graphics::VertexAttributeType::Vector2, GL_FLOAT,         false }, // Position
        { &m_vertexBuffer, Graphics::VertexAttributeType::Vector2, GL_FLOAT,         false }, // Texture
        { &m_vertexBuffer, Graphics::VertexAttributeType::Vector4, GL_UNSIGNED_BYTE, true }, // Color
    };

    Graphics::VertexArrayInfo inputLayoutInfo;
    inputLayoutInfo.attributeCount = Utility::StaticArraySize(inputAttributes);
    inputLayoutInfo.attributes = &inputAttributes[0];

    if(!m_vertexArray.Initialize(m_renderContext, inputLayoutInfo))
    {
        LOG_ERROR("Could not initialize vertex array!");
        return false;
    }

    // Retrieve built in font data.
    unsigned char* fontData = nullptr;

    int fontWidth = 0;
    int fontHeight = 0;

    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&fontData, &fontWidth, &fontHeight);

    if(fontData == nullptr || fontWidth == 0 || fontHeight == 0)
    {
        LOG_ERROR("Could not retrieve font data!");
        return false;
    }

    // Create font texture.
    Graphics::Texture::CreateFromParams textureParams;
    textureParams.renderContext = m_renderContext;
    textureParams.width = fontWidth;
    textureParams.height = fontHeight;
    textureParams.format = GL_RGBA;
    textureParams.mipmaps = false;
    textureParams.data = fontData;

    if(!m_fontTexture.Initialize(textureParams))
    {
        LOG_ERROR("Could not initialize font texture!");
        return false;
    }

    ImGui::GetIO().Fonts->TexID = (void*)(intptr_t)m_fontTexture.GetHandle();

    // Create sampler.
    // Set linear filtering otherwise textures without mipmaps will be black.
    Graphics::SamplerInfo samplerInfo;
    samplerInfo.textureMinFilter = GL_LINEAR;
    samplerInfo.textureMagFilter = GL_LINEAR;

    if(!m_sampler.Initialize(m_renderContext, samplerInfo))
    {
        LOG_ERROR("Could not initialize sampler!");
        return false;
    }

    // Load shader.
    Graphics::Shader::LoadFromFile shaderParams;
    shaderParams.fileSystem = params.fileSystem;
    shaderParams.filePath = "Data/Engine/Shaders/Interface.shader";
    shaderParams.renderContext = m_renderContext;

    m_shader = params.resourceManager->Acquire<Graphics::Shader>(shaderParams.filePath, shaderParams);

    if(m_shader == nullptr)
    {
        LOG_ERROR("Could not initialize shader!");
        return false;
    }

    // Success!
    return m_initialized = true;
}

void EditorRenderer::Draw()
{
    ASSERT(m_initialized, "Editor renderer has not been initialized!");

    // We are expecting ImGui context to be set by the caller.
    ASSERT(ImGui::GetCurrentContext() != nullptr, "Editor interface context is not set!");

    // Get current window dimensions.
    int windowWidth = m_window->GetWidth();
    int windowHeight = m_window->GetHeight();

    // Access ImGui context.
    ImGuiIO& io = ImGui::GetIO();

    // Set current display size.
    io.DisplaySize.x = (float)windowWidth;
    io.DisplaySize.y = (float)windowHeight;

    // Handle case where Update() has not been called yet.
    // Calling NewFrame() twice in a row should be fine.
    if(ImGui::GetFrameCount() == 0)
    {
        ImGui::NewFrame();
    }

    // End our rendering frame.
    ImGui::EndFrame();
    ImGui::Render();

    // Get interface render data.
    ImDrawData* drawData = ImGui::GetDrawData();

    // Calculate rendering transform.
    glm::mat4 transform = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);

    // Push a rendering state.
    auto& renderState = m_renderContext->PushState();
    SCOPE_GUARD(m_renderContext->PopState());

    // Prepare rendering state.
    renderState.Enable(GL_BLEND);
    renderState.BlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderState.BlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    renderState.Enable(GL_SCISSOR_TEST);

    renderState.Viewport(0, 0, windowWidth, windowHeight);

    renderState.UseProgram(m_shader->GetHandle());
    m_shader->SetUniform("vertexTransform", transform);
    m_shader->SetUniform("textureDiffuse", 0);

    renderState.BindSampler(0, m_sampler.GetHandle());

    // Process draw data.
    ImVec2 position = drawData->DisplayPos;
    for(int list = 0; list < drawData->CmdListsCount; ++list)
    {
        const ImDrawList* commandList = drawData->CmdLists[list];
        const ImDrawIdx* indexBufferOffset = 0;

        m_vertexBuffer.Update(commandList->VtxBuffer.Data, commandList->VtxBuffer.Size);
        m_indexBuffer.Update(commandList->IdxBuffer.Data, commandList->IdxBuffer.Size);

        for(int command = 0; command < commandList->CmdBuffer.Size; ++command)
        {
            const ImDrawCmd* drawCommand = &commandList->CmdBuffer[command];

            if(drawCommand->UserCallback)
            {
                drawCommand->UserCallback(commandList, drawCommand);
            }
            else
            {
                ImVec4 clipRect;
                clipRect.x = drawCommand->ClipRect.x - position.x;
                clipRect.y = drawCommand->ClipRect.y - position.y;
                clipRect.z = drawCommand->ClipRect.z - position.x;
                clipRect.w = drawCommand->ClipRect.w - position.y;

                glm::ivec4 scissorRect;
                scissorRect.x = (int)clipRect.x;
                scissorRect.y = (int)(windowHeight - clipRect.w);
                scissorRect.z = (int)(clipRect.z - clipRect.x);
                scissorRect.w = (int)(clipRect.w - clipRect.y);

                renderState.Scissor(scissorRect.x, scissorRect.y, scissorRect.z, scissorRect.w);

                renderState.ActiveTexture(GL_TEXTURE0);
                renderState.BindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)drawCommand->TextureId);

                renderState.BindVertexArray(m_vertexArray.GetHandle());
                renderState.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.GetHandle());
                renderState.DrawElements(GL_TRIANGLES, (GLsizei)drawCommand->ElemCount, GL_UNSIGNED_SHORT, indexBufferOffset);
            }

            indexBufferOffset += drawCommand->ElemCount;
        }
    }
}
