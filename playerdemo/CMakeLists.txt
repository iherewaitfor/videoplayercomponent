#Cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project (playerdemo)

# 查找目录下的所有源文件
# 并将名称保存到DIR_SRCS变量
aux_source_directory(. DIR_SRCS)
include_directories(include)

# 设置子系统为窗口程序
set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS" ) 

#项目使用UNICODE版本。win中的各种API将使用UNICODE版本。
add_definitions("-DUNICODE" "-D_UNICODE")


# 指定生成目标
add_executable(playerdemo ${DIR_SRCS})

# 添加链接库
target_link_libraries(playerdemo videoplayercomponent)

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avcodec-55.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avformat-55.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )


add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avutil-52.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/swresample-0.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/swscale-2.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )


add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/allin.mp4 ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/audiogift.mp4 ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )
add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/out.mp4 ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )
  
add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/../videoplayercomponent/$<CONFIGURATION>/videoplayercomponent.dll ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
  )


add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avcodec-55.dll ${PROJECT_BINARY_DIR}
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avformat-55.dll ${PROJECT_BINARY_DIR}
  )


add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/avutil-52.dll ${PROJECT_BINARY_DIR}
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/swresample-0.dll ${PROJECT_BINARY_DIR}
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/swscale-2.dll ${PROJECT_BINARY_DIR}
  )


add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/allin.mp4 ${PROJECT_BINARY_DIR}
  )

add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/audiogift.mp4 ${PROJECT_BINARY_DIR}
  )
add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/out.mp4 ${PROJECT_BINARY_DIR}
  )
  
add_custom_command(TARGET playerdemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/../videoplayercomponent/$<CONFIGURATION>/videoplayercomponent.dll ${PROJECT_BINARY_DIR}
  )


