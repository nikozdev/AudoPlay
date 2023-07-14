cmake_minimum_required(VERSION 3.20)
# proj
project(aSoundAgent
    VERSION 0.0.0.0
    ) # project
set(vProjName ${PROJECT_NAME})
set(vProjVnum ${PROJECT_VERSION})
# file
set(vFileDirLoc ${PROJECT_SOURCE_DIR})
set(vFileDirSrc ${vFileDirLoc}/src)
# make
add_executable(${vProjName}) # add_executable
target_sources(
    ${vFileDirSrc}/main.cxx
    ) # target_sources
