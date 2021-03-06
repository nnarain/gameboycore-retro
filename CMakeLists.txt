cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")


project(gameboycore_libretro)

# Get project version from git tag
include(get-version-config)
get_version()

# External dependencies
add_subdirectory(src/external)

set(GAMEBOYCORE_RETRO_SOURCES
    src/libretro.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${GAMEBOYCORE_RETRO_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${LIBRETRO_INCLUDE_DIR}
)

target_compile_options(${PROJECT_NAME} PRIVATE -DGAMEBOYCORE_STATIC=1 -DGAMEBOYCORE_RETRO_VERSION="${VERSION}")

target_link_libraries(${PROJECT_NAME}
    gameboycore::gameboycore
)

########################################################################################################################
### Installation                                                                                                     ###
########################################################################################################################

# Configure the .info file
configure_file(config/gameboycore_libretro.info.in ${CMAKE_BINARY_DIR}/gameboycore_libretro.info @ONLY)

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION cores
)

install(
    FILES ${CMAKE_BINARY_DIR}/gameboycore_libretro.info
    DESTINATION info
)

include(CPackComponent)

set(CPACK_PACKAGE_NAME "gameboycore_libretro")
set(CPACK_PACKAGE_VENDOR "nnarain")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GameboyCore libretro core")
set(CPACK_PACKAGE_VERSION "${VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "gameboycore_libretro")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

include(CPack)
