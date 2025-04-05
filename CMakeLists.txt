cmake_minimum_required( VERSION 3.28 )
project( "Format PNG - projekt" )

set(SFML_BUILD_EXAMPLES FALSE)
set(SFML_BUILD_DOC FALSE)
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory( "lib/SFML" )
add_subdirectory( "lib/fftw" )

add_executable( a 
    "src/helper.cpp"
    "src/critical_chunks.cpp"
    "src/ancillary_chunks.cpp"
    "src/png.cpp"
    "src/main.cpp"
)

set_target_properties(a PROPERTIES LINK_FLAGS -Wl,--stack,10000000)
target_include_directories( a PUBLIC "include" "lib/fftw/api" )
target_link_libraries( a sfml-graphics fftw3 )