#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(DeuteriumDataFormat SHARED ${module_sources} ${module_headers})
target_link_libraries(DeuteriumDataFormat ProtiumThreads ProtiumAllocation)
set_target_properties(DeuteriumDataFormat PROPERTIES LINKER_LANGUAGE CXX)


#Install Headers and CMake Config
install(TARGETS DeuteriumDataFormat DESTINATION lib EXPORT DeuteriumConfig)
install(FILES ${module_headers} DESTINATION include/Deuterium/DataFormat)

