# Courtesy of reddit user u/gbeier

# Builds RPM for the project.

# I have not tested this myself since I do not have an RPM based OS right now.
# In the mean time, if someone does use it, please do tell if it works correctly :)

cmake_minimum_required(VERSION 3.10)
project(ls_extended)
add_executable(${CMAKE_PROJECT_NAME} src/cmdopts.c  src/colors.c  src/core.c  src/help.c  src/icons.c  src/ls.c  src/main.c  src/strvec.c)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SRC_DIR}/include)
 
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}-${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/local /usr/local/bin)
include(CPack)
