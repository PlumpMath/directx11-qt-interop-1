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

# Provide this find script to cmake.

# Locate QDX11
# This module defines
# QDX11_LIBRARIES
# QDX11_FOUND, if false, do not try to link to QDX11
# QDX11_INCLUDE_DIRS, where to find the headers
# QDX11_DIR corresponds to the environment variable QDX11_DIR
#

set(QDX11_DIR "$ENV{QDX11_DIR}" CACHE PATH "Location of QDX11")

MACRO(QDX11_FIND headername)

    FIND_PATH(QDX11_INCLUDE_DIR ${headername}
        PATHS
        ${QDX11_DIR}/include
        $ENV{QDX11_DIR}/include
        DOC "Location of QDX11 Headers"
    )

    FIND_LIBRARY(QDX11_LIBRARY
		NAMES QDX11
        PATHS
        ${QDX11_DIR}/lib
        $ENV{QDX11_DIR}/lib
        DOC "Location of QDX11 Libraries"
    )

    if(QDX11_LIBRARY AND QDX11_INCLUDE_DIR)
        SET(QDX11_FOUND 1)
    endif(QDX11_LIBRARY AND QDX11_INCLUDE_DIR)

ENDMACRO(QDX11_FIND)

MACRO(QDX11_FIND_DEBUG_AND_RELEASE headername)

	FIND_PATH(QDX11_INCLUDE_DIR ${headername}
        PATHS
        ${QDX11_DIR}/include
        $ENV{QDX11_DIR}/include
        DOC "Location of QDX11 Headers"
    )

    FIND_LIBRARY(QDX11_LIBRARY_RELEASE
        NAMES QDX11
        PATHS
        ${QDX11_DIR}/lib
        $ENV{QDX11_DIR}/lib
        DOC "Location of QDX11 Libraries"
    )
	
	FIND_LIBRARY(QDX11_LIBRARY_DEBUG
        NAMES QDX11d
        PATHS
        ${QDX11_DIR}/lib
        $ENV{QDX11_DIR}/lib
        DOC "Location of QDX11 Libraries"
    )

	set(QDX11_LIBRARY
		debug ${QDX11_LIBRARY_DEBUG}
		optimized ${QDX11_LIBRARY_RELEASE}
	)

    if(QDX11_LIBRARY AND QDX11_INCLUDE_DIR)
        SET(QDX11_FOUND 1)
    endif(QDX11_LIBRARY AND QDX11_INCLUDE_DIR)
ENDMACRO(QDX11_FIND_DEBUG_AND_RELEASE)


if(WIN32)
	set(QDX11_FOUND "NO")
	QDX11_FIND(qdx11/Dx11Widget.h)
	QDX11_FIND_DEBUG_AND_RELEASE(qdx11/Dx11Widget.h)
	
	if(QDX11_FOUND)
		
		message(STATUS "QDX11 found")
		
		set(QDX11_FOUND "YES")
	   
		set(QDX11_INCLUDE_DIRS
			${QDX11_WIDGET_INCLUDE_DIR}
		)
		
		set(QDX11_LIBRARIES
			${QDX11_WIDGET_LIBRARY}
		)
	endif()
endif()