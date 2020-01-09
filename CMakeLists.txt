cmake_minimum_required(VERSION 3.1)

#set project name and language
project(vf-storage-hash LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag# spawn out some info on configuration
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(VfStorageHash_VERSION_MAJOR "0")
set(VfStorageHash_VERSION_MINOR "0")
set(VfStorageHash_VERSION_PATCH "1")

set(PROJECT_VERSION "${VfStorageHash_VERSION_MAJOR}.${VfStorageHash_VERSION_MINOR}.${VfStorageHash_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core CONFIG REQUIRED)
find_package(VfHelpers REQUIRED)
find_package(VfEvent REQUIRED)
find_package(VfComponent REQUIRED)

#sum up project Files 
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

#create library 
add_library(VfStorageHash SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )


#link dependencies
target_link_libraries(VfStorageHash 
    PRIVATE
    Qt5::Core
    VeinMeta::VfHelpers
    VeinMeta::VfComponent
    VeinMeta::VfEvent
    )

# announce headers - target perspective
target_include_directories(VfStorageHash
    PUBLIC
    $<INSTALL_INTERFACE:include/VfStorageHash>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}	
    )

#set target Version
set_target_properties(VfStorageHash PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfStorageHash PROPERTIES SOVERSION ${VfStorageHash_VERSION_MAJOR})

message(STATUS "Project Version: " ${PROJECT_VERSION})

#generate export header
include(GenerateExportHeader)
generate_export_header(VfStorageHash)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfstoragehash_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )



# install library
install(TARGETS VfStorageHash
    EXPORT VfStorageHashExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure MarkdownQtConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    VfStorageHashConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

# configure *Config.cmake
configure_file(VfStorageHashConfig.cmake.in VfStorageHashConfig.cmake @ONLY)


# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfStorageHashConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfStorageHashConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfStorageHash
    )

# install targets cmake-files
install(EXPORT VfStorageHashExport
    FILE VfStorageHashTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfStorageHash
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)



