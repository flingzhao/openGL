cmake_minimum_required(VERSION 3.22)

set (CMAKE_C_COMPILER "D:/mingw64/w64devkit/bin/gcc.exe")
set (CMAKE_CXX_COMPILER "D:/mingw64/w64devkit/bin/g++.exe")
set(CMAKE_BUILD_TYPE Debug)
project(openGL)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(ASSIMP_LIB
    basic
    model_explode
    model_normal_vector
    skybox_env_mapping
    asteroid
    asteroid_instanced
    )

set(CHAPTERS 
    basic
    advanced_opengl
    advanced_lighting
    )

set(basic basic
          model_explode
          model_normal_vector
          )

set(advanced_opengl 
    stencil_testing
    blend
    face_culling
    framebuffer
    rear_view_mirror
    skybox
    skybox_env_mapping
    gl_pointsize
    gl_frontfacing
    uniform_buffer_obj
    geometry_shader_houses
    instancing_quad
    asteroid
    asteroid_instanced
    msaa
    msaa_offscreen
    )

set(advanced_lighting
    advanced_lighting
    gamma_correction
    shadow_mapping_depth
    shadow_mapping_base
    point_shadow
    normal_mapping
    5.1.parallex_mapping
    5.2.parallex_mapping
    5.3.parallex_mapping
    hdr
    )
file(GLOB DLLS "Lib/*")

link_directories(${CMAKE_SOURCE_DIR}/Lib)

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

include_directories("./vendor/include")
include_directories("./vendor/include/glm")
include_directories("./src")

function(create_project_from_src chapter demo)
    list(FIND ASSIMP_LIB ${demo} target_index)
    if(target_index GREATER -1)
        set(NAME "${chapter}__${demo}")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/${demo}")
        file(GLOB SOURCE
        "src/Shader.cpp"
        "src/Camera.cpp"
        "src/util.cpp"
        "src/stb_image.cpp"
        "src/Model.cpp"
        "src/Mesh.cpp"
        "src/glad.c"
        "src/${chapter}/*.h"
        "src/${chapter}/*.cpp"
        "src/${chapter}/${demo}/*.h"
        "src/${chapter}/${demo}/*.cpp"
        ) 
	else()
        set(NAME "${chapter}__${demo}")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/${demo}")
        file(GLOB SOURCE
        "src/Shader.cpp"
        "src/Camera.cpp"
        "src/util.cpp"
        "src/stb_image.cpp"
        "src/glad.c"
        "src/${chapter}/*.h"
        "src/${chapter}/*.cpp"
        "src/${chapter}/${demo}/*.h"
        "src/${chapter}/${demo}/*.cpp"
        ) 
    endif()
        
    if(target_index GREATER -1)
        message("${demo} 需要assimp library")
        file(COPY ${DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")
    endif()    
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} PUBLIC
            glfw3
            openGL32
            )  
    if(target_index GREATER -1)
        target_link_libraries(${NAME} PUBLIC
            assimp
            )
        file(COPY ${DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")
    endif()  
endfunction(create_project_from_src)

foreach(CHAPTER ${CHAPTERS})
    foreach(demo ${${CHAPTER}})
        create_project_from_src(${CHAPTER} ${demo}) 
    endforeach(demo)
endforeach(CHAPTER)


# add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     ${DLLS}
#     "${CMAKE_CURRENT_SOURCE_DIR}/bin"
#     )
