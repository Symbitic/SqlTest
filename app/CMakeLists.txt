cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(AddResources)

find_package(Qt5 CONFIG REQUIRED
    COMPONENTS Charts Concurrent Qml Quick QuickControls2 Sql Widgets
    OPTIONAL_COMPONENTS QuickCompiler
)

#set_package_properties("LibZip" PROPERTIES
#    DESCRIPTION  "A library for reading, creating, and modifying zip archives"
#    URL "https://libzip.org/"
#    TYPE RECOMMENDED
#    PURPOSE "Support EPub documents in QReader."
#)

add_resources(RCC_SRCS sqltester.qrc)

add_executable(sqltester
    main.cpp
    ${RCC_SRCS}
)

target_link_libraries(sqltester PRIVATE
    Qt5::Charts
    Qt5::Concurrent
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Sql
    Qt5::Widgets
    sqltest
)

#if (TARGET LibZip::LibZip)
#    target_sources(QReader PRIVATE
#        formats/epub/epub.h
#        formats/epub/epub.cpp
#    )
#
#    target_link_libraries(QReader PRIVATE Qt5::Xml LibZip::LibZip)
#endif ()

# Install the runtime
install(TARGETS sqltester
    EXPORT SqlTester
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT GUI
)

# Install the CMake targets
install(EXPORT SqlTester
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    NAMESPACE SqlTest::
    FILE SqlTesterTargets.cmake
    COMPONENT Devel
)
