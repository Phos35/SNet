# 版本要求
cmake_minimum_required(VERSION 3.12)

# 测试项目
PROJECT(TEST)

# 添加头文件目录
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/alias)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/buffer)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/config)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/io)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/log)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/utils/time)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/third/jsoncpp)

# 添加源文件
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/alias SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/buffer SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/config SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/io SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/log SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/time SRC_LIST)

# 添加main函数源文件
ADD_EXECUTABLE(TEST
    /home/ubuntu/projects/web_server/SNet/tests/utils/log/test_logfile.cpp
    ${SRC_LIST}
)