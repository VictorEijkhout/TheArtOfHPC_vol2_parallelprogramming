cmake_minimum_required( VERSION 3.20 )
project( ${PROJECT_NAME} VERSION 1.0 )

find_package(OpenMP)
if(OpenMP_C_FOUND)
else()
	message( FATAL_ERROR "Could not find OpenMP" )
endif()

add_executable( ${PROJECT_NAME} ${PROJECT_NAME}.c )
target_compile_features( ${PROJECT_NAME} PRIVATE c_std_99 )
target_link_libraries( ${PROJECT_NAME} PUBLIC OpenMP::OpenMP_C )

install( TARGETS ${PROJECT_NAME} DESTINATION . )
