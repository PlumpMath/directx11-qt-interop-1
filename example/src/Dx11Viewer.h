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

#ifndef DX11_VIEWER_H
#define DX11_VIEWER_H

#include "ui_d3dviewer.h"
#include <qdx11/Dx11Widget.h>
#include <qdx11/IRendererFactory.h>

/**
* @brief Example viewer showing Dx11Widget integration into other widgets.
*/
class Dx11Viewer : public QMainWindow
{
	Q_OBJECT

public:
	Dx11Viewer(QWidget* parent = 0, Qt::WFlags flags = 0);
	virtual ~Dx11Viewer();

protected:
	qdx11::Dx11Widget* m_dx11Widget;
    Ui::Dx11ViewerClass ui;
    qdx11::IRendererFactory* m_rendererFactory;
};

#endif // DX11_VIEWER_H