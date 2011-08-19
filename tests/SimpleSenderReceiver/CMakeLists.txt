# $Id:$

cmake_minimum_required( VERSION 2.8 )
project( SimpleSenderReceiver )

include_directories( ${SimpleSenderReceiver_SOURCE_DIR} )

list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
include( VistaCommon )


# this variable indicates to sub-projects that they are build all together
set( SimpleSenderReceiver_COMMON_BUILD TRUE )

vista_conditional_add_subdirectory( SimpleSenderReceiver_BUILD_RECEIVER Receiver ON )
vista_conditional_add_subdirectory( SimpleSenderReceiver_BUILD_SENDER Sender ON )

