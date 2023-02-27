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
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/event)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/socket)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/tcp)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/message_processor/decoder)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/message_processor/message)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/message_processor/dispatcher)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/message_processor)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/http)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/http/message_processor)
INCLUDE_DIRECTORIES(/home/ubuntu/projects/web_server/SNet/net/http/message_processor/message)

# 添加源文件
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/alias SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/buffer SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/config SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/io SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/log SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/utils/time SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/event SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/socket SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/tcp SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/message_processor/dispatcher SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/message_processor/decoder SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/message_processor/message SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/message_processor SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/http SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/http/message_processor SRC_LIST)
AUX_SOURCE_DIRECTORY(/home/ubuntu/projects/web_server/SNet/net/http/message_processor/message SRC_LIST)

# 添加编译选项
ADD_COMPILE_OPTIONS(-g)
ADD_COMPILE_OPTIONS(-lpthread)

# 添加main函数源文件
ADD_EXECUTABLE(TEST
/home/ubuntu/projects/web_server/SNet/tests/http/test_http_server.cpp
    ${SRC_LIST}
)

# 添加链接库
TARGET_LINK_LIBRARIES(TEST pthread)