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

set(target QDx11Example)

add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# needs to be there if one wants supress the console window for GUI Applications on Windows
if(WIN32 AND NOT QDX11_SHOW_DEBUG_CONSOLE)
	set(GUI_TYPE WIN32)
else()
	set(GUI_TYPE)
endif(WIN32 AND NOT QDX11_SHOW_DEBUG_CONSOLE)

set(sources
	src/main.cpp
	src/Dx11Viewer.h
	src/Dx11Viewer.cpp
	src/ui/d3dviewer.ui
	src/Math.h
	src/Math.cpp
	src/ExampleRenderer.h
	src/ExampleRenderer.cpp
	src/ExampleRendererFactory.h
	src/ExampleRendererFactory.cpp
	src/ExampleWidget.h
	src/ExampleWidget.cpp
	src/CamState.h
	src/CamState.cpp
)

set(fx
	src/fx/simple.fx
)

# moc compiler
qt4_auto_wrap(sources ${sources})

add_executable(${target} ${GUI_TYPE} ${sources} ${common_sources} ${fx})
configureDebugPostfix("d")
configureSourceGroups()
compileFX(${fx} ${CMAKE_SOURCE_DIR}/example)

set(QDX11_INCLUDE ${CMAKE_SOURCE_DIR}/include)
include_directories(${QDX11_INCLUDE})

target_link_libraries(${target}
	qdx11
	${DX11_LIBRARIES}
	${QT_QTMAIN_LIBRARY}
	${QT_LIBRARIES}
)

# install .exe and generated .fxo files
install(TARGETS ${target} DESTINATION bin)
FILE(GLOB files "${CMAKE_BINARY_DIR}/example/*.fxo")
INSTALL(FILES ${files} DESTINATION bin)