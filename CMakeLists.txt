cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(slack++ VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include_directories(include src)

set(SOURCES
	src/SlackClient.cpp
	src/SlackUI.cpp
	src/Window.cpp
	src/Input.cpp
	src/Chat.cpp
	src/Roster.cpp
	src/Session.cpp

	include/SlackClient.hpp
	include/SlackUI.hpp
	include/Headers.hpp
	include/Log.hpp
	include/Window.hpp
	include/Input.hpp
	include/Chat.hpp
	include/Roster.hpp
	include/Session.hpp
	include/WebsocketClient.hpp)

add_executable(slack++ main.cpp ${SOURCES})

find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(Jsoncpp REQUIRED)
find_package(Libnotify)
if(LIBNOTIFY_FOUND)
    message("Using libnotify")
    add_definitions( -DLIBNOTIFY_FOUND)
endif(LIBNOTIFY_FOUND)
find_package(Threads)

set(CURSES_NEED_NCURSES TRUE)
find_package(Ncursesw REQUIRED)

target_include_directories(slack++
	PRIVATE ${OPENSSL_INCLUDE_DIRS}
	PRIVATE ${CURL_INCLUDE_DIRS}
	PRIVATE ${JSONCPP_INCLUDE_DIRS}
	PRIVATE ${NCURSESW_INCLUDE_DIRS}
	PRIVATE ${LIBNOTIFY_INCLUDE_DIRS}
	PUBLIC include)




if ("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")

    message ("Building on FreeBSD")
    find_library(JSONCPP_LIB jsoncpp HINTS /usr/local/lib)
    find_library(NCURSESW_LIB ncursesw HINTS /usr/local/lib)
    target_link_libraries(slack++
    	${OPENSSL_SSL_LIBRARY}
	${OPENSSL_CRYPTO_LIBRARY}
	${CURL_LIBRARIES}
	${JSONCPP_LIB}
	${NCURSESW_LIB}
	${LIBNOTIFY_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})
else()
    message("Building default (${CMAKE_SYSTEM_NAME})")
    target_link_libraries(slack++
	${OPENSSL_SSL_LIBRARY}
	${OPENSSL_CRYPTO_LIBRARY}
	${CURL_LIBRARIES}
	${JSONCPP_LIBRARIES}
	${NCURSESW_LIBRARIES}
	${LIBNOTIFY_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})
endif()

set_property(TARGET slack++ PROPERTY CXX_STANDARD 14)
