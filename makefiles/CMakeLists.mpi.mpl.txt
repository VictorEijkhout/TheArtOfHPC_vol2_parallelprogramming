cmake_minimum_required( VERSION 3.12 )
project( ${PROJECTNAME} VERSION 1.0 )

find_package( mpl REQUIRED )

add_executable( ${PROJECTNAME} ${PROJECTNAME}.cxx )
target_compile_features( ${PROJECTNAME} PRIVATE cxx_std_20 )
target_include_directories(
	${PROJECTNAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	mpl::mpl )
target_link_libraries(
	${PROJECTNAME} PUBLIC 
	mpl::mpl )

install( TARGETS ${PROJECTNAME} DESTINATION . )
