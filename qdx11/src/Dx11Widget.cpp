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
#include "Dx11Widget.h"

// qt
#include <QResizeEvent>

// std
#include <iostream>
#include <sstream>

namespace qdx11
{
    Dx11Widget::Dx11Widget(IRendererFactory* factory, QWidget* parent /* = NULL */, Qt::WindowFlags flags /* = 0 */)
	    : QWidget(parent, flags),
	      m_leftMouseDown(false),
	      m_rightMouseDown(false)
    {
        m_renderer = factory->create(winId());

        setAttribute(Qt::WA_PaintOnScreen, true);
        setAttribute(Qt::WA_NativeWindow, true);

        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(0);

        setFocus();
    }

    Dx11Widget::~Dx11Widget()
    {
	    if(m_renderer)
		    delete m_renderer;
    }

    void Dx11Widget::paintEvent(QPaintEvent* event)
    {
	    m_renderer->frame();
    }

    void Dx11Widget::resizeEvent(QResizeEvent* event)
    {
	    m_renderer->setViewportWidth(width());
	    m_renderer->setViewportHeight(height());
        update();
    }

    void Dx11Widget::mousePressEvent(QMouseEvent* e)
    {
        setFocus();

	    if (e->button() == Qt::LeftButton)
	    {
		    m_leftMouseDown = true;
	    } 
	    else if (e->button() == Qt::RightButton)
	    {
		    m_rightMouseDown = true;
	    }

        m_prevMousePos = e->pos();
    }

    void Dx11Widget::mouseReleaseEvent(QMouseEvent* e)
    {
	    if (e->button() == Qt::LeftButton)
	    {
		    m_leftMouseDown = false;
	    }

	    if (e->button() == Qt::RightButton)
	    {
		    m_rightMouseDown = false;
	    }
    }

    void Dx11Widget::mouseMoveEvent(QMouseEvent* e)
    {
        m_prevMousePos = e->pos();
    }

    void Dx11Widget::wheelEvent(QWheelEvent* e)
    {

    }

    void Dx11Widget::keyPressEvent(QKeyEvent* e)
    {

    }
} // namespace qdx11