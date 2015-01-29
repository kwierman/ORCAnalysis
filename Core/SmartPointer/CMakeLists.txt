#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCASmartPointer SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCASmartPointer ORCADesign ORCAThreads  ORCALinkedList ORCAAllocation)
set_target_properties(ORCASmartPointer PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ORCASmartPointer DESTINATION lib EXPORT ORCAConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/ORCA/SmartPointer)

