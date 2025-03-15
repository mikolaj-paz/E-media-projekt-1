cmake_minimum_required( VERSION 3.28 )
project( "Format PNG - projekt" )

add_executable( a 
    "src/msb_to_lsb.cpp"
    "src/critical_chunks.cpp"
    "src/ancillary_chunks.cpp"
    "src/png.cpp"
    "src/main.cpp"
)

target_include_directories( a PUBLIC "include")