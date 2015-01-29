#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCAThreads SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCAThreads ORCADesign)
install(TARGETS ORCAThreads DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Threads)

