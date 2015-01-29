#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCAAllocation SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCAAllocation ORCADesign ORCAThreads ORCASingleton ORCAUtilities)
set_target_properties(ORCAAllocation PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCAAllocation DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Allocation)

