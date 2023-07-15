cmake_minimum_required(VERSION 3.20)
# proj
project(aSoundAgent
    VERSION 0.1.0
    DESCRIPTION "audio dsp application"
    ) # project
# lang
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
# libs
add_subdirectory(lib/portaudio)
# make
add_executable(aSoundAgent src/main.cxx)
target_compile_options(aSoundAgent PRIVATE -Wno-format)
target_link_libraries(aSoundAgent PortAudio)
