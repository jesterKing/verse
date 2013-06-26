
# CMakeFile.txt for Python module

if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -fPIC -Wall -Wextra -pedantic -std=c99 -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -fPIC -Wall -Wextra -pedantic -std=c99  -O3")
	endif()
endif (CMAKE_COMPILER_IS_GNUCC)

include_directories (../include)
include_directories (${PYTHON_INCLUDE_DIRS})

set (verse_module_src verse_module.c)

message("Python lib: ${PYTHON_LIBRARY}")

add_library (pyverse SHARED ${verse_module_src})
set_target_properties (pyverse PROPERTIES PREFIX "")
set_target_properties (pyverse PROPERTIES 
		OUTPUT_NAME "python/verse${VERSE_VERSION}")
add_dependencies (pyverse verse_static_lib)
target_link_libraries (pyverse
		verse_static_lib
		${CMAKE_THREAD_LIBS_INIT}
		${OPENSSL_LIBRARIES}
		${PYTHON_LIBRARY})

install (TARGETS pyverse
	LIBRARY DESTINATION lib/python${PYTHON_VERSION}/site-packages
	ARCHIVE DESTINATION lib/python${PYTHON_VERSION}/site-packages
	CONFIGURATIONS "Debug")
