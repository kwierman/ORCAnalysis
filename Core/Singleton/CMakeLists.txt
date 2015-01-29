#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumSingleton SHARED ${module_sources} ${module_headers})
set_target_properties(ProtiumSingleton PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ProtiumSingleton ProtiumThreads)
install(TARGETS ProtiumSingleton DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/Singleton)

