#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(DeuteriumFileSystem SHARED ${module_sources} ${module_headers})
target_link_libraries(DeuteriumFileSystem ProtiumThreads)
set_target_properties(DeuteriumFileSystem PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS DeuteriumFileSystem DESTINATION lib EXPORT DeuteriumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Deuterium/FileSystem)

