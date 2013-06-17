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

#ifndef BASIC_RENDERER_H
#define BASIC_RENDERER_H

#include <QThread>
#include <QWidget>

// windows
#include <Windows.h>
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <xnamath.h>
#include <DxErr.h>

#include "InteropState.h"
#include "Utility.h"
#include "PerformanceTimer.h"

#include "Export.h"

class QDX11_API BasicRenderer : public QThread
{
	Q_OBJECT

public:
	BasicRenderer(WId hwnd, InteropState* InteropState, int width, int height, int frameLimiter = 60);
	virtual ~BasicRenderer();

signals:
	void fpsChanged(float, float);

protected:
	virtual void run();
	virtual bool init();
	virtual void handleInput();
	virtual void onResize();
	virtual void updateScene() {};
	virtual void render() = 0;

	bool initD3D();
	float aspectRatio() const;
	void calculateFPS();

protected:
	InteropState* m_InteropState;
	WId m_windowHandle;
	int m_width;
	int m_height;
	unsigned int m_4xMSAAQuality;
	bool m_enable4xMSAA;
	PerformanceTimer m_timer;
    float m_frameLimiter;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapChain;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	D3D_DRIVER_TYPE m_driverType;
};

#endif // BASIC_RENDERER_H