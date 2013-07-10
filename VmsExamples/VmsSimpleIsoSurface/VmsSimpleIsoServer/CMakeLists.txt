# $Id: CMakeLists.txt 35799 2013-06-25 10:30:03Z bh736275 $

cmake_minimum_required( VERSION 2.8 )

if( NOT VMSSIMPLEISO_COMMON_BUILD )
	project( VmsSimpleIsoServer )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif(NOT VMSSIMPLEISO_COMMON_BUILD)

vista_use_package( VmsSimpleIsoVocabulary REQUIRED FIND_DEPENDENCIES ) 
vista_use_package( VmsBase REQUIRED FIND_DEPENDENCIES ) 
vista_use_package( VmsZMQBindings REQUIRED FIND_DEPENDENCIES ) 

vista_use_package( VistaVisExt "HEAD" REQUIRED FIND_DEPENDENCIES )
vista_use_package( VistaCoreLibs REQUIRED VistaBase VistaAspects VistaTools VistaInterProcComm FIND_DEPENDENCIES) 

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_executable( VmsSimpleIsoServer ${ProjectSources} )
target_link_libraries( VmsSimpleIsoServer
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

vista_configure_app( VmsSimpleIsoServer )
vista_create_default_info_file( VmsSimpleIsoServer )

SET_TARGET_PROPERTIES(VmsSimpleIsoServer PROPERTIES FOLDER "Examples/VmsSimpleIso")