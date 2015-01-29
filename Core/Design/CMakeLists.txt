#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCADesign SHARED ${module_sources} ${module_headers})
set_target_properties(ORCADesign PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCADesign DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/Design)
