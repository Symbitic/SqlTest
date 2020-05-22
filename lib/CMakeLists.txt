cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Qt5 CONFIG REQUIRED
    COMPONENTS Core Sql
)

add_library(sqltest
    QSqlTest.cpp
    QSqlTest.h
    QSqlTestCsvFile.cpp
    QSqlTestCsvFile.h
    QSqlTestDatabase.cpp
    QSqlTestDatabase.h
    QSqlTestJsonFile.cpp
    QSqlTestJsonFile.h
    QSqlTestLogger.cpp
    QSqlTestLogger.h
)

target_include_directories(sqltest
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(sqltest PUBLIC
    Qt5::Core
    Qt5::Sql
)

# Install the library
install(TARGETS sqltest
    EXPORT SqlTest
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Devel
)

# Install the headers
install(FILES QSqlTest.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RENAME QSqlTest
    COMPONENT Devel
)

# Install the CMake targets
install(EXPORT SqlTest
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    NAMESPACE SqlTest::
    FILE SqlTestTargets.cmake
    COMPONENT Devel
)
