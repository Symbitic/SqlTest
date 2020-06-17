cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Qt5 CONFIG REQUIRED
    COMPONENTS Core
)

add_executable(sqltestrunner
    main.cpp
)

target_link_libraries(sqltestrunner PRIVATE
    sqltest
    Qt5::Core
)

if(DEFINED ENV{CI} AND NOT CMAKE_BUILD_TYPE MATCHES "[Dd]ebug")
    if(WIN32)
        set_target_properties(sqltestrunner PROPERTIES SUFFIX -win32.exe)
    elseif(APPLE)
        set_target_properties(sqltestrunner PROPERTIES SUFFIX -macOS)
    else()
        set_target_properties(sqltestrunner PROPERTIES SUFFIX -linux)
    endif()
endif()

# Install the runtime
install(TARGETS sqltestrunner
    EXPORT SqlTestRunner
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
    RENAME sqltestrunner
)

# Install the CMake targets
install(EXPORT SqlTestRunner
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    NAMESPACE SqlTest::
    FILE SqlTestRunnerTargets.cmake
    COMPONENT Devel
)
