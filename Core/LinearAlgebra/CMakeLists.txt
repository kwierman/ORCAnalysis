#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCALinearAlgebra SHARED ${module_sources} ${module_headers})
set_target_properties(ORCALinearAlgebra PROPERTIES LINKER_LANGUAGE CXX )
target_link_libraries(ORCALinearAlgebra ORCAAllocation ORCAThreads ORCASingleton)

install(TARGETS ORCALinearAlgebra DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/LinearAlgebra)

