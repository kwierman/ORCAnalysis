#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCASingleton SHARED ${module_sources} ${module_headers})
set_target_properties(ORCASingleton PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ORCASingleton ORCAThreads)
install(TARGETS ORCASingleton DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Singleton)

