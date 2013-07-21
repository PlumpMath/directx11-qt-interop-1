// Copyright (c) 2013, Hannes Würfel <hannes.wuerfel@student.hpi.uni-potsdam.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef EXAMPLE_RENDERER_H
#define EXAMPLE_RENDERER_H

#include <qdx11/BasicRenderer.h>

/**
* @brief The example DirectX11 Implementation with vbo, ibo and shader.
*/
class ExampleRenderer : public BasicRenderer
{
public:
    ExampleRenderer(WId hwnd, int width, int height, int frameLimiter = 60);
    virtual ~ExampleRenderer();

    void setTheta(float value);
    void setPhi(float value);
    void setRadius(float value);

    float theta();
    float phi();
    float radius();

    virtual void frame();

protected:
    virtual bool init();
    virtual void handleInput();
    virtual void updateScene();
    virtual void render();
    virtual void onResize();

private:
    void createGeomBuffers();
    void createVertexLayouts();
    void createOffscreenRenderTargetView();
    void createTestQuadGeometryBuffers();
    void loadFX();

    void screenshot();
    void renderTestQuad();
    void renderCube();

private:
    ID3D11Buffer* m_vb;
    ID3D11Buffer* m_ib;
    ID3DX11Effect* m_fx;
    ID3DX11EffectTechnique* m_technique;
    ID3DX11EffectTechnique* m_techniqueTextured;
    ID3DX11EffectMatrixVariable* m_fxWorldViewProjection;
    ID3DX11EffectShaderResourceVariable* m_texture;
    ID3D11InputLayout* m_inputLayout;
    XMFLOAT4X4 m_worldMatrix;
    XMFLOAT4X4 m_viewMatrix;
    XMFLOAT4X4 m_projectionMatrix;

    // render to texture
    ID3D11Buffer* m_testQuadVB;
    ID3D11Buffer* m_testQuadIB;

    ID3D11ShaderResourceView* m_offscreenSRV;
    ID3D11RenderTargetView* m_offscreenRTV;

    float m_theta;
    float m_phi;
    float m_radius;
};
#endif // EXAMPLE_RENDERER_H