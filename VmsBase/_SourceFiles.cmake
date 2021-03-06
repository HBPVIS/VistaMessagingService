# $Id$

set( RelativeDir "." )
set( RelativeSourceGroup "Source Files" )

set( DirFiles
	VmsAnswerRequestSocket.cpp
	VmsAnswerRequestSocket.h
	VmsBaseConfig.h
	VmsMarshallingCodec.cpp
	VmsMarshallingCodec.h
	VmsMsgCodec.h
	VmsReceiveSocket.cpp
	VmsReceiveSocket.h
	VmsSerializingCodec.h
	VmsSendRequestSocket.cpp
	VmsSendRequestSocket.h
	VmsSendSocket.cpp
	VmsSendSocket.h
	VmsSocketCoreFactory.cpp
	VmsSocketCoreFactory.h
	VmsSocketCore.cpp
	VmsSocketCore.h
	VmsSocketDemux.cpp
	VmsSocketDemux.h
	VmsSocketFactory.cpp
	VmsSocketFactory.h
	VmsTypedReceiveSocket.h
	VmsTypedSendSocket.h
	VmsVocabulary.cpp
	VmsVocabulary.h
	_SourceFiles.cmake
)
set( DirFiles_SourceGroup "${RelativeSourceGroup}" )

set( LocalSourceGroupFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalSourceGroupFiles "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${DirFiles_SourceGroup} FILES ${LocalSourceGroupFiles} )

