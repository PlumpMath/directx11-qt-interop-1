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

#include <Windows.h>
#include "PerformanceTimer.h"

namespace qdx11
{
    PerformanceTimer::PerformanceTimer()
        : m_prevTime(0),
        m_currTime(0),
        m_baseTime(0),
        m_secsPerCnt(0.0),
        m_deltaTime(-1.0)
    {
        __int64 cntsPerSec;
        QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
        m_secsPerCnt = 1.0 / static_cast<double>(cntsPerSec);
    }

    float PerformanceTimer::totalTime() const
    {
        return static_cast<float>((m_currTime - m_baseTime) * m_secsPerCnt);
    }

    float PerformanceTimer::deltaTime() const
    {
        return static_cast<float>(m_deltaTime);
    }

    void PerformanceTimer::start()
    {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        m_baseTime = currTime;
        m_prevTime = currTime;
    }

    void PerformanceTimer::perFrame()
    {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        m_currTime = currTime;
        m_deltaTime = (m_currTime - m_prevTime) * m_secsPerCnt;
        m_prevTime = m_currTime;
    }
} // namespace qdx11