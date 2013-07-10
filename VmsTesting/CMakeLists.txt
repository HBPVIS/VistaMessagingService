# $Id: CMakeLists.txt 36032 2013-07-02 16:06:30Z bh736275 $
include( VistaTestingUtils )

#remember to configure the lib under test as well!
vista_use_package( VmsBase REQUIRED FIND_DEPENDENCIES)
vista_use_package( VmsZMQBindings REQUIRED FIND_DEPENDENCIES)
vista_use_package( ZMQ REQUIRED FIND_DEPENDENCIES)
vista_use_package( GTest REQUIRED )

####
# configure test for the message vocabulary
####
vista_configure_test( VocabularyTest VocabularyTest.cpp)
target_link_libraries( VocabularyTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( VocabularyTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase )
###

####
# configure test for the socket core factory
####
vista_configure_test( SocketCoreFactoryTest SocketCoreFactoryTest.cpp)
target_link_libraries( SocketCoreFactoryTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( SocketCoreFactoryTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###

####
# configure test for the socket factory
####
vista_configure_test( SocketFactoryTest SocketFactoryTest.cpp)
target_link_libraries( SocketFactoryTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( SocketFactoryTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###



####
# configure test for the socket core
####
vista_configure_test( SocketCoreTest SocketCoreTest.cpp TestingUtils.h)
target_link_libraries( SocketCoreTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( SocketCoreTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###

####
# configure test for the request socket pair
####
vista_configure_test( RequestSocketTest RequestSocketTest.cpp TestingUtils.h)
target_link_libraries( RequestSocketTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( RequestSocketTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###

####
# configure test for the fire-and-forget socket pair
####
vista_configure_test( SendReceiveSocketTest SendReceiveSocketTest.cpp TestingUtils.h)
target_link_libraries( SendReceiveSocketTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( SendReceiveSocketTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###

####
# configure test for the socket demultiplexer
####
vista_configure_test( SocketDemuxTest SocketDemuxTest.cpp TestingUtils.h)
target_link_libraries( SocketDemuxTest
	${VISTA_USE_PACKAGE_LIBRARIES}
)
add_dependencies( SocketDemuxTest VistaBase VistaAspects VistaTools VistaInterProcComm VmsBase VmsZMQBindings)
###
