cmake_minimum_required(VERSION 3.20)
# proj
project(aSoundAgent
    VERSION 0.1.0
    DESCRIPTION "audio dsp application"
    ) # project
set(vProjName ${PROJECT_NAME})
set(vProjVnum ${PROJECT_VERSION})
# lang
set(vLangVnum 20)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# file
set(vFileDirLoc ${PROJECT_SOURCE_DIR}/..)
set(vFileDirSrc ${vFileDirLoc}/src)
set(vFileDirGen ${vFileDirLoc}/gen)
set(vFileDirLib ${vFileDirLoc}/lib)
# libs
add_subdirectory(
    ${vFileDirLib}/portaudio
    ${vFileDirGen}/CMakeFiles/portaudio.dir
    ) # add_subdirectory
# make
add_executable(${vProjName}
    ${vFileDirSrc}/main.cxx
    ) # add_executable
target_compile_options(${vProjName} PRIVATE
    -Wno-format
    ) # target_compile_options
target_link_libraries(${vProjName}
    PortAudio
    ) # target_link_libraries
