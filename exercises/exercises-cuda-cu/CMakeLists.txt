cmake_minimum_required( VERSION 3.12 )
project( ${PROJECTNAME} VERSION 1.0 LANGUAGES C CXX CUDA )

add_executable( ${PROJECTNAME} )
target_sources( ${PROJECTNAME} PRIVATE ${PROJECTNAME}.cu )

set_target_properties( ${PROJECTNAME}
  PROPERTIES CUDA_ARCHITECTURES "all" )

install( TARGETS ${PROJECTNAME} DESTINATION . )
