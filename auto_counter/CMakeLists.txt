CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(auto_counter)

FIND_PACKAGE(OpenCV REQUIRED)

FILE(GLOB SRCS ABSOLUTE ${PROJECT_SOURCE_DIR} *.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
