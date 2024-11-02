/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2023 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_BACKEND_RENDER_TARGET_GLES2_HPP
#define TGUI_BACKEND_RENDER_TARGET_GLES2_HPP

#include <TGUI/Backend/Renderer/GLES2/BackendTextureGLES2.hpp>

#if TGUI_BUILD_AS_CXX_MODULE
    import tgui;
#else
    #include <TGUI/Backend/Renderer/BackendRenderTarget.hpp>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGUI_MODULE_EXPORT namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Render target implementation that makes use of OpenGL ES
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API BackendRenderTargetGLES2 : public BackendRenderTarget
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BackendRenderTargetGLES2();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~BackendRenderTargetGLES2() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Select the color that will be used by clearScreen
        /// @param color  Background color of the window
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setClearColor(const Color& color) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Clears the screen, called at the beginning of each frame when gui.mainLoop() is called
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void clearScreen() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Informs the render target about which part of the window is used for rendering
        ///
        /// @param view        Defines which part of the gui is being shown
        /// @param viewport    Defines which part of the window is being rendered to
        /// @param targetSize  Size of the window
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setView(FloatRect view, FloatRect viewport, Vector2f targetSize) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws the gui and all of its widgets
        ///
        /// @param root  Root container that holds all widgets in the gui
        ///
        /// GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE and GL_SCISSOR_TEST are temporarily changed but restored at the end of this function
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void drawGui(const std::shared_ptr<RootContainer>& root) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws a vertex array. This is called by the other draw functions.
        ///
        /// @param states       Render states to use for drawing
        /// @param vertices     Pointer to first element in array of vertices
        /// @param vertexCount  Amount of elements in the vertex array
        /// @param indices      Pointer to first element in array of indices
        /// @param indexCount   Amount of elements in the indices array
        /// @param texture      Texture to use, or nullptr when drawing colored triangles
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void drawVertexArray(const RenderStates& states, const Vertex* vertices, std::size_t vertexCount,
                             const unsigned int* indices, std::size_t indexCount, const std::shared_ptr<BackendTexture>& texture) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Called from addClippingLayer and removeClippingLayer to apply the clipping
        ///
        /// @param clipRect      View rectangle to apply
        /// @param clipViewport  Viewport to apply
        ///
        /// Both rectangles may be empty when nothing that will be drawn is going to be visible.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateClipping(FloatRect clipRect, FloatRect clipViewport) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Calls glEnableVertexAttribArray and glVertexAttribPointer.
        // Called only once when using a VAO (GLES 3.x), but on every draw when using GLES 2.0
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setVertexAttribs();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Creates the vertex and index buffers
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void createBuffers();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        unsigned int m_shaderProgram = 0;
        unsigned int m_vertexArray = 0;
        unsigned int m_vertexBuffer = 0;
        unsigned int m_indexBuffer = 0;
        std::size_t m_vertexBufferSize = 0;
        std::size_t m_indexBufferSize = 0;

        std::unique_ptr<BackendTextureGLES2> m_emptyTexture;
        std::shared_ptr<BackendTextureGLES2> m_currentTexture;

        Transform m_projectionTransform;
        int m_projectionMatrixShaderUniformLocation = 0;
        unsigned int m_positionShaderLocation = 0;
        unsigned int m_colorShaderLocation = 1;
        unsigned int m_texCoordShaderLocation = 2;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_BACKEND_RENDER_TARGET_GLES2_HPP
