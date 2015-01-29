#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ORCADataFormat SHARED ${module_sources} ${module_headers})
target_link_libraries(ORCADataFormat ProtiumAllocation)
set_target_properties(ORCADataFormat PROPERTIES LINKER_LANGUAGE CXX)


#Install Headers and CMake Config
install(TARGETS ORCADataFormat DESTINATION lib EXPORT ORCAConfig)
install(FILES ${module_headers} DESTINATION include/ORCA/DataFormat)

