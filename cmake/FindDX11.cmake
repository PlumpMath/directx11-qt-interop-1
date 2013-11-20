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

# Locate DirectX11
# This module defines
# DX11_LIBRARIES
# DX11_FOUND, if false, do not try to link to DirectX11
# DX11_INCLUDE_DIRS, where to find the headers
# DX11_DIR corresponds to the environment variable DXSDK_DIR 
#

set(DX11_DIR "$ENV{DXSDK_DIR}" CACHE PATH "Location of DirectX11")

# check if system is 32bit or 64bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BINARY_DIR "x64")
else()
	set(BINARY_DIR "x86")
endif()

MACRO(DX11_FIND varname headername)

    FIND_PATH(DX11_${varname}_INCLUDE_DIR ${headername}
        PATHS
        ${DX11_DIR}/Include
        $ENV{DXSDK_DIR}/Include
		$ENV{DXSDK_DIR}/Samples/C++/Effects11/Inc
        DOC "Location of DirectX11 Headers"
    )

    FIND_LIBRARY(DX11_${varname}_LIBRARY
        NAMES ${varname}
        PATHS
        ${DX11_DIR}/lib/${BINARY_DIR}
        $ENV{DXSDK_DIR}/lib
        DOC "Location of DirectX11 Libraries"
    )

    if(DX11_${varname}_LIBRARY AND DX11_${varname}_INCLUDE_DIR)
        SET(DX11_${varname}_FOUND 1)
    endif(DX11_${varname}_LIBRARY AND DX11_${varname}_INCLUDE_DIR)

ENDMACRO(DX11_FIND)

MACRO(DX11_FIND_DEBUG_AND_RELEASE varname headername)

	FIND_PATH(DX11_${varname}_INCLUDE_DIR ${headername}
        PATHS
        ${DX11_DIR}/Include
        $ENV{DXSDK_DIR}/Include
		$ENV{DXSDK_DIR}/Samples/C++/Effects11/Inc
        DOC "Location of DirectX11 Headers"
    )

    FIND_LIBRARY(DX11_${varname}_LIBRARY_RELEASE
        NAMES ${varname}
        PATHS
        ${DX11_DIR}/lib/${BINARY_DIR}
        $ENV{DXSDK_DIR}/lib
        DOC "Location of DirectX11 Libraries"
    )
	
	FIND_LIBRARY(DX11_${varname}_LIBRARY_DEBUG
        NAMES ${varname}d
        PATHS
        ${DX11_DIR}/lib/${BINARY_DIR}
        $ENV{DXSDK_DIR}/lib
        DOC "Location of DirectX11 Libraries"
    )
	
	set(DX11_${varname}_LIBRARY
		debug ${DX11_${varname}_LIBRARY_DEBUG}
		optimized ${DX11_${varname}_LIBRARY_RELEASE}
	)

    if(DX11_${varname}_LIBRARY AND DX11_${varname}_INCLUDE_DIR)
        SET(DX11_${varname}_FOUND 1)
    endif(DX11_${varname}_LIBRARY AND DX11_${varname}_INCLUDE_DIR)
ENDMACRO(DX11_FIND_DEBUG_AND_RELEASE)


if(WIN32)
	DX11_FIND(D3D11 D3D11.h)
	DX11_FIND(D3DCOMPILER D3Dcompiler.h)
	DX11_FIND_DEBUG_AND_RELEASE(EFFECTS11 d3dx11effect.h)
	DX11_FIND(DXGI dxgi.h)
	DX11_FIND(DXGUID D3D11.h)
	
	set(DX11_FOUND "NO")
	
	if(DX11_D3D11_FOUND AND
		DX11_D3DCOMPILER_FOUND AND
		DX11_EFFECTS11_FOUND AND
		DX11_DXGI_FOUND AND
		DX11_DXGUID_FOUND)
		
		message(STATUS "DirectX11 found")
		
		set(DX11_FOUND "YES")
	   
		set(DX11_INCLUDE_DIRS
			${DX11_D3D11_INCLUDE_DIR}
			${DX11_D3DCOMPILER_INCLUDE_DIR}
			${DX11_EFFECTS11_INCLUDE_DIR}
			${DX11_DXGI_INCLUDE_DIR}
			${DX11_DXGUID_INCLUDE_DIR}
		)
		
		set(DX11_LIBRARIES
			${DX11_D3D11_LIBRARY}
			${DX11_D3DCOMPILER_LIBRARY}
			${DX11_EFFECTS11_LIBRARY}
			${DX11_DXGI_LIBRARY}
			${DX11_DXGUID_LIBRARY}
		)
	endif()
endif()