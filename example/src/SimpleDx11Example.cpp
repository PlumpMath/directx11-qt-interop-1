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
#include "SimpleDx11Example.h"
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
};

SimpleDx11Example::SimpleDx11Example(WId hwnd, InteropState* InteropState, int width, int height)
	: BasicRenderer(hwnd, InteropState, width, height),
	  m_vb(0),
	  m_ib(0),
	  m_fx(0),
	  m_technique(0),
	  m_fxWorldViewProjection(0),
	  m_inputLayout(0)
{
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
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


bool SimpleDx11Example::init()
{
	if(!SimpleDx11Example::initD3D())
    {
		return false;
    }

	createGeomBuffers();
	loadFX();
	createVertexLayouts();

	return true;
}

void SimpleDx11Example::handleInput()
{
	BasicRenderer::handleInput();

	XMMATRIX projection = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, aspectRatio(), 1.0f, 100.0f);
	XMStoreFloat4x4(&m_projectionMatrix, projection);
}

SimpleDx11Example::~SimpleDx11Example()
{
	releaseCOM(m_vb);
	releaseCOM(m_ib);
	releaseCOM(m_fx);
	releaseCOM(m_inputLayout);
}

void SimpleDx11Example::render()
{
	assert(m_context);
	assert(m_swapChain);

	float bgColor[] = {0.75f, 0.75f, 0.75f, 1.0f};
	//float bgColor[] = {0.94f, 0.94f, 0.94f, 1.0f};

	m_context->ClearRenderTargetView(m_renderTargetView, bgColor);
	m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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

	m_swapChain->Present(0, 0);
}

void SimpleDx11Example::updateScene()
{
	float theta = m_InteropState->getTheta();
	float phi = m_InteropState->getPhi();
	float radius = m_InteropState->getRadius();

	// convert spherical to cartesian coordinates
	float x = radius * sinf(phi) * cosf(theta);
	float z = radius * sinf(phi) * sinf(theta);
	float y = radius * cosf(phi);

	// build view matrix
	XMVECTOR eye = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR lookAt = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	XMMATRIX V = XMMatrixLookAtLH(eye, lookAt, up);
	XMStoreFloat4x4(&m_viewMatrix, V);
}

void SimpleDx11Example::createGeomBuffers()
{
	// vertex buffer
	float magenta[] = {1.0f, 0.0f, 1.0f, 1.0f};
	float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};

	Vertex vertices[] =
	{
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), magenta},
		{XMFLOAT3(-1.0f, +1.0f, -1.0f), blue},
		{XMFLOAT3(+1.0f, +1.0f, -1.0f), blue },
		{XMFLOAT3(+1.0f, -1.0f, -1.0f), magenta},
		{XMFLOAT3(-1.0f, -1.0f, +1.0f), magenta},
		{XMFLOAT3(-1.0f, +1.0f, +1.0f), blue},
		{XMFLOAT3(+1.0f, +1.0f, +1.0f), blue},
		{XMFLOAT3(+1.0f, -1.0f, +1.0f), magenta}
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
	UINT indices[] =
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
	ibDesc.ByteWidth = sizeof(UINT) * 36;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	m_device->CreateBuffer(&ibDesc, &iinitData, &m_ib);
}

void SimpleDx11Example::loadFX()
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
	m_fxWorldViewProjection = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void SimpleDx11Example::createVertexLayouts()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3DX11_PASS_DESC passDesc;
	m_technique->GetPassByIndex(0)->GetDesc(&passDesc);
	m_device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_inputLayout);
}