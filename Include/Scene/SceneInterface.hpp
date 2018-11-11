/*
    Copyright (c) 2018 Piotr Doan. All rights reserved.
*/

#pragma once

/*
    Scene
*/

namespace Scene
{
    // Scene draw parameters structure.
    struct SceneDrawParams
    {
        SceneDrawParams();

        // Name of camera entity.
        std::string cameraName;

        // Viewport rectangle in pixels.
        glm::ivec4 viewportRect;

        // Time alpha between frames.
        // Used for interpolation calculations.
        float timeAlpha;
        // Returns the viewport size.
        glm::ivec2 CalculateViewportSize() const;
    };

    // Scene interface.
    class SceneInterface
    {
    public:
        // Called when the scene is about to enter.
        virtual void OnEnter()
        {
        }

        // Called when the scene is about to exit.
        virtual void OnExit()
        {
        }

        // Called when the scene need to be updated.
        virtual void OnUpdate(float timeDelta)
        {
        }

        // Called when the scene needs to be drawn.
        virtual void OnDraw(const SceneDrawParams& drawParams)
        {
        }

        // Returns the scene name.
        virtual const char* GetDebugName() const
        {
            return "Unnamed";
        }
    };
}