#Get together the sources
file(GLOB module_sources *.cc)
file(GLOB module_headers *.hh)

#Make the library
add_library(ProtiumContainers SHARED ${module_sources} ${module_headers})
set_target_properties(ProtiumContainers PROPERTIES LINKER_LANGUAGE CXX )
target_link_libraries(ProtiumContainers ProtiumAllocation )
#For suppressing the warning regarding the bit shift flag
#set_property( TARGET main APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
install(TARGETS ProtiumContainers DESTINATION lib EXPORT ProtiumConfig)

#Install Headers
install(FILES ${module_headers} DESTINATION include/Protium/Collections)

