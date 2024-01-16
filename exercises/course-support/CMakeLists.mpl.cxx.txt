cmake_minimum_required( VERSION 3.12 )
project( ${PROJECT_NAME} VERSION 1.0 )

find_package( mpl REQUIRED )

add_executable( ${PROJECT_NAME} ${PROJECT_NAME}.cxx )
target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )
target_include_directories(
	${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	mpl::mpl )
target_link_libraries(
	${PROJECT_NAME} PUBLIC 
	mpl::mpl )

install( TARGETS ${PROJECT_NAME} DESTINATION . )
