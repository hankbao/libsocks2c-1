cmake_minimum_required(VERSION 3.12)
project(libsocks2c)

OPTION(BUILD_DEBUG "" OFF)
OPTION(BUILD_DLL "" OFF)
OPTION(ENABLE_UOUT "whether build with UDP Over UTCP feature" OFF)
OPTION(BUILD_MOBILE_LIB "build mobile platform lib" OFF)

IF (ENABLE_UOUT)
    add_definitions(-DUDP_OVER_UTCP)
ENDIF()

add_definitions(-DPROTOCOL_AES256GCM)


add_definitions(-DDISABLE_DEBUG_LOG)
#add_definitions(-DLOG_DEBUG_DETAIL)
#add_definitions(-DUDP_DEBUG_DETAIL)
#add_definitions(-DTCP_DEBUG_DETAIL)
add_definitions(-DMULTITHREAD_IO)
add_definitions(-DSINGLETON_RAW_POINTER)
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
set(CMAKE_CXX_STANDARD 17)

#include headers
IF (WIN32)
  add_definitions(-DTINS_STATIC)
  add_definitions(-D_WIN32_WINNT=0x0A00)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_definitions(-DSODIUM_STATIC)
  add_definitions(-DBUILD_DLL)
  IF (ENABLE_UOUT)
      include_directories("C:/libtins/include")
      include_directories("C:/WpdPack/Include")
      include_directories("C:/WinDivert-1.4.3-A/include")
  ENDIF()
    include_directories("C:/libsodium/include")
	include_directories("C:/spdlog-1.3.1/include")
ELSE()
    IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        #all in include_directories("/usr/local/include")
    ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
        #include_directories("/root/spdlog-1.3.1/include")
    ENDIF()
    include_directories("/usr/local/include")
ENDIF()

SET(CORE_FILES
        lib/libsocks2c.cpp
		lib/libsocks2c.h
        lib/libsocks2c-client.cpp
        lib/libsocks2c-client.h
        src/protocol/socks5/socks5_protocol.h
        src/protocol/socks5/socks5_protocol_helper.cpp
        src/protocol/socks5/socks5_protocol_helper.h
        src/netio/basic_network_io_mt.h
        src/netio/basic_network_io_mt.cpp

        src/utils/logger.cpp
        src/utils/logger.h
        src/protocol/custom/chacha20poly1305withobf/chacha20poly1305withobf.h
        src/protocol/basic_protocol/iproxy_protocol.h
        src/protocol/custom/chacha20poly1305withobf/chacha20poly1305withobf_helper.h
        src/protocol/basic_protocol/client_proxy_protocol.h
        src/utils/randomNumberGenerator.cpp
        src/utils/randomNumberGenerator.h

        src/protocol/basic_protocol/protocol_def.h

        src/protocol/custom/chacha20/chacha20.h
        src/protocol/custom/chacha20/chacha20_helper.h

        src/utils/singleton.h
		src/net/udp/bufferqueue.h
        src/utils/macro_def.h
        src/utils/sqlhost.h
        src/factory/client_factory.h
        lib/client_proxymap.h
        )

message("Build Client")
LIST(APPEND CORE_FIELS
        src/net/tcp/client/client_tcp_proxy.h
        src/net/tcp/client/client_tcp_proxy_session.h

        src/net/udp/client/client_udp_proxy.h
        src/net/udp/client/client_udp_proxy_session.h)


IF (ENABLE_UOUT)
    message("Build uout")

	LIST(APPEND CORE_FILES
		src/net/udp/client/raw/helper/available_port.cpp
		src/net/udp/client/raw/helper/available_port.h
		src/net/udp/client/raw/helper/firewall.cpp
		src/net/udp/client/raw/helper/firewall.h
        src/net/udp/client/raw/helper/tcp_checksum.cpp

        src/utils/system/system_exec.h
		)
ENDIF()

IF(BUILD_DLL)
	add_library(libsocks2c SHARED ${CORE_FILES})
ELSE()
	add_library(libsocks2c ${CORE_FILES})
ENDIF()
#LIST(APPEND CORE_FILES main.cpp)
#add_executable(libsocks2c ${CORE_FILES})
set_target_properties(libsocks2c PROPERTIES PREFIX "")

#link libs and platform specific source file
IF (WIN32)
		include_directories("C:/Boost/include/boost-1_70")

		IF (BUILD_DEBUG)
			IF (ENABLE_UOUT)
				target_link_libraries(libsocks2c "C:/libtins/lib/debug/tins.lib")
			ENDIF()
			target_link_libraries(libsocks2c "C:/libsodium/x64/Debug/v142/static/libsodium.lib")
			
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_regex-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_context-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_coroutine-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_context-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_date_time-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_thread-vc142-mt-gd-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_chrono-vc142-mt-gd-x64-1_70.lib")
		ELSE()
			IF (ENABLE_UOUT)
				target_link_libraries(libsocks2c "C:/libtins/lib/release/tins.lib")
			ENDIF()
			target_link_libraries(libsocks2c "C:/libsodium/x64/Release/v142/static/libsodium.lib")
			
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_regex-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_context-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_coroutine-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_context-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_date_time-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_thread-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_chrono-vc142-mt-x64-1_70.lib")
			target_link_libraries(libsocks2c "C:/Boost/lib/libboost_date_time-vc142-mt-x64-1_70.lib")
		ENDIF()

		IF (ENABLE_UOUT)
			target_link_libraries(libsocks2c "C:/WpdPack/Lib/x64/wpcap.lib")
			target_link_libraries(libsocks2c "C:/WinDivert-1.4.3-A/x86_64/WinDivert.lib")
			target_link_libraries(libsocks2c "Iphlpapi.lib")
		ENDIF()

ELSE()
    target_link_libraries(libsocks2c pthread)
    target_link_libraries(libsocks2c "/usr/local/lib/libsodium.a")

	IF (ENABLE_UOUT)
		IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
			target_link_libraries(libsocks2c "/usr/local/lib/libtins.a")
			target_link_libraries(libsocks2c "/usr/local/lib/libpcap.a")
		ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
			target_link_libraries(libsocks2c "/usr/local/lib/libtins.a")
			target_link_libraries(libsocks2c "/usr/local/lib/libpcap.a")
		ENDIF()
	ENDIF()
    
ENDIF()

IF(NOT BUILD_MOBILE_LIB)

	IF(NOT WIN32)
	    target_link_libraries(libsocks2c "/usr/local/lib/libboost_thread.a")
		target_link_libraries(libsocks2c "/usr/local/lib/libboost_regex.a")
		target_link_libraries(libsocks2c "/usr/local/lib/libboost_coroutine.a")
        target_link_libraries(libsocks2c "/usr/local/lib/libboost_context.a")
        target_link_libraries(libsocks2c "/usr/local/lib/libboost_filesystem.a")
	ENDIF()

ENDIF()

if (BUILD_MOBILE_LIB)
    set(ENABLE_UOUT, OFF)
    remove_definitions(-DUDP_OVER_UTCP)
    remove_definitions(-DMULTITHREAD_IO)
endif()


IF(BUILD_TESTING)
	add_subdirectory(test/socks5protocolhelper)
	enable_testing()
	add_test(NAME Socks5ProtocolHelper COMMAND Test)
ENDIF()


