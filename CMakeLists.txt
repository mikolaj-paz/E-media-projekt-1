cmake_minimum_required( VERSION 3.28 )
project( "Format PNG - projekt" )

set(SFML_BUILD_EXAMPLES FALSE)
set(SFML_BUILD_DOC FALSE)
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory( "lib" )

add_executable( project1 
    "src/helper.cpp"
    "src/anonymize.cpp"
    "src/critical_chunks.cpp"
    "src/ancillary_chunks.cpp"
    "src/rsa.cpp"
    "src/png.cpp"
    "src/project1.cpp"
)
    
add_executable( project2
    "src/helper.cpp"
    "src/critical_chunks.cpp"
    "src/ancillary_chunks.cpp"
    "src/compression.cpp"
    "src/rsa.cpp"
    "src/cryptosystem.cpp"
    "src/png.cpp"
    "src/project2.cpp"
)

set_target_properties(project1 PROPERTIES LINK_FLAGS -Wl,--stack,10000000000)
target_include_directories( project1 PUBLIC "include" "lib/fftw/api" )
target_link_libraries( project1 sfml-graphics fftw3 cryptopp )

set_target_properties(project2 PROPERTIES LINK_FLAGS -Wl,--stack,10000000000)
target_include_directories( project2 PUBLIC "include" )
target_link_libraries( project2 sfml-graphics cryptopp )