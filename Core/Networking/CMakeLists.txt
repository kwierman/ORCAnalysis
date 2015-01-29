#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(DeuteriumNetworking SHARED ${module_sources} ${module_headers})
target_link_libraries(DeuteriumNetworking ProtiumThreads)
set_target_properties(DeuteriumNetworking PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS DeuteriumNetworking DESTINATION lib EXPORT DeuteriumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Deuterium/Networking)

