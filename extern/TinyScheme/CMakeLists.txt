cmake_minimum_required (VERSION 3.1)
project (tinyscheme)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# remove 'lib' prefix for dlls 
if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# ----------------------------------------------------------------------------

set (TINYSCHEME_DIR "src")
set (RE_MODULE_DIR "src/re")

# ----------------------------------------------------------------------------

set (TINYSCHEME_SRCS
	${TINYSCHEME_DIR}/scheme.c
	${TINYSCHEME_DIR}/api.c
	${TINYSCHEME_DIR}/dynload.c
)

set(TINYSCHEME_APP_SRCS
	${TINYSCHEME_DIR}/main.c
)

if (WIN32)

	set(TINYSCHEME_DEFS
		USE_STRLWR=0 USE_DL=1 USE_MATH=1 USE_ASCII_NAMES=0
	)

if (MSVC)
	set(TINYSCHEME_DEFS ${TINYSCHEME_DEFS} _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
endif(MSVC)

else(WIN32)
	set(TINYSCHEME_DEFS
		SUN_DL=1 USE_DL=1 USE_MATH=1 USE_ASCII_NAMES=0
	)
endif(WIN32)

# ----------------------------------------------------------------------------

add_library (tinyscheme SHARED ${TINYSCHEME_SRCS})
target_compile_definitions(tinyscheme PUBLIC ${TINYSCHEME_DEFS})

if (0)
add_executable(tinyscheme-app ${TINYSCHEME_APP_SRCS})
if (WIN32)
target_link_libraries(tinyscheme-app tinyscheme)
else (WIN32)
target_link_libraries(tinyscheme-app tinyscheme m dl)
endif (WIN32)
target_compile_definitions(tinyscheme-app PUBLIC ${TINYSCHEME_DEFS})
set_target_properties (tinyscheme-app PROPERTIES OUTPUT_NAME tinyscheme)

# ----------------------------------------------------------------------------

set(RE_MODULE_SRCS
	${RE_MODULE_DIR}/re.c 
	${RE_MODULE_DIR}/debug.c 
	${RE_MODULE_DIR}/regcomp.c 
	${RE_MODULE_DIR}/regerror.c 
	${RE_MODULE_DIR}/regexec.c 
	${RE_MODULE_DIR}/regfree.c 
	${RE_MODULE_DIR}/split.c
)

add_library(tinyscheme-re SHARED ${RE_MODULE_SRCS})
target_link_libraries(tinyscheme-re tinyscheme)
target_include_directories (tinyscheme-re PUBLIC ${TINYSCHEME_DIR} PUBLIC ${RE_MODULE_DIR})
target_compile_definitions(tinyscheme-re PUBLIC ${TINYSCHEME_DEFS})
set_target_properties (tinyscheme-re PROPERTIES OUTPUT_NAME re)
endif()

# ----------------------------------------------------------------------------
