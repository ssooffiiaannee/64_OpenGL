cmake_minimum_required(VERSION 3.20)

if(WIN32)
    # cmake -S . -B build -G "MinGW Makefiles"
    # set(CMAKE_CXX_COMPILER "C:/ProgramData/mingw64/mingw64/bin/g++.exe")
    set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")
elseif(UNIX)
    # set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")
endif()

project(EMSim)

if(WIN32)
    # set(CMAKE_CXX_STANDARD 17)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lX11")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
message("message : CMAKE_INSTALL_PREFIX  ", ${CMAKE_INSTALL_PREFIX})
message("message : PROJECT_SOURCE_DIR  ", ${PROJECT_SOURCE_DIR})
message("message : CMAKE_INSTALL_PREFIX  ", ${CMAKE_INSTALL_PREFIX})

set(EMSIM_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/include)
set(EMSIM_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(EMSIM_DIR ${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME})
set(EMSIM_SRC_DIR ${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/src)



include_directories(${EMSIM_INSTALL_INCLUDE_DIR})

# source files

# Find the library
find_library(MYLIBRARY X11 PATHS /usr/lib /usr/local/lib)

if(WIN32)
    set(SRC_FILES ${EMSIM_DIR}/win_entrypoint.c)
    set(SRC_FILES ${SRC_FILES} ${EMSIM_SRC_DIR}/wms/wms_win32.c)
elseif(UNIX)
    
    set(SRC_FILES ${SRC_FILES} ${EMSIM_DIR}/unix_entrypoint.c)
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${MYLIBRARY})

add_subdirectory(include)
add_subdirectory(thirdparty)
