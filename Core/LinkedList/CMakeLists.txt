#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCALinkedList SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCALinkedList ORCADesign ORCAThreads )
set_target_properties(ORCALinkedList PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCALinkedList DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/LinkedList)

