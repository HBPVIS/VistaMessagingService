# $Id: CMakeLists.txt 35420 2013-06-05 09:03:56Z bh736275 $

cmake_minimum_required( VERSION 2.8 )

if( NOT VMSSIMPLEISO_COMMON_BUILD )
	project( VmsSimpleIsoVocabulary )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VMSSIMPLEISO_COMMON_BUILD )

vista_use_package( VmsBase REQUIRED FIND_DEPENDENCIES ) 
vista_use_package( VistaVisExt "HEAD" REQUIRED FIND_DEPENDENCIES )
vista_use_package( VistaCoreLibs REQUIRED VistaBase VistaAspects VistaTools VistaInterProcComm FIND_DEPENDENCIES) 

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_library( VmsSimpleIsoVocabulary ${ProjectSources} )
target_link_libraries( VmsSimpleIsoVocabulary
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

vista_configure_lib( VmsSimpleIsoVocabulary )
if( VMSSIMPLEISO_COMMON_BUILD )
	vista_install( VmsSimpleIsoVocabulary VmsSimpleIsoVocabulary )
else( VMSSIMPLEISO_COMMON_BUILD )
	vista_install( VmsSimpleIsoVocabulary )
endif( VMSSIMPLEISO_COMMON_BUILD )
vista_create_cmake_configs( VmsSimpleIsoVocabulary )
vista_create_default_info_file( VmsSimpleIsoVocabulary )

SET_TARGET_PROPERTIES(VmsSimpleIsoVocabulary PROPERTIES FOLDER "Examples/VmsSimpleIso")


