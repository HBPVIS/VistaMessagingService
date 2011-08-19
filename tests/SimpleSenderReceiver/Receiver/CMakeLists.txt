# $Id:$

cmake_minimum_required( VERSION 2.8 )
if( NOT VISTAMESSAGINGPROTO_COMMON_BUILD )
	project( Receiver )

	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VISTAMESSAGINGPROTO_COMMON_BUILD )

#include necessary dependencies
vista_use_package( ZMQ REQUIRED )
vista_use_package( VistaCoreLibs REQUIRED VistaBase VistaTools VistaMath VistaAspects VistaInterProcComm FIND_DEPENDENCIES )
vista_use_package( VistaMessagingService REQUIRED )

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_executable( Receiver ${ProjectSources} )
target_link_libraries( Receiver
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

vista_configure_app( Receiver )
vista_create_default_info_file( Receiver )

