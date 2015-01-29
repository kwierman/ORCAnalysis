#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.h)

#Make the library
add_library(ORCATime SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCATime ORCAAllocation)
set_target_properties(ORCATime PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCATime DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Time)

