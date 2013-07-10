# $Id: CMakeLists.txt 35308 2013-05-29 09:35:00Z bh736275 $

cmake_minimum_required( VERSION 2.8 )
if( NOT VMS_COMMON_BUILD )
	project( VmsZMQBindings )
	list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
	include( VistaCommon )
endif( NOT VMS_COMMON_BUILD )

vista_use_package( VmsBase REQUIRED FIND_DEPENDENCIES ) 
vista_use_package( ZMQ REQUIRED)

# Including the source files of all source subfolders recursively
include( "./_SourceFiles.cmake" )

add_library( VmsZMQBindings ${ProjectSources} )
#The following line prevent CMake from adding all depencies to other projects that link it from within the same cmake build
set_property( TARGET VmsZMQBindings PROPERTY LINK_INTERFACE_LIBRARIES "" )
target_link_libraries( VmsZMQBindings
	${VISTA_USE_PACKAGE_LIBRARIES} # contains all libraries from vista_use_package() calls
)

vista_configure_lib( VmsZMQBindings )
if( VMS_COMMON_BUILD )
	vista_install( VmsZMQBindings VmsZMQBindings )
else( VMS_COMMON_BUILD )
	vista_install( VmsZMQBindings )
endif( VMS_COMMON_BUILD )
vista_create_cmake_configs( VmsZMQBindings )
vista_create_default_info_file( VmsZMQBindings )

