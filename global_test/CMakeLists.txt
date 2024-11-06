find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem unit_test_framework)

add_definitions(-DBOOST_TEST_DYN_LINK -g)
add_executable(mytest1 main.cpp)

target_link_libraries(mytest1
    PRIVATE Boost::boost
    PRIVATE Boost::system
    PRIVATE Boost::filesystem
    PRIVATE Boost::unit_test_framework)