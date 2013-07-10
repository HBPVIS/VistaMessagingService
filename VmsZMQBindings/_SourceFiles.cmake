# $Id$

set( RelativeDir "." )
set( RelativeSourceGroup "Source Files" )

set( DirFiles
	VmsZMQConfig.h
	VmsZMQSocketCoreFactory.cpp
	VmsZMQSocketCoreFactory.h
	VmsZMQSocketCore.cpp
	VmsZMQSocketCore.h
	VmsZMQSocketDemux.cpp
	VmsZMQSocketDemux.h
	_SourceFiles.cmake
)
set( DirFiles_SourceGroup "${RelativeSourceGroup}" )

set( LocalSourceGroupFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalSourceGroupFiles "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${DirFiles_SourceGroup} FILES ${LocalSourceGroupFiles} )

