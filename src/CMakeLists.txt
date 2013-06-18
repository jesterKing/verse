
# CMakeFile.txt for verse server and verse library


# Source code for verse library
set (libverse_src
		common/v_unpack.c
		common/v_session.c
		common/v_resend_mechanism.c
		common/v_pack.c
		common/v_network.c
		common/v_list.c
		common/v_history.c
		common/v_context.c
		common/v_connection.c
		common/v_common.c
		common/v_commands.c
		common/v_stream.c
		common/sys_cmds/v_user_auth_success.c
		common/sys_cmds/v_user_auth_request.c
		common/sys_cmds/v_user_auth_failure.c
		common/sys_cmds/v_sys_commands.c
		common/sys_cmds/v_negotiate.c
		common/sys_cmds/v_ack_nak.c
		common/queues/v_out_queue.c
		common/queues/v_in_queue.c
		common/queues/v_cmd_queue.c
		common/node_cmds/v_node_unsubscribe.c
		common/node_cmds/v_node_subscribe.c
		common/node_cmds/v_node_prio.c
		common/node_cmds/v_node_link.c
		common/node_cmds/v_node_destroy.c
		common/node_cmds/v_node_create.c
		common/node_cmds/v_node_lock.c
		common/node_cmds/v_node_unlock.c
		common/node_cmds/v_node_owner.c
		common/node_cmds/v_node_perm.c
		common/node_cmds/taggroup_cmds/v_taggroup_unsubscribe.c
		common/node_cmds/taggroup_cmds/v_taggroup_subscribe.c
		common/node_cmds/taggroup_cmds/v_taggroup_destroy.c
		common/node_cmds/taggroup_cmds/v_taggroup_create.c
		common/node_cmds/taggroup_cmds/tag_cmds/v_tag_set.c
		common/node_cmds/taggroup_cmds/tag_cmds/v_tag_destroy.c
		common/node_cmds/taggroup_cmds/tag_cmds/v_tag_create.c
		common/node_cmds/layer_cmds/v_layer_create.c
		common/node_cmds/layer_cmds/v_layer_destroy.c
		common/node_cmds/layer_cmds/v_layer_subscribe.c
		common/node_cmds/layer_cmds/v_layer_unsubscribe.c
		common/node_cmds/layer_cmds/v_layer_set_value.c
		common/node_cmds/layer_cmds/v_layer_unset_value.c
		common/fake_cmds/v_fake_user_auth.c
		common/fake_cmds/v_fake_tag_create_ack.c
		common/fake_cmds/v_fake_tag_destroy_ack.c
		common/fake_cmds/v_fake_fps.c
		common/fake_cmds/v_fake_connect_terminate.c
		common/fake_cmds/v_fake_connect_accept.c
		common/fake_cmds/v_fake_commands.c
		common/fake_cmds/v_fake_node_create_ack.c
		common/fake_cmds/v_fake_node_destroy_ack.c
		common/fake_cmds/v_fake_taggroup_create_ack.c
		common/fake_cmds/v_fake_taggroup_destroy_ack.c
		common/fake_cmds/v_fake_layer_create_ack.c
		common/fake_cmds/v_fake_layer_destroy_ack.c
		api/verse.c
		client/vc_udp_connect.c
		client/vc_tcp_connect.c
		client/vc_main.c)

if(WIN32)
	set (winstuff_src
		common/win/v_string.c
		common/win/v_time.c
		common/win/v_getopt.c)
endif()

# Source code for verse server		
set (server_src
		server/vs_user.c
		server/vs_udp_connect.c
		server/vs_tcp_connect.c
		server/vs_taggroup.c
		server/vs_tag.c
		server/vs_node.c
		server/vs_node_access.c
		server/vs_main.c
		server/vs_link.c
		server/vs_layer.c
		server/vs_data.c
		server/vs_config.c
		server/vs_auth_csv.c)
		

include_directories ("../include")
if(WIN32)
	include_directories ("../include/win")
