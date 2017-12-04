project(libtrivc)
cmake_minimum_required(VERSION 2.8)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

macro(space_append VAR)
  string(REPLACE ";" " " SPACED_ITEMS "${ARGN}")
  set(${VAR} "${${VAR}} ${SPACED_ITEMS}")
endmacro(space_append)

find_program(DPKG dpkg)
find_program(RPM rpm)
#message(${DPKG})
#message(${RPM})
if (EXISTS ${DPKG})
	set(CPACK_GENERATOR "DEB")
	set(LIB_DESTINATION "lib")
	set(PACKAGE_HEADERS_SUFFIX "dev")
	set(PACKAGE_EXTENSION "deb")
	set(PACKAGE_FIX "${CMAKE_CURRENT_SOURCE_DIR}/contrib/package/cpack-deb-fix.sh")
elseif (EXISTS ${RPM})
	set(CPACK_GENERATOR "RPM")
	set(LIB_DESTINATION "lib64")
	set(PACKAGE_HEADERS_SUFFIX "devel")
	set(PACKAGE_EXTENSION "rpm")
	set(PACKAGE_FIX "${CMAKE_CURRENT_SOURCE_DIR}/contrib/package/cpack-rpm-fix.sh")
else()
	message(FATAL_ERROR "unknown package type")
endif()


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
set(TRIVC_VERSION_STRING "${VERSION}")
set_target_properties(trivc PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR})
install(TARGETS trivc
	LIBRARY
	COMPONENT library
	DESTINATION ${LIB_DESTINATION})

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

# packaging
# =========

set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "the trivial C stuff")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_COMPONENTS_ALL library headers)

set(CPACK_COMPONENT_LIBRARY_DESCRIPTION "libtrivc runtime")
set(CPACK_COMPONENT_LIBRARY_DISPLAY_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_RPM_LIBRARY_FILE_NAME "${CPACK_PACKAGE_NAME}.${PACKAGE_EXTENSION}")
set(CPACK_RPM_LIBRARY_DEBUGINFO_PACKAGE ON)

set(CPACK_COMPONENT_HEADERS_DESCRIPTION "header files for libtrivc")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME
	"${CPACK_PACKAGE_NAME} ${PACKAGE_HEADERS_SUFFIX}")
set(CPACK_COMPONENT_HEADERS_DEPENDS library)
set(CPACK_${CPACK_GENERATOR}_HEADERS_FILE_NAME
	"${CPACK_PACKAGE_NAME}-${PACKAGE_HEADERS_SUFFIX}.${PACKAGE_EXTENSION}")

set(CPACK_PACKAGE_CONTACT "Idan Kedar <ik.imbaf@gmail.com>")

# TODO package dependencies
set(CPACK_${CPACK_GENERATOR}_COMPONENT_INSTALL 1)
include(CPack)

set(PACKAGE_BASE "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(LIBRARY_SUFFIX "-Linux-library")
set(HEADERS_SUFFIX "-Linux-headers")

add_custom_target(package-fix
	COMMAND "${PACKAGE_FIX}"
		"${PACKAGE_BASE}${LIBRARY_SUFFIX}.${PACKAGE_EXTENSION}"
		"-library" ""
	COMMAND "${PACKAGE_FIX}"
		"${PACKAGE_BASE}${HEADERS_SUFFIX}.${PACKAGE_EXTENSION}"
		"-headers" "-${PACKAGE_HEADERS_SUFFIX}"
)
