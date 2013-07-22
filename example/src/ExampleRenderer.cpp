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

// own
#include "ExampleRenderer.h"
#include "Math.h"

// win
#include <D3D11.h>
#include <DXGI.h>

// std
#include <cassert>
#include <fstream>
#include <string>
#include <iostream>

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT2 TexCoord;
};

ExampleRenderer::ExampleRenderer(WId hwnd, bool enable4xMSAA, int width, int height)
    : BasicRenderer(hwnd, enable4xMSAA, width, height),
    m_vb(0),
    m_ib(0),
    m_testQuadVB(0),
    m_testQuadIB(0),
    m_fx(0),
    m_technique(0),
    m_techniqueTextured(0),
    m_fxWorldViewProjection(0),
    m_inputLayout(0),
    m_offscreenRTV(0),
    m_offscreenSRV(0),
    m_phi(1.5f * Math::Pi),
    m_theta(0.25f * Math::Pi),
    m_radius(10)
{
    if(!init())
    {
        std::cerr << "Error occured while creating Dx11 Context.\nAborting...\n";
        exit(0);
    }

    XMMATRIX identity = XMMatrixIdentity();
    XMStoreFloat4x4(&m_worldMatrix, identity);
    XMStoreFloat4x4(&m_viewMatrix, identity);
    XMStoreFloat4x4(&m_projectionMatrix, identity);
}


bool ExampleRenderer::init()
{
    if(!ExampleRenderer::initD3D())
    {
        return false;
    }

    loadFX();
    createVertexLayouts();

    createGeomBuffers();
    createTestQuadGeometryBuffers();
    createOffscreenRenderTargetView();

    return true;
}

void ExampleRenderer::handleInput()
{
    BasicRenderer::handleInput();

    XMMATRIX projection = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, aspectRatio(), 1.0f, 100.0f);
    XMStoreFloat4x4(&m_projectionMatrix, projection);
}

ExampleRenderer::~ExampleRenderer()
{
    m_context->ClearState();

    releaseCOM(m_vb);
    releaseCOM(m_ib);
    releaseCOM(m_fx);
    releaseCOM(m_inputLayout);
    releaseCOM(m_offscreenRTV);
    releaseCOM(m_offscreenSRV);
    releaseCOM(m_testQuadVB);
    releaseCOM(m_testQuadIB);
}

void ExampleRenderer::render()
{
    assert(m_context);
    assert(m_swapChain);

    float bgColor[] = {0.75f, 0.75f, 0.75f, 1.0f};

    ID3D11RenderTargetView* renderTargets[1] = {m_offscreenRTV};
    m_context->OMSetRenderTargets(1, renderTargets, m_depthStencilView);

    m_context->ClearRenderTargetView(m_offscreenRTV, bgColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    renderCube(); // render cube to offscreen texture
    //screenshot(); // debug render-to-texture

    // reset to backbuffer
    renderTargets[0] = m_renderTargetView;
    m_context->OMSetRenderTargets(1, renderTargets, m_depthStencilView);
    m_context->ClearRenderTargetView(m_renderTargetView, bgColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    renderCube();     // render cube again but now to the backbuffer
    renderTestQuad(); // render offscreen texture on test quad

    m_swapChain->Present(0, 0);

}

void ExampleRenderer::renderCube()
{
    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
    m_context->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);

    XMMATRIX world = XMLoadFloat4x4(&m_worldMatrix);
    XMMATRIX view = XMLoadFloat4x4(&m_viewMatrix);
    XMMATRIX projection = XMLoadFloat4x4(&m_projectionMatrix);
    XMMATRIX worldViewProjection = world * view * projection;

    m_fxWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));

    D3DX11_TECHNIQUE_DESC techDesc;
    m_technique->GetDesc(&techDesc);
    for(unsigned int i = 0; i < techDesc.Passes; ++i)
    {
        m_technique->GetPassByIndex(i)->Apply(0, m_context);
        m_context->DrawIndexed(36, 0, 0);
    }
}

void ExampleRenderer::renderTestQuad()
{
    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    XMMATRIX identity = XMMatrixIdentity();

    D3DX11_TECHNIQUE_DESC techDesc;
    m_techniqueTextured->GetDesc(&techDesc);
    for(unsigned int i = 0; i < techDesc.Passes; ++i)
    {
        m_context->IASetVertexBuffers(0, 1, &m_testQuadVB, &stride, &offset);
        m_context->IASetIndexBuffer(m_testQuadIB, DXGI_FORMAT_R32_UINT, 0);

        m_fxWorldViewProjection->SetMatrix(reinterpret_cast<const float*>(&identity));
        m_texture->SetResource(m_offscreenSRV); // bind offscreen texture as shader input texture

        m_techniqueTextured->GetPassByIndex(i)->Apply(0, m_context);
        m_context->DrawIndexed(6, 0, 0);
    }
}

void ExampleRenderer::updateScene()
{
    // convert spherical to cartesian coordinates
    float x = m_radius * sinf(m_phi) * cosf(m_theta);
    float z = m_radius * sinf(m_phi) * sinf(m_theta);
    float y = m_radius * cosf(m_phi);

    // build view matrix
    XMVECTOR eye = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR lookAt = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    XMMATRIX V = XMMatrixLookAtLH(eye, lookAt, up);
    XMStoreFloat4x4(&m_viewMatrix, V);
}

