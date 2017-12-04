project(libtrivc)
cmake_minimum_required(VERSION 2.8)
set(VERSION 0.0.1)

macro(space_append VAR)
  string(REPLACE ";" " " SPACED_ITEMS "${ARGN}")
  set(${VAR} "${${VAR}} ${SPACED_ITEMS}")
endmacro(space_append)

# build
# =====

space_append(CMAKE_C_FLAGS
	-std=gnu11
	-D_FILE_OFFSET_BITS=64
	-g -Wall -Wextra -Werror
	-Wno-unused-parameter
	-Wformat=2 -Wcomment
	-Wundef -Wunknown-pragmas -Wmissing-include-dirs
	-Wfloat-equal -Wsequence-point -Wpointer-arith -Woverlength-strings
	-Wcast-align
	-Wswitch-default
	-Wparentheses -Wlogical-op
	-Wdisabled-optimization
	-Winline
	-Wswitch-enum
	-Wstrict-prototypes -Wdeclaration-after-statement -Wnested-externs
	-Wjump-misses-init -Wwrite-strings
	-Wframe-larger-than=16384
	)

space_append(CMAKE_CXX_FLAGS
	-std=gnu++11
	-D_FILE_OFFSET_BITS=64
	-g -Wall -Wextra -Werror
	-Wno-unused-parameter
	)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}
	)

set(SRCS
	string.c
	atomic.c
	buf.c
	)

add_library(trivc SHARED ${SRCS})
target_link_libraries(trivc z)
install(TARGETS trivc
	LIBRARY
	DESTINATION lib64)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/contrib/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_subdirectory(include)

# testing
# =======

include(CTest)

add_subdirectory(test)
