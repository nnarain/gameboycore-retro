cmake_minimum_required(VERSION 3.0)

project(gameboycore_libretro)

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)

if(IS_BIG_ENDIAN)
	set(ENDIAN "__BIGENDIAN__")
else(IS_BIG_ENDIAN)
	set(ENDIAN "__LITTLEENDIAN__")
endif(IS_BIG_ENDIAN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# gameboycore
find_package(GameboyCore)

message("${GAMEBOYCORE_STATIC_LIB}")

SET(GAMEBOYCORE_RETRO_HEADERS

)

SET(GAMEBOYCORE_RETRO_SOURCES
    src/libretro.cpp
)

include_directories(
    ${GAMEBOYCORE_INCLUDE_DIR}
    ${LIBRETRO_COMMON_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} SHARED
    ${GAMEBOYCORE_RETRO_HEADERS}
    ${GAMEBOYCORE_RETRO_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	COMPILE_FLAGS "-DGAMEBOYCORE_STATIC"
)

target_link_libraries(${PROJECT_NAME}
	${GAMEBOYCORE_STATIC_LIB}
)

# installation
include(get-version-config)
get_version()

install(
	TARGET ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

set(CPACK_GENERATOR "ZIP")

set(CPACK_PACKAGE_NAME "gameboycore_libretro")
set(CPACK_PACKAGE_VENDOR "nnarain")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GameboyCore libretro core")
set(CPACK_PACKAGE_VERSION "${VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "gameboycore_libretro")

include(CPack)
