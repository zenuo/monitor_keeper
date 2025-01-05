cmake_minimum_required(VERSION 3.16)

project(monitor_keeper
    LANGUAGES CXX
)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

#############################
# Compile time dependencies #
#############################

include(FetchContent)
FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/f5e54359df4c26b6230fc61d38aa294581393084.zip
    URL_HASH SHA256=17c535ec9abe0ea6d1e150875627c585f4b1293b769670da013ce24208726167
)
FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/v1.12.0.tar.gz
    URL_HASH SHA256=4dccf2d10f410c1e2feaff89966bfc49a1abb29ef6f08246335b110e001e09a9
)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(fmt spdlog json googletest)


####################
# Compiler options #
####################

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(WARNING_FLAGS "-Wall")

LIST(APPEND MK_LIBS
    fmt::fmt
    nlohmann_json::nlohmann_json
    spdlog::spdlog_header_only
)

add_executable(monitor_keeper
    main.cc
    monitor_keeper.h
)
target_link_libraries(monitor_keeper PRIVATE
    ${MK_LIBS})
target_compile_features(monitor_keeper PRIVATE cxx_std_17)

enable_testing()
add_executable(monitor_keeper_test
    test.cc
    monitor_keeper.h)
target_link_libraries(monitor_keeper_test
    ${MK_LIBS}
    GTest::gtest_main)
target_compile_features(monitor_keeper_test PRIVATE cxx_std_17)

