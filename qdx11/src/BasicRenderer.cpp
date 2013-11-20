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
#include "BasicRenderer.h"
#include "Math.h"

// win
#include <D3D11.h>
#include <DXGI.h>

// std
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

#include <iostream>

namespace qdx11
{
    BasicRenderer::BasicRenderer(WId hwnd, bool enable4xMSAA, int width, int height)
        : m_windowHandle(hwnd),
	      m_driverType(D3D_DRIVER_TYPE_HARDWARE),
	      m_width(1),
	      m_height(1),
	      m_enable4xMSAA(enable4xMSAA),
	      m_4xMSAAQuality(0),
	      m_device(0),
	      m_context(0),
	      m_swapChain(0),
	      m_depthStencilBuffer(0),
	      m_renderTargetView(0),
	      m_depthStencilView(0)
    {
	    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    }


    bool BasicRenderer::init()
    {
	    if(!initD3D())
		    return false;

	    return true;
    }

    void BasicRenderer::frame()
    {
        static bool timerStarted = false;

        if(!timerStarted)
        {
            m_timer.start();
            timerStarted = true;
        }

        handleInput();
        m_timer.perFrame();
        calculateFPS();
        updateScene();
        render();
    }

    void BasicRenderer::handleInput()
    {
	    if(m_width != m_viewportWidth || m_height != m_viewportHeight)
	    {
		    m_width = m_viewportWidth;
		    m_height = m_viewportHeight;

		    onResize();
	    }
    }

    BasicRenderer::~BasicRenderer()
    {
	    m_swapChain->SetFullscreenState(FALSE, NULL);

	    releaseCOM(m_renderTargetView);
	    releaseCOM(m_depthStencilView);
	    releaseCOM(m_swapChain);
	    releaseCOM(m_depthStencilBuffer);

	    if (m_context)
	    {
		    m_context->ClearState();
	    }

	    releaseCOM(m_context);
	    releaseCOM(m_device);
    }

    void BasicRenderer::onResize()
    {
	    assert(m_context);
	    assert(m_device);
	    assert(m_swapChain);

	    releaseCOM(m_renderTargetView);
	    releaseCOM(m_depthStencilView);
	    releaseCOM(m_depthStencilBuffer);

	    m_swapChain->ResizeBuffers(1, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	    ID3D11Texture2D* backBuffer;
	    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	    m_device->CreateRenderTargetView(backBuffer, 0,&m_renderTargetView);
	    releaseCOM(backBuffer);

	    D3D11_TEXTURE2D_DESC depthStencilDesc;
	    depthStencilDesc.Width = m_width;
	    depthStencilDesc.Height = m_height;
	    depthStencilDesc.MipLevels = 1;
	    depthStencilDesc.ArraySize = 1;
	    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	    if (m_enable4xMSAA)
	    {
		    depthStencilDesc.SampleDesc.Count = 4;
		    depthStencilDesc.SampleDesc.Quality = m_4xMSAAQuality - 1;
	    }
	    else
	    {
		    depthStencilDesc.SampleDesc.Count = 1;
		    depthStencilDesc.SampleDesc.Quality = 0;
	    }

	    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	    depthStencilDesc.CPUAccessFlags = 0;
	    depthStencilDesc.MiscFlags = 0;

	    m_device->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer);
	    m_device->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView);

	    // bind to pipeline
	    m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	    m_viewport.TopLeftX = 0;
	    m_viewport.TopLeftY = 0;
	    m_viewport.Width = static_cast<float>(m_width);
	    m_viewport.Height = static_cast<float>(m_height);
	    m_viewport.MinDepth = 0.0f;
	    m_viewport.MaxDepth = 1.0f;

	    m_context->RSSetViewports(1, &m_viewport);
    }

    bool BasicRenderer::initD3D()
    {
	    unsigned int createDeviceFlags = 0;

    #	if defined(DEBUG) || defined(_DEBUG)
	    createDeviceFlags  |= D3D11_CREATE_DEVICE_DEBUG;
    #	endif

	    D3D_FEATURE_LEVEL featureLevel;
	    HRESULT hr = D3D11CreateDevice(
		    0,
		    m_driverType,
		    0,
		    createDeviceFlags,
		    0, 0,
		    D3D11_SDK_VERSION,
		    &m_device,
		    &featureLevel,
		    &m_context);

	    if (FAILED(hr))
	    {
		    return false;
	    }

	    if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	    {
		    return false;
	    }

	    m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality);
	    assert(m_4xMSAAQuality > 0);

	    DXGI_SWAP_CHAIN_DESC swapChainDesc;
	    swapChainDesc.BufferDesc.Width = m_width;
	    swapChainDesc.BufferDesc.Height = m_height;
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	    if( m_enable4xMSAA )
	    {
		    swapChainDesc.SampleDesc.Count   = 4;
		    swapChainDesc.SampleDesc.Quality = m_4xMSAAQuality-1;
	    }
	    else
	    {
		    swapChainDesc.SampleDesc.Count   = 1;
		    swapChainDesc.SampleDesc.Quality = 0;
	    }

	    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	    swapChainDesc.BufferCount = 1;
	    swapChainDesc.OutputWindow = reinterpret_cast<HWND>(m_windowHandle);
	    swapChainDesc.Windowed = true;
	    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	    IDXGIDevice* dxgiDevice = 0;
	    m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	    IDXGIDevice* dxgiAdapter = 0;
	    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	    IDXGIFactory* dxgiFactory = 0;
	    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	    dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);

	    releaseCOM(dxgiDevice);
	    releaseCOM(dxgiAdapter);
	    releaseCOM(dxgiFactory);

	    onResize();

	    return true;
    }

    float BasicRenderer::aspectRatio() const
    {
	    return static_cast<float>(m_width) / m_height;
    }

    void BasicRenderer::calculateFPS()
    {
	    static int frameCount = 0;
	    static float elapsedTime = 0.0f;

	    ++frameCount;

	    // compute averages over one second period
        if((m_timer.totalTime() - elapsedTime) >= 1.0f)
	    {
		    float fps = static_cast<float>(frameCount);
		    float millisecsPerFrame = 1000.0f / fps;
		    emit fpsChanged(fps, millisecsPerFrame);

		    frameCount = 0;
		    elapsedTime += 1.0f;
	    }
    }

    void BasicRenderer::setViewportWidth(int width)
    {
        m_viewportWidth = width;
    }

    int BasicRenderer::viewportWidth()
    {
        return m_viewportWidth;
    }

    void BasicRenderer::setViewportHeight(int height)
    {
        m_viewportHeight = height;
    }

    int BasicRenderer::viewportHeight()
    {
        return m_viewportHeight;
    }
} // namespace qdx11