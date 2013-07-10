# $Id: CMakeLists.txt 35546 2013-06-12 07:58:38Z bh736275 $

cmake_minimum_required( VERSION 2.8 )
if( NOT VMS_COMMON_BUILD )
	project( VmsBase )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VMS_COMMON_BUILD )

vista_use_package( VistaCoreLibs REQUIRED VistaBase VistaAspects VistaTools VistaInterProcComm FIND_DEPENDENCIES) 

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_library( VmsBase ${ProjectSources} )
#The following line prevent CMake from adding all depencies to other projects that link it from within the same cmake build
set_property( TARGET VmsBase PROPERTY LINK_INTERFACE_LIBRARIES "" )
target_link_libraries( VmsBase
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

vista_configure_lib( VmsBase )
if( VMS_COMMON_BUILD )
	vista_install( VmsBase VmsBase )
else( VMS_COMMON_BUILD )
	vista_install( VmsBase )
endif( VMS_COMMON_BUILD )
vista_create_cmake_configs( VmsBase )
vista_create_default_info_file( VmsBase )

