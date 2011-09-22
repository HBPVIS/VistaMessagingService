# $Id:$

set( RelativeDir "." )
set( RelativeSourceGroup "Source Files" )

set( DirFiles
	VmsConfig.h
	VmsEndpointFactory.cpp
	VmsEndpointFactory.h
	VmsInPlaceMsgCodec.cpp
	VmsInPlaceMsgCodec.h
	VmsMsg.cpp
	VmsMsg.h
	VmsMsgCodec.cpp
	VmsMsgCodec.h
	VmsMsgFactory.cpp
	VmsMsgFactory.h
	VmsMsgReceiver.cpp
	VmsMsgReceiver.h
	VmsMsgSender.cpp
	VmsMsgSender.h
	VmsMsgReceptor.h
	VmsMsgReceptor.cpp
	VmsMsgHandler.h
	VmsMsgHandler.cpp
	VmsSerialMsgCodec.cpp
	VmsSerialMsgCodec.h
	VmsStringMsg.cpp
	VmsStringMsg.h
	VmsConfig.h
	_SourceFiles.cmake
)
set( DirFiles_SourceGroup "${RelativeSourceGroup}" )

set( LocalSourceGroupFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalSourceGroupFiles "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${DirFiles_SourceGroup} FILES ${LocalSourceGroupFiles} )

set( SubDirFiles "" )
foreach( Dir ${SubDirs} )
	list( APPEND SubDirFiles "${RelativeDir}/${Dir}/_SourceFiles.cmake" )
endforeach()

foreach( SubDirFile ${SubDirFiles} )
	include( ${SubDirFile} )
endforeach()

