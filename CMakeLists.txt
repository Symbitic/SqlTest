cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_BUILD_TYPE_INIT "Release")

project(SqlTester LANGUAGES CXX VERSION 0.1.0)

option(ENABLE_GUI "Enable the SqlTester GUI" OFF)
option(ENABLE_TESTS "Enable unit tests" ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(GNUInstallDirs)

# Determine install location for CMake files
if(WIN32)
    set(CMAKE_INSTALL_CMAKEDIR CMake
        CACHE STRING "Installation directory for CMake files")
else()
    set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SqlTest
        CACHE STRING "Installation directory for CMake files")
endif()

add_subdirectory(lib)
add_subdirectory(cli)
if(ENABLE_GUI)
    add_subdirectory(app)
endif()
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Generate local targets file
export(TARGETS sqltest
    NAMESPACE SqlTest::
	FILE "${CMAKE_CURRENT_BINARY_DIR}/SqlTestDepends.cmake")

# Generate CMake packages
configure_package_config_file(SqlTestConfig.cmake.in SqlTestConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR})
write_basic_package_version_file(SqlTestConfigVersion.cmake
    COMPATIBILITY AnyNewerVersion)

# Install the CMake config files
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/SqlTestConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/SqlTestConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
  COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(PROJECT_CONTACT "Alex Shaw <alex.shaw.as@gmail.com>")
set(PROJECT_DESCRIPTION "Acceptance testing for SQL")
set(PROJECT_VENDOR "Alex Shaw")
set(PROJECT_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(PROJECT_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(PROJECT_LICENSE_NAME "MIT")
set(PROJECT_HOMEPAGE "https://github.com/Symbitic/QSqlTest")
set(PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
