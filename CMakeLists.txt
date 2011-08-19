# $Id:$

cmake_minimum_required( VERSION 2.8 )
project( VistaMessagingService )

list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )

#make vista specific cmake magic available
include( VistaCommon )

# right now, the VMS requires the zeromq message oriented middleware
vista_use_package( ZMQ REQUIRED )
vista_use_package( VistaCoreLibs REQUIRED VistaBase VistaTools VistaMath VistaAspects VistaInterProcComm FIND_DEPENDENCIES )

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_library( VistaMessagingService ${ProjectSources} )
target_link_libraries( VistaMessagingService
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

#add doxygen target for convenient build
vista_create_doxygen_target( "${CMAKE_CURRENT_SOURCE_DIR}/documentation/Doxyfile" )

#finalize configuration
vista_configure_lib( VistaMessagingService )
vista_install( VistaMessagingService )
vista_create_cmake_configs( VistaMessagingService )
