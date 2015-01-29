#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumSmartPointer SHARED ${module_sources} ${module_headers})
target_link_libraries(ProtiumSmartPointer ProtiumDesign ProtiumThreads  ProtiumLinkedList ProtiumAllocation)
set_target_properties(ProtiumSmartPointer PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ProtiumSmartPointer DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/SmartPointer)

