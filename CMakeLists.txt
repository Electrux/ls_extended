cmake_minimum_required(VERSION 3.0)

project(ls_extended
	VERSION 1.0.0
	LANGUAGES C)

add_executable(ls_extended
	src/cmdopts.c
	src/colors.c
	src/core.c
	src/help.c
	src/icons.c
	src/ls.c
	src/main.c
	src/strvec.c
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
