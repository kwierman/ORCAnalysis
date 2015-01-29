#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumAllocation SHARED ${module_sources} ${module_headers})
target_link_libraries(ProtiumAllocation ProtiumDesign ProtiumThreads ProtiumSingleton ProtiumUtilities)
set_target_properties(ProtiumAllocation PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ProtiumAllocation DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/Allocation)

