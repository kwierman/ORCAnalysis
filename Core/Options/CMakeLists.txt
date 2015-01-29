#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCAOptions SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCAOptions ProtiumThreads)
set_target_properties(ORCAOptions PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCAOptions DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Options)

