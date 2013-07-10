# $Id: CMakeLists.txt 35308 2013-05-29 09:35:00Z bh736275 $
cmake_minimum_required( VERSION 2.8 )

if( NOT VMSEXAMPLES_COMMON_BUILD )
	project( VmsExampleApp )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VMSEXAMPLES_COMMON_BUILD)

# this variable indicates to sub-projects that they are build all together
set( VMSSIMPLEISO_COMMON_BUILD TRUE )

add_subdirectory( VmsSimpleIsoClient )
add_subdirectory( VmsSimpleIsoServer )
add_subdirectory( VmsSimpleIsoVocabulary )

