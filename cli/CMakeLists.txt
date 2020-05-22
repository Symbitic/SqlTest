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

# Install the runtime
install(TARGETS sqltestrunner
    EXPORT SqlTestRunner
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)

# Install the CMake targets
install(EXPORT SqlTestRunner
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    NAMESPACE SqlTest::
    FILE SqlTestRunnerTargets.cmake
    COMPONENT Devel
)
