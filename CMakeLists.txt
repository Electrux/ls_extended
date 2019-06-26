cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE "RelWithDebInfo"
	CACHE STRING "CMake Build Type")

project(ls_extended
	VERSION 2.0.0
	LANGUAGES C)

add_executable(ls_extended
	src/cmdopts.c
	src/color.c
	src/core.c
	src/disp_basic.c
	src/disp_long.c
	src/help.c
	src/icons.c
	src/ls.c
	src/sorts.c
	src/vec.c
	src/main.c
	)
target_include_directories(ls_extended PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS ls_extended RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "RPM;DEB;TGZ;7Z;ZIP")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "Noone <noone@localhost.local>"
	CACHE STRING "(CPack) Package Contact")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ""
	CACHE STRING "(CPack) DEB Package Maintainer")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/local /usr/local/bin)
include(CPack)
