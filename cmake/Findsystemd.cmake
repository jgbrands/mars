find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_systemd QUIET "libsystemd")

find_path(systemd_INCLUDE_DIR
        NAMES sd-daemon.h
        PATHS ${PC_systemd_INCLUDE_DIRS}
        PATH_SUFFIXES systemd)

find_library(systemd_LIBRARY
        NAMES systemd
        PATHS ${PC_systemd_LIBRARY_DIRS})

# Set the version and mark these artifacts as advanced.
set(systemd_VERSION ${PC_systemd_VERSION})
mark_as_advanced(systemd_FOUND systemd_INCLUDE_DIR systemd_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(systemd
        REQUIRED_VARS systemd_INCLUDE_DIR systemd_LIBRARY
        VERSION_VAR systemd_VERSION)

if (systemd_FOUND)
    set(systemd_INCLUDE_DIRS ${systemd_INCLUDE_DIR})
    set(systemd_LIBRARIES ${systemd_LIBRARY})
endif ()

if (systemd_FOUND AND NOT TARGET systemd::systemd)
    add_library(systemd::systemd INTERFACE IMPORTED)
    set_target_properties(systemd::systemd PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${systemd_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${systemd_LIBRARY}")
endif ()