endif()
include_directories (${INIPARSER_INCLUDE_DIRS})
include_directories (${OPENSSL_INCLUDE_DIR})
include_directories (${PTHREAD_INCLUDE_DIR})

if(WIN32)
	set(LINK_DIRECTORIES ${PTHREAD_LIB_DIR} ${OPENSSL_LIB_DIR})
endif()

# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -fPIC -Wall -Wextra -pedantic -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -fPIC -Wall -Wextra -pedantic -O3")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

if (MSVC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT /MTd -Od")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG /MT	-O2")
	endif ()
	set (CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:CONSOLE /INCREMENTAL:NO /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib")
endif (MSVC)

if(WIN32)
	add_library (winstuff_lib STATIC ${winstuff_src})
	set_target_properties (winstuff_lib PROPERTIES 
		OUTPUT_NAME "winstuff"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/static/"
	)
	set (EXTRA_LIBS ${EXTRA_LIBS} winstuff_lib)
endif()


# Set up shared verse library (libverse.so)
add_library (verse_shared_lib SHARED ${libverse_src})
if (WIN32)

# TODO: Make building static verse library (libverse.a) optional
add_library (verse_static_lib STATIC ${libverse_src})
set_target_properties (verse_static_lib PROPERTIES 
		OUTPUT_NAME "verse"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/static/")
target_link_libraries (verse_shared_lib
		${EXTRA_LIBS}
		${LIB_EAY_DEBUG}
		${OPENSSL_LIBRARIES}
		${INIPARSER_LIBRARIES}
		${PTHREAD_LIBRARIES}
		${PLATFORM_LINKLIBS}
	)
target_link_libraries(verse_static_lib
		${EXTRA_LIBS}
		${LIB_EAY_DEBUG}
		${OPENSSL_LIBRARIES}
		${INIPARSER_LIBRARIES}
		${PTHREAD_LIBRARIES}
		${PLATFORM_LINKLIBS})
endif(WIN32)
set_target_properties (verse_shared_lib PROPERTIES
		VERSION "${${PROJECT_NAME}_VERSION}"
		SOVERSION "${${PROJECT_NAME}_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}"
		OUTPUT_NAME "verse"
		clean_direct_output 1)


# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    	set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -Wall -Wextra -pedantic")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -Wall -Wextra -pedantic")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

message ("OpenSSL libs: ${OPENSSL_LIBRARIES}")
message ("CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")
message ("PTREAD_LIBRARIES: ${PTHREAD_LIBRARIES}")
message ("LINK_DIRECTORIES: ${LINK_DIRECTORIES}")
message ("INIPARSER_LIBRARIES: ${INIPARSER_LIBRARIES}")
message ("PLATFORM_LINKLIBS: ${PLATFORM_LINKLIBS}")

# Set up verse server executable
add_executable (verse_server ${server_src})
add_dependencies (verse_server verse_shared_lib)
if(WIN32)
add_dependencies (verse_server winstuff_lib)
target_link_libraries (verse_server
		verse_shared_lib
		${EXTRA_LIBS}
		${CMAKE_THREAD_LIBS_INIT}
		${LIB_EAY_DEBUG}
		${INIPARSER_LIBRARIES}
		${PTHREAD_LIBRARIES}
		${PLATFORM_LINKLIBS})
else()
target_link_libraries (verse_server
		verse_shared_lib
		${CMAKE_THREAD_LIBS_INIT}
		${OPENSSL_LIBRARIES}
		${INIPARSER_LIBRARIES}
		${PTHREAD_LIBRARIES}
		${PLATFORM_LINKLIBS})
endif()


# The rule to install Verse server
install (TARGETS verse_server
	RUNTIME DESTINATION bin
	CONFIGURATIONS "Debug"
	)

# The rule to install shared library used by Verse server and Verse clients
install (TARGETS verse_shared_lib
	CONFIGURATIONS "Debug"
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
