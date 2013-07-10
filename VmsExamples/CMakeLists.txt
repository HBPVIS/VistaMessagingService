# $Id: CMakeLists.txt 35308 2013-05-29 09:35:00Z bh736275 $

cmake_minimum_required( VERSION 2.8 )

if( NOT VMS_COMMON_BUILD )
	project( VmsExamples )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VMS_COMMON_BUILD)

# this variable indicates to sub-projects that they are build all together
set( VMSEXAMPLES_COMMON_BUILD TRUE )

vista_conditional_add_subdirectory( VMS_BUILD_SIMPLEISO_EXAMPLE VmsSimpleIsoSurface ${VMS_BUILD_EXAMPLES} )

