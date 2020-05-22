cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Test)

foreach(tst IN ITEMS tst_sqltest)
    add_executable(${tst} ${tst}.cpp)
    target_link_libraries(${tst} PRIVATE sqltest Qt5::Test)
    add_test(NAME ${tst} COMMAND ${tst})
endforeach()
