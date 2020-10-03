cmake_minimum_required(VERSION 3.13)
project(mars
        VERSION 0.1.0
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(drm REQUIRED)

set(MARS_SOURCES
        src/main.cpp
        src/drm/DrmConnector.cpp
        src/drm/DrmEncoder.cpp
        src/drm/DrmFrameBuffer.cpp
        src/drm/DrmDevice.cpp
        src/drm/DrmDeviceResources.cpp
        src/drm/DrmSwapchain.cpp
        src/drm/DrmDumbBuffer.cpp
        src/drm/DrmVideoDisplayController.cpp
        src/drm/DrmPresentationBackend.cpp
        src/drm/DrmAtomicRequest.cpp)

set(MARS_HEADERS
        include/mars/drm/DrmConnector.hpp
        include/mars/drm/DrmEncoder.hpp
        include/mars/drm/DrmFrameBuffer.hpp
        include/mars/drm/DrmDevice.hpp
        include/mars/drm/DrmDeviceResources.hpp
        include/mars/drm/DrmVideoDisplayController.hpp
        include/mars/drm/DrmSwapchain.hpp
        include/mars/drm/DrmDumbBuffer.hpp
        include/mars/drm/DrmPropertyBlob.hpp
        include/mars/drm/DrmPresentationBackend.hpp
        include/mars/drm/DrmPlaneResources.hpp
        include/mars/drm/DrmPlane.hpp
        include/mars/drm/DrmResourcePtr.hpp
        include/mars/drm/DrmObjectProperties.hpp
        include/mars/drm/DrmAtomicRequest.hpp)

add_executable(mars
        ${MARS_SOURCES}
        ${MARS_HEADERS})

target_compile_features(mars
        PRIVATE cxx_std_17)

target_include_directories(mars
        PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(mars
        PUBLIC drm::drm)