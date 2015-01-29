#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCAFileSystem SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCAFileSystem ORCAThreads)
set_target_properties(ORCAFileSystem PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCAFileSystem DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/FileSystem)

