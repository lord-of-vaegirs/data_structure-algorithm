cmake_minimum_required(VERSION 3.14)
project(data_structure_algorithm)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Linux x86特定配置
if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -mcmodel=large")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcmodel=large")
endif()

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加可执行文件
add_executable(${PROJECT_NAME} src/blocktrade.cpp)

# 使用header-only版本的csv-parser
target_include_directories(${PROJECT_NAME} PRIVATE external/csv-parser/single_include)