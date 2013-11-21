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

# Add custom fxc build step for .fx files
# This module defines
# compileFX(fxFiles), use to specify the .fx files to be compiled
# DX11_FXC, if false, do not try to compile effect files offline
# DX11_FXC_FLAGS_DEBUG
# DX11_FXC_FLAGS_RELEASE
# DX11_FXC_BUILD_TYPE, specify 'debug' for debug build and assembly output, specify 'release' for release build
#

set(DX11_FXC_FLAGS_DEBUG /Fc /Od /Zi /T fx_5_0 /Fo CACHE STRING "fxc flags")
set(DX11_FXC_FLAGS_RELEASE /T fx_5_0 /Fo CACHE STRING "fxc flags")
set(DX11_FXC_BUILD_TYPE "release" CACHE STRING "build type")
SET_PROPERTY(CACHE DX11_FXC_BUILD_TYPE PROPERTY STRINGS release debug)

find_program(DX11_FXC
			 fxc
			 PATHS
			 ${DX11_DIR}/bin/${BINARY_DIR}
			 DOC "fx compiler")

if(NOT DX11_FXC)
	message(SEND_ERROR "Cannot find fxc.")
endif()

function(compileFX fxFiles rootDir)

	if(DX11_FXC_BUILD_TYPE MATCHES "debug")
		set(DX11_EFFECTS_FLAGS ${DX11_FXC_FLAGS_DEBUG})
	elseif(DX11_FXC_BUILD_TYPE MATCHES "release")
		set(DX11_EFFECTS_FLAGS ${DX11_FXC_FLAGS_RELEASE})
	endif()
	
	foreach(FILE ${fxFiles})
		get_filename_component(FILE_WE ${FILE} NAME_WE)
		add_custom_command(OUTPUT ${FILE_WE}.fxo
						   COMMAND ${DX11_FXC} ${DX11_EFFECTS_FLAGS} ${FILE_WE}.fxo ${rootDir}/${FILE}
						   MAIN_DEPENDENCY ${FILE}
						   COMMENT "Effect-compile ${FILE}"
						   VERBATIM
		)
	endforeach(FILE)
endfunction()

