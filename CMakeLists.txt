cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "svbb")
set(CURRENT_VERSION 0.1)
project(${PROJECT_NAME} VERSION ${CURRENT_VERSION} LANGUAGES CXX)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(INCLUDE_FILES 
    "${INCLUDE_DIR}/svbb.hpp"
)
set(TEST_FILES
	"${TEST_DIR}/svbb.t.cpp"
)

add_library(${PROJECT_NAME} INTERFACE)
add_library(svbb::svbb ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if(BUILD_TESTS)
	enable_testing()
    add_executable("${PROJECT_NAME}_test" ${INCLUDE_FILES} ${TEST_FILES} "${TEST_DIR}/config.hpp")
	target_link_libraries("${PROJECT_NAME}_test" PRIVATE svbb::svbb)

    if(SVBB_USE_BOOST_SPIRIT_CONVERSIONS)
        find_package(Boost 1.66 REQUIRED QUIET)
        target_link_libraries("${PROJECT_NAME}_test" PRIVATE Boost::boost)
        target_compile_definitions("${PROJECT_NAME}_test" PRIVATE SVBB_USE_BOOST_SPIRIT_CONVERSIONS)
    endif()

    set(SVBB_NO_CXX14_CONSTEXPR ON)
	if(MSVC)
		target_compile_options("${PROJECT_NAME}_test" PRIVATE "/W4" "/WX")

        if(MSVC_VERSION GREATER_EQUAL 1910)
            target_compile_features("${PROJECT_NAME}_test" PRIVATE cxx_std_17)
            target_compile_options("${PROJECT_NAME}_test" PRIVATE "/permissive-" "/std:c++17")
            target_compile_definitions("${PROJECT_NAME}_test" PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
            set(SVBB_NO_CXX14_CONSTEXPR OFF)
        elseif(MSVC_VERSION GREATER_EQUAL 1700)
            target_compile_features("${PROJECT_NAME}_test" PRIVATE cxx_std_11)
            set(SVBB_USE_STX_STRING_VIEW ON)
        else()
            message(ERROR "Unsupported MSCV version.")
        endif()
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		target_compile_options("${PROJECT_NAME}_test" PRIVATE "-Wall" "-Werror" "-pedantic")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
            target_compile_features("${PROJECT_NAME}_test" PRIVATE cxx_std_17)
            set(SVBB_NO_CXX14_CONSTEXPR OFF)
        elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.8)
            target_compile_features("${PROJECT_NAME}_test" PRIVATE cxx_std_11)
            set(SVBB_USE_STX_STRING_VIEW ON)
        else()
            message(ERROR "Unsupported GCC version.")
        endif()
    else()
        message(ERROR "Unsupported compiler.")
	endif()

    if(SVBB_NO_CXX14_CONSTEXPR)
        target_compile_definitions("${PROJECT_NAME}_test" PRIVATE SVBB_NO_CXX14_CONSTEXPR)
    endif()
   
    if(SVBB_USE_BOOST_STRING_VIEW)
        target_compile_definitions("${PROJECT_NAME}_test" PRIVATE SVBB_USE_BOOST_STRING_VIEW)
    elseif(SVBB_USE_STX_STRING_VIEW)
        target_compile_definitions("${PROJECT_NAME}_test" PRIVATE SVBB_USE_STX_STRING_VIEW)
        target_include_directories("${PROJECT_NAME}_test" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern)
    endif()

	add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
endif()
