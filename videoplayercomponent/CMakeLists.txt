#Cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project (videoplayercomponent)

# 查找目录下的所有源文件
aux_source_directory(. DIR_LIB_SRCS)
include_directories(include)

# 设置子系统为窗口程序
set ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SUBSYSTEM:WINDOWS" ) 

#项目使用UNICODE版本。win中的各种API将使用UNICODE版本。
add_definitions("-DUNICODE" "-D_UNICODE")


add_definitions("-DVIDEOPLAYERCOMPONENT_EXPORTS" "-D_VIDEOPLAYERCOMPONENT_EXPORTS")

# 指定生成MathFunctions链接库
add_library(videoplayercomponent SHARED ${DIR_LIB_SRCS})