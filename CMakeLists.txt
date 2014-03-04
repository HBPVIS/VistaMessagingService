# $Id: CMakeLists.txt 35308 2013-05-29 09:35:00Z bh736275 $
cmake_minimum_required( VERSION 2.8 )

project( VistaMessagingService )

ENABLE_TESTING()

include_directories( ${VflMessaging_SOURCE_DIR} )

list( APPEND CMAKE_MODULE_PATH "$ENV{VISTA_CMAKE_COMMON}" )
include( VistaCommon )

vista_set_version( VistaMessagingService HEAD "HEAD" svn_rev )

# this variable indicates to sub-projects that they are build all together
set( VMS_COMMON_BUILD TRUE )

vista_conditional_add_subdirectory( VMS_BUILD_BASE_LIB	VmsBase ON )
vista_conditional_add_subdirectory( VMS_BUILD_ZMQ_LIB   VmsZMQBindings ON )
vista_conditional_add_subdirectory( VMS_BUILD_TESTS 	VmsTesting OFF )
vista_conditional_add_subdirectory( VMS_BUILD_EXAMPLES 	VmsExamples OFF )

