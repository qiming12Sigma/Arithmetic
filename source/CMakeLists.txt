#指定cmake的最小版本号
CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

#指定项目名称
PROJECT(Arithmetic LANGUAGES CXX)

#指定Qt路径和启用当前目录(按需设置)
SET(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#引入Qt库
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)

#添加预编译宏
ADD_DEFINITIONS(-DTORRENT_LIBRARY)
ADD_DEFINITIONS(-DNO_FREETYPE)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
ADD_DEFINITIONS(-DHCPP_LEGACY_API)

#将各种文件归类到指定变量
FILE(GLOB_RECURSE HEADER_FILES "*.h")
FILE(GLOB_RECURSE SRC_FILES "*.cpp")
FILE(GLOB_RECURSE FORMS "*.ui")
FILE(GLOB_RECURSE RESOURCE_FILES "*.qrc")
FILE(GLOB TS_FILES "*.ts")
FILE(GLOB MOC_FILES "moc_*.cpp")

#将ui文件编译成ui_*.h文件
IF(FORMS)
	QT5_WRAP_UI(FORMS_UIC ${FORMS})
ENDIF()

#将包含Q_OBJECT的头文件编译成moc_*.cpp文件
QT5_WRAP_CPP(MOC_FILES ${HEADER_FILES})

#启用qrc资源文件
QT5_ADD_RESOURCES(RES_FILES resourse.qrc)

#启用当前头文件目录
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#将ui文件和生成文件整理在一个文件夹中
SOURCE_GROUP("Form Files" FILES ${FORMS})
#将qrc资源文件整理在一个文件夹中
SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})
#查找当前目录下所有的.h和.cpp文件
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SRC_FILES})
#翻译文件
SOURCE_GROUP("Translation Files" FILES ${TS_FILES})

#将MOC,UI,QRC的中间生成文件编译进EXE中
SOURCE_GROUP("Generated Files" FILES ${MOC_FILES} ${FORMS_UIC} ${RES_FILES} ${TS_FILES})

#引入库目录路径
SET(SDK_PATH "../SDK-PATH")
LINK_DIRECTORIES(${SDK_PATH}/Lib ${SDK_PATH}/opencv3.4/lib-world ${SDK_PATH}/libpng/lib ${SDK_PATH}/log4cplus/lib ${SDK_PATH}/zlib/lib)

#引入库目录路径
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/x64)

#将所有文件编译成EXE,并设置以WIN32的方式运行
ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${SRC_FILES} ${MOC_FILES} ${FORMS_UIC} ${RESOURCE_FILES} ${RES_FILES})

#设置EXE文件的输出目录
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/x64)

#引入库目录头文件
INCLUDE_DIRECTORIES(
 ${SDK_PATH}/libpng/include
 ${SDK_PATH}/zlib/include
 ${SDK_PATH}/log4cplus/include
 ${SDK_PATH}/opencv3.4/include
 ${SDK_PATH}/opencv3.4/include/opencv
 ${SDK_PATH}/opencv3.4/include/opencv2
 )
INCLUDE_DIRECTORIES(${AUTOGEN_BUILD_DIR})

#链接以上lib库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBRARIES})

#链接opencv开发库
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	debug opencv_world343d.lib			optimized opencv_world343.lib					
	debug zlibstaticd.lib				optimized zlibstatic.lib
	debug libpng16_staticd.lib			optimized libpng16_static.lib
	debug log4cplusUD.lib				optimized log4cplusU.lib
)

#启用Qt的库模块
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql Qt5::Network Qt5::Concurrent)
QT5_USE_MODULES(${PROJECT_NAME} Widgets Core Gui Sql Network Concurrent)