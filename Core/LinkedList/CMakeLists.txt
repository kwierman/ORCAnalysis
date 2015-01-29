#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumLinkedList SHARED ${module_sources} ${module_headers})
target_link_libraries(ProtiumLinkedList ProtiumDesign ProtiumThreads )
set_target_properties(ProtiumLinkedList PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ProtiumLinkedList DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/LinkedList)

