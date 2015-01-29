#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(DeuteriumAll SHARED ${module_sources} ${module_headers})
target_link_libraries(DeuteriumAll )
set_target_properties(DeuteriumAll PROPERTIES LINKER_LANGUAGE CXX)


#Install Headers
install(TARGETS DeuteriumAll DESTINATION lib EXPORT DeuteriumConfig)
install(FILES ${module_headers} DESTINATION include)

