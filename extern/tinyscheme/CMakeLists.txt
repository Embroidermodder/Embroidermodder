cmake_minimum_required(VERSION 2.8)
project(tinyscheme)

find_package(Threads)

if (NOT MSVC)
add_definitions(-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast)
endif ()

# special flags for MSVC
if (MSVC OR "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif()

add_executable(tinyscheme_repl test/repl.c source/dynload.c)
add_executable(tinyscheme_test test/test.c)

include_directories(include
	node_modules/zpl.c/include
	node_modules/zpl_json.c/include)

add_library(tinyscheme_static STATIC source/scheme.c source/dynload.c)
add_library(tinyscheme SHARED source/scheme.c source/dynload.c)

target_link_libraries(tinyscheme                        ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(tinyscheme_repl                   ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(tinyscheme_test tinyscheme_static ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (UNIX)
	target_link_libraries(tinyscheme                        m)
	target_link_libraries(tinyscheme_repl                   m)
	target_link_libraries(tinyscheme_test tinyscheme_static m)
endif()

target_compile_definitions(tinyscheme_static PRIVATE USE_INTERFACE=1 STANDALONE=0)
target_compile_definitions(tinyscheme        PRIVATE USE_INTERFACE=1 STANDALONE=0)