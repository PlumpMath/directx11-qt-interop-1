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
#include "ExampleWidget.h"
#include "ExampleRenderer.h"
#include "Math.h"

    // qt
#include <QResizeEvent>

    // std
#include <iostream>
#include <sstream>

using namespace qdx11;

ExampleWidget::ExampleWidget(IRendererFactory* factory, QWidget* parent /* = NULL */, Qt::WindowFlags flags /* = 0 */)
    : Dx11Widget(factory, parent, flags)
{
    connect(m_renderer, SIGNAL(fpsChanged(float, float)), this, SLOT(onFPSChanged(float, float)));
}

ExampleWidget::~ExampleWidget()
{
    if(m_renderer)
        delete m_renderer;
}

void ExampleWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (m_leftMouseDown)
    {
        // orbit camera
        float dx = XMConvertToRadians(0.25f * static_cast<float>(e->pos().x() - m_prevMousePos.x()));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(e->pos().y() - m_prevMousePos.y()));

        float theta = static_cast<ExampleRenderer*>(m_renderer)->theta() + dx;
        float phi = static_cast<ExampleRenderer*>(m_renderer)->phi() + dy;
        phi = Math::clamp(phi, 0.1f, Math::Pi - 0.1f);

        static_cast<ExampleRenderer*>(m_renderer)->setTheta(theta);
        static_cast<ExampleRenderer*>(m_renderer)->setPhi(phi);
    }

    if (m_rightMouseDown)
    {
        // change distance from camera
        float dx = 0.05f * static_cast<float>(e->pos().x() - m_prevMousePos.x());
        float dy = 0.05f * static_cast<float>(e->pos().y() - m_prevMousePos.y());

        float radius = static_cast<ExampleRenderer*>(m_renderer)->radius() + dx - dy;
        radius = Math::clamp(radius, 5.0f, 20.0f);

        static_cast<ExampleRenderer*>(m_renderer)->setRadius(radius);
    }

    m_prevMousePos = e->pos();
    update();
}

void ExampleWidget::wheelEvent(QWheelEvent* e)
{
    //
    // see http://harmattan-dev.nokia.com/docs/platform-api-reference/xml/daily-docs/libqt4/qwheelevent.html
    //
    float d = 0.5f * static_cast<float>(e->delta() / 120);
    float radius = static_cast<ExampleRenderer*>(m_renderer)->radius() + d;

    radius = Math::clamp(radius, 5.0f, 20.0f);
    static_cast<ExampleRenderer*>(m_renderer)->setRadius(radius);
    update();
}

void ExampleWidget::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_Escape)
    {
        exit(0);
    }
}

void ExampleWidget::onFPSChanged(float fps, float mspf)
{
    std::stringstream sstream;
    sstream << "fps: " << fps << "   millisecs/frame: " << mspf << "ms";
    QString message(sstream.str().c_str());

    emit updateStatusBar(message);
}