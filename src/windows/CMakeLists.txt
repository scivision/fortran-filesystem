configure_file(crt.f90.inc crt.f90 COPYONLY)
configure_file(path.f90.inc path.f90 COPYONLY)
configure_file(sys.f90.inc sys.f90 COPYONLY)

target_sources(pathlib PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/crt.f90 ${CMAKE_CURRENT_BINARY_DIR}/path.f90 ${CMAKE_CURRENT_BINARY_DIR}/sys.f90)
