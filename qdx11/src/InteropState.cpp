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

#include "InteropState.h"

InteropState::InteropState()
    : m_width(800),
      m_height(600),
      m_phi(1.5f * 3.14f),
      m_theta(0.25f * 3.14f),
      m_radius(5) // #TODO
{
}

void InteropState::setTheta(float value)
{
	m_mutex.lock();
	m_theta = value;
	m_mutex.unlock();
}

void InteropState::setPhi(float value)
{
	m_mutex.lock();
	m_phi = value;
	m_mutex.unlock();
}

void InteropState::setRadius(float value)
{
	m_mutex.lock();
	m_radius = value;
	m_mutex.unlock();
}

void InteropState::setWidth(int value)
{
	m_mutex.lock();
	m_width = value;
	m_mutex.unlock();
}

void InteropState::setHeight(int value)
{
	m_mutex.lock();
	m_height = value;
	m_mutex.unlock();
}

float InteropState::getTheta()
{
	m_mutex.lock();
	float theta = m_theta;
	m_mutex.unlock();

	return theta;
}

float InteropState::getPhi()
{
	m_mutex.lock();
	float phi = m_phi;
	m_mutex.unlock();

	return phi;
}

float InteropState::getRadius()
{
	m_mutex.lock();
	float radius = m_radius;
	m_mutex.unlock();

	return m_radius;
}

int InteropState::getWidth()
{
	m_mutex.lock();
	int width = m_width;
	m_mutex.unlock();

	return width;
}

int InteropState::getHeight()
{
	m_mutex.lock();
	int height = m_height;
	m_mutex.unlock();

	return height;
}