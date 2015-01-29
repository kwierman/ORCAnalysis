#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCAUtilities SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCAUtilities  ORCASingleton)
set_target_properties(ORCAUtilities PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCAUtilities DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Allocation)

