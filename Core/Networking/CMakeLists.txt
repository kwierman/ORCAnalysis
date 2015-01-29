#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCANetworking SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCANetworking ProtiumThreads)
set_target_properties(ORCANetworking PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCANetworking DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Networking)