void ExampleRenderer::onResize()
{
    BasicRenderer::onResize();
    createOffscreenRenderTargetView();

    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, aspectRatio(), 0.1f, 100.0f);
    XMStoreFloat4x4(&m_projectionMatrix, P);
}

void ExampleRenderer::screenshot()
{
    ID3D11Resource* resource;
    if(!m_offscreenSRV)
    {
        return;
    }
    m_offscreenSRV->GetResource(&resource);

    ID3D11Texture2D* tex;
    resource->QueryInterface(&tex);

    HRESULT hr = D3DX11SaveTextureToFile(m_context, tex, D3DX11_IFF_PNG, L"test.png");
    if(FAILED(hr))
    {
        std::cout << "Error by writing texture to file.\n";
    }

    releaseCOM(tex);
    releaseCOM(resource);
}

void ExampleRenderer::createGeomBuffers()
{
    // vertex buffer
    float magenta[] = {1.0f, 0.0f, 1.0f, 1.0f};
    float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};

    Vertex vertices[] =
    {
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), magenta, XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, +1.0f, -1.0f), blue, XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(+1.0f, +1.0f, -1.0f), blue, XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(+1.0f, -1.0f, -1.0f), magenta, XMFLOAT2(1.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, +1.0f), magenta, XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, +1.0f, +1.0f), blue, XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(+1.0f, +1.0f, +1.0f), blue, XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(+1.0f, -1.0f, +1.0f), magenta, XMFLOAT2(1.0f, 1.0f)}
    };

    D3D11_BUFFER_DESC vbDesc;
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vbDesc.ByteWidth = sizeof(Vertex) * 8;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    vbDesc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    m_device->CreateBuffer(&vbDesc, &vinitData, &m_vb);

    // index buffer
    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        4, 5, 1,
        4, 1, 0,
        3, 2, 6,
        3, 6, 7,
        1, 5, 6,
        1, 6, 2,
        4, 0, 3, 
        4, 3, 7
    };

    D3D11_BUFFER_DESC ibDesc;
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.ByteWidth = sizeof(unsigned int) * 36;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    ibDesc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    m_device->CreateBuffer(&ibDesc, &iinitData, &m_ib);
}

void ExampleRenderer::createTestQuadGeometryBuffers()
{
    float magenta[] = {1.0f, 0.0f, 1.0f, 1.0f};
    float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};

    Vertex vertices[] =
    {
        {XMFLOAT3(0.0f, -1.0f, 0.0f), magenta, XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(0.0f, 0.0f, 0.0f), blue, XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(1.0f, 0.0f, 0.0f), blue, XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(+1.0f, -1.0f, 0.0f), magenta, XMFLOAT2(1.0f, 1.0f)},
    };

    D3D11_BUFFER_DESC vbDesc;
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vbDesc.ByteWidth = sizeof(Vertex) * 4;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vertexInitData;
    vertexInitData.pSysMem = vertices;
    HANDLE_ERROR(m_device->CreateBuffer(&vbDesc, &vertexInitData, &m_testQuadVB));

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    D3D11_BUFFER_DESC ibDesc;
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.ByteWidth = sizeof(UINT) * 6;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA indexInitData;
    indexInitData.pSysMem = indices;
    HANDLE_ERROR(m_device->CreateBuffer(&ibDesc, &indexInitData, &m_testQuadIB));
}

void ExampleRenderer::createOffscreenRenderTargetView()
{
    releaseCOM(m_offscreenSRV);
    releaseCOM(m_offscreenRTV);

    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = m_width;
    texDesc.Height = m_height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    texDesc.SampleDesc.Count = 4;
    texDesc.SampleDesc.Quality = m_4xMSAAQuality - 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    ID3D11Texture2D* offscreenTex = 0;
    HANDLE_ERROR(m_device->CreateTexture2D(&texDesc, 0, &offscreenTex));
    HANDLE_ERROR(m_device->CreateShaderResourceView(offscreenTex, 0, &m_offscreenSRV));
    HANDLE_ERROR(m_device->CreateRenderTargetView(offscreenTex, 0, &m_offscreenRTV));

    releaseCOM(offscreenTex);
}

void ExampleRenderer::loadFX()
{
    std::ifstream fin("simple.fxo", std::ios_base::binary);
    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    std::vector<char> compiledShader(size);
    fin.read(&compiledShader[0], size);
    fin.close();

    D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, m_device, &m_fx);   

    m_technique = m_fx->GetTechniqueByName("SimpleTechnique");
    m_techniqueTextured = m_fx->GetTechniqueByName("SimpleTechniqueTextured");
    m_texture = m_fx->GetVariableByName("tex")->AsShaderResource();
    m_fxWorldViewProjection = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void ExampleRenderer::createVertexLayouts()
{
    D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3DX11_PASS_DESC passDesc;
    m_technique->GetPassByIndex(0)->GetDesc(&passDesc);
    m_device->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_inputLayout);
}

void ExampleRenderer::setTheta(float value)
{
    m_theta = value;
}

void ExampleRenderer::setPhi(float value)
{
    m_phi = value;
}

void ExampleRenderer::setRadius(float value)
{
    m_radius = value;
}

float ExampleRenderer::theta()
{
    return m_theta;
}

float ExampleRenderer::phi()
{
    return m_phi;
}

float ExampleRenderer::radius()
{
    return m_radius;
}