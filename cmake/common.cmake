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

macro(qt4_auto_wrap outfiles)
    # clear accumulating variables
    unset(moc_headers)
    unset(ui_files)
    unset(qrc_files)

    # generate list of moc, uic, qrc files
    foreach(fileName ${ARGN})
        # moc: headers (WITH .h) need to contain "Q_OBJECT"
        if(fileName MATCHES "\\.h$")
            file(STRINGS ${fileName} lines REGEX Q_OBJECT)
            if(lines)
                set(moc_headers ${moc_headers} ${fileName})
                message(STATUS "moc: ${fileName}")
            endif()
        endif()

        # uic: files have extension ".ui"
        if(fileName MATCHES "\\.ui$")
            set(ui_files ${ui_files} ${fileName})
            message(STATUS "uic: ${fileName}")
        endif()

        # qrc: files have extension ".qrc"
        if(fileName MATCHES "\\.qrc$")
            set(qrc_files ${qrc_files} ${fileName})
            message(STATUS "qrc: ${fileName}")
        endif()
    endforeach()
    
    # use standard functions to handle these files
    QT4_WRAP_CPP(${outfiles} ${moc_headers})
    QT4_WRAP_UI(${outfiles} ${ui_files})
    QT4_ADD_RESOURCES(${outfiles} ${qrc_files})
    
    # add include directory for generated ui_*.h files
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(qt4_auto_wrap)
