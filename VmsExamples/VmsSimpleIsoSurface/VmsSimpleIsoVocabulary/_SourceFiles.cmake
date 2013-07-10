# $Id$

set( RelativeDir "." )
set( RelativeSourceGroup "Source Files" )

set( DirFiles
	RegisterSimpleIsoVoc.cpp
	RegisterSimpleIsoVoc.h
	RequestIsosurfaceMsg.cpp
	RequestIsosurfaceMsg.h
	SimpleIsoVocConfig.h
	TerminateMsg.cpp
	TerminateMsg.h
	UpdatePolyDataMsg.cpp
	UpdatePolyDataMsg.h
	_SourceFiles.cmake
)
set( DirFiles_SourceGroup "${RelativeSourceGroup}" )

set( LocalSourceGroupFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalSourceGroupFiles "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${DirFiles_SourceGroup} FILES ${LocalSourceGroupFiles} )

