#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumThreads SHARED ${module_sources} ${module_headers})
target_link_libraries(ProtiumThreads ProtiumDesign)
install(TARGETS ProtiumThreads DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/Threads)

