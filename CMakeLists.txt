cmake_minimum_required(VERSION 3.22)

project(openGL)
include_directories("./vendor/include")
include_directories("./vendor/include/glm")


link_directories("./Lib")

file(GLOB_RECURSE
    src_dir
    "./src/*.cpp"
    "./src/*.c"
    "./src/*.h")

add_executable(${PROJECT_NAME} ${src_dir})

target_link_libraries(${PROJECT_NAME} 
    glfw3
    openGL32
    libassimp-5.dll
    )  