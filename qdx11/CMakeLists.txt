# Copyright (c) 2013, Hannes Würfel <hannes.wuerfel@student.hpi.uni-potsdam.de>
# Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright notice,
#	  this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#	  notice, this list of conditions and the following disclaimer in the
#	  documentation and/or other materials provided with the distribution.
#	* Neither the name of the Computer Graphics Systems Group at the
#	  Hasso-Plattner-Institute (HPI), Germany nor the names of its
#     contributors may be used to endorse or promote products derived from
#     this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set(target qdx11)
add_definitions(-DQDX11_LIBRARY) # adds COMPILE_DEFINITIONS for the current directory
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(HEADER_PATH ${CMAKE_SOURCE_DIR}/include/qdx11)

set(header
	${HEADER_PATH}/BasicRenderer.h
	${HEADER_PATH}/Dx11Widget.h
	${HEADER_PATH}/IRendererFactory.h
	${HEADER_PATH}/InteropState.h
	${HEADER_PATH}/PerformanceTimer.h
	${HEADER_PATH}/Utility.h
	${HEADER_PATH}/Export.h
)
	
set(sources
	src/BasicRenderer.cpp
	src/Dx11Widget.cpp
	src/InteropState.cpp
	src/PerformanceTimer.cpp
	src/Utility.cpp
	${header}
)
	
qt4_auto_wrap(sources ${sources})
add_library(${target} SHARED ${sources})
include_directories(${target} ${DX11_INCLUDE_DIRS} ${HEADER_PATH})
target_link_libraries(${target}
	${DX11_LIBRARIES}
	${QT_LIBRARIES}
)

configureDebugPostfix("d")
configureSourceGroups()

install(TARGETS ${target} DESTINATION lib)