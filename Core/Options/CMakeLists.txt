#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(DeuteriumOptions SHARED ${module_sources} ${module_headers})
target_link_libraries(DeuteriumOptions ProtiumThreads)
set_target_properties(DeuteriumOptions PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS DeuteriumOptions DESTINATION lib EXPORT DeuteriumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Deuterium/Options)

