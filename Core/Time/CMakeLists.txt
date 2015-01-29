#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.h)

#Make the library
add_library(ProtiumTime SHARED ${module_sources} ${module_headers})
target_link_libraries(ProtiumTime ProtiumAllocation)
set_target_properties(ProtiumTime PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ProtiumTime DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/Time)

