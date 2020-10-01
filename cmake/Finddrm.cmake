find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_drm QUIET "libdrm")

find_path(drm_INCLUDE_DIR
        NAMES drm.h
        PATHS ${PC_drm_INCLUDE_DIRS})

find_library(drm_LIBRARY
        NAMES drm
        PATHS ${PC_drm_LIBRARY_DIRS})

# Set the version and mark these artifacts as advanced.
set(drm_VERSION ${PC_drm_VERSION})
mark_as_advanced(drm_FOUND drm_INCLUDE_DIR drm_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(drm
        REQUIRED_VARS drm_INCLUDE_DIR drm_LIBRARY
        VERSION_VAR drm_VERSION)

if (drm_FOUND)
    set(drm_INCLUDE_DIRS ${PC_drm_INCLUDE_DIRS})
    set(drm_LIBRARIES ${drm_LIBRARY})
endif ()

if (drm_FOUND AND NOT TARGET drm::drm)
    add_library(drm::drm INTERFACE IMPORTED)
    set_target_properties(drm::drm PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${drm_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${drm_LIBRARY}")
endif ()
