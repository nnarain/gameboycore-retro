#
# CMake find module for GameboyCore
#
# @author Natesh Narain
# @date Nov 12 2016
#

find_path(GAMEBOYCORE_DIR
    NAMES
        include/gameboycore/gameboycore_api.h
    PATHS
        ${GAMEBOYCORE_ROOT}
        $ENV{GAMEBOYCORE_ROOT}

        /usr/include/
        /usr/local/include

        C:/
)

set(GAMEBOYCORE_INCLUDE_DIR "${GAMEBOYCORE_DIR}/include")
set(GAMEBOYCORE_LIBRARIES   "${GAMEBOYCORE_DIR}/lib/gameboycore.lib")
set(GAMEBOYCORE_STATIC_LIB  "${GAMEBOYCORE_DIR}/lib/gameboycore-s.lib")
set(GAMEBOYCORE_BIN_DIR     "${GAMEBOYCORE_DIR}/bin")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GAMEBOYCORE
	DEFAULT_MSG
	GAMEBOYCORE_INCLUDE_DIR
    GAMEBOYCORE_LIBRARIES
    GAMEBOYCORE_STATIC_LIB
    GAMEBOYCORE_BIN_DIR
)
