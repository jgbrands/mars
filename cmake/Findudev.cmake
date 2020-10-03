find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_drm QUIET "libudev")

find_path(udev_INCLUDE_DIR
        NAMES libudev.h
        PATHS ${PC_udev_INCLUDE_DIRS})

find_library(udev_LIBRARY
        NAMES udev
        PATHS ${PC_udev_LIBRARY_DIRS})

# Set the version and mark these artifacts as advanced.
set(udev_VERSION ${PC_udev_VERSION})
mark_as_advanced(udev_FOUND udev_INCLUDE_DIR udev_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(udev
        REQUIRED_VARS udev_LIBRARY udev_INCLUDE_DIR
        VERSION_VAR udev_VERSION)

if (udev_FOUND)
    set(udev_INCLUDE_DIRS ${PC_udev_INCLUDE_DIRS})
    set(udev_LIBRARIES ${udev_LIBRARY})
endif ()

if (udev_FOUND AND NOT TARGET udev::udev)
    add_library(udev::udev INTERFACE IMPORTED)
    set_target_properties(udev::udev PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${udev_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${udev_LIBRARY}")
endif ()
