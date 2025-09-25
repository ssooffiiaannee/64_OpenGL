cmake_minimum_required(VERSION 3.20)

if(WIN32)
    # set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")
elseif(UNIX)
    # set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")
endif()

project(EMSim)

if(WIN32)
    set(CMAKE_CXX_STANDARD 17)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ") #/MD /NODEFAULTLIB
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLEW -lglfw")
endif()

# set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(VULKAN_INC_PATH D:/VulkanSDK/1.3.296.0/Include)
set(VULKAN_LIB_PATH D:/VulkanSDK/1.3.296.0/Lib)

set(GLFW_INC_PATH D:/3D_routine/deps/glfw-3.3.9.bin.WIN64/glfw-3.3.9.bin.WIN64/include)
set(GLFW_LIB_PATH D:/3D_routine/deps/glfw-3.3.9.bin.WIN64/glfw-3.3.9.bin.WIN64/lib-vc2022)

set(GLEW_INC_PATH D:/3D_routine/deps/glew-2.1.0/include)
set(GLEW_LIB_PATH D:/3D_routine/deps/glew-2.1.0/lib/Release/x64)

set(OPENGL_PATH "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64")


set(EMSIM_INSTALL_INCLUDE_DIR ${GLEW_INC_PATH} ${GLFW_INC_PATH} ${VULKAN_INC_PATH})
set(EMSIM_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(EMSIM_DIR ${PROJECT_SOURCE_DIR})
# set(EMSIM_SRC_DIR ${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/src)



include_directories(${EMSIM_INSTALL_INCLUDE_DIR})

# source files

# Find the library
# find_library(Vulkan_lib vulkan-1.lib PATHS ${VULKAN_LIB_PATH})
find_library(GLFW_LIB glfw3 PATHS ${GLFW_LIB_PATH})
find_library(GLEW_LIB glew32 PATHS ${GLEW_LIB_PATH})
find_library(OpenGL  opengl32  PATHS ${OPENGL_PATH})
# find_library(GL GL )

set(SRC_FILES ${EMSIM_DIR}/main.cpp)


add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
# set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${GLEW_LIB} ${GLFW_LIB} ${OpenGL})

# add_subdirectory(include)
# add_subdirectory(thirdparty)
