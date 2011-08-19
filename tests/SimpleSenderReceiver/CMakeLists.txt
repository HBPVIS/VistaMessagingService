# $Id:$

cmake_minimum_required( VERSION 2.8 )
project( VistaMessagingProto )

include_directories( ${VistaMessagingProto_SOURCE_DIR} )

list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
include( VistaCommon )


# this variable indicates to sub-projects that they are build all together
set( VISTAMESSAGINGPROTO_COMMON_BUILD TRUE )

vista_conditional_add_subdirectory( VISTAMESSAGINGPROTO_BUILD_RECEIVER Receiver ON )
vista_conditional_add_subdirectory( VISTAMESSAGINGPROTO_BUILD_SENDER Sender ON )

