cmake_minimum_required(VERSION 3.28)
project(core_engine_debugger C)

set(CMAKE_C_STANDARD 11)
# DEBUG_XCODE: 探索結果の文字コード変換に必要
add_definitions(-DEXP_C99_FIXED_WIDTH_INTEGER_TYPES=1 -DDEBUG_XCODE__=1)

# core_engine のライブラリ群を読み込む
set(CORE_ENGINE_LIB_DIR "../core_engine/lib")
# lib/Platform
set(PLATFORM ${CORE_ENGINE_LIB_DIR}/Platform)
set(PLATFORM_SRC ${PLATFORM}/Sources)
set(PLATFORM_PUBLIC_HEADERS ${PLATFORM}/PublicHeaders)

include_directories(${PLATFORM_PUBLIC_HEADERS})

# lib/ExpLib
set(EXPLIB ${CORE_ENGINE_LIB_DIR}/ExpLib)
set(EXPLIB_SRC ${EXPLIB}/Sources)
set(EXPLIB_HEADERS ${EXPLIB}/Headers)
set(EXPLIB_PUBLIC_HEADERS ${EXPLIB}/PublicHeaders)

include_directories(${EXPLIB_HEADERS})
include_directories(${EXPLIB_PUBLIC_HEADERS})

# lib/ExpDiaLib
set(EXPDIALIB ${CORE_ENGINE_LIB_DIR}/ExpDiaLib)
set(EXPDIALIB_SRC ${EXPDIALIB}/Sources)
set(EXPDIALIB_VIEW_SRC ${EXPDIALIB}/ViewSources)
set(EXPDIALIB_HEADERS ${EXPDIALIB}/Headers)
set(EXPDIALIB_PUBLIC_HEADERS ${EXPDIALIB}/PublicHeaders)
set(EXPDIALIB_VIEW_HEADERS ${EXPDIALIB}/ViewHeaders)

include_directories(${EXPDIALIB_HEADERS})
include_directories(${EXPDIALIB_PUBLIC_HEADERS})
include_directories(${EXPDIALIB_VIEW_HEADERS})

# lib/ExpToku
set(EXPTOKU ${CORE_ENGINE_LIB_DIR}/ExpToku)
set(EXPTOKU_SRC ${EXPTOKU}/Sources)
set(EXPTOKU_HEADERS ${EXPTOKU}/Headers)
set(EXOTOKU_PUBLIC_HEADERS ${EXPTOKU}/PublicHeaders)

include_directories(EXPTOKU_HEADERS)
include_directories(EXOTOKU_PUBLIC_HEADERS)

# lib/DispLineLib
set(DISPLINELIB ${CORE_ENGINE_LIB_DIR}/DispLineLib)

include_directories(${DISPLINELIB})

# build 対象のソースコードを追加
file(GLOB PLATFORM ${PLATFORM_SRC}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${PLATFORM})

file(GLOB EXPLIB ${EXPLIB_SRC}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${EXPLIB})

file(GLOB EXPDIALIB ${EXPDIALIB_SRC}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${EXPDIALIB})
file(GLOB EXODIALIB_VIEW ${EXPDIALIB_VIEW_SRC}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${EXODIALIB_VIEW})

file(GLOB EXPTOKU ${EXPTOKU_SRC}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${EXPTOKU_SRC})

file(GLOB DISPLINELIB ${DISPLINELIB}/*.c)
set(CORE_ENGINE_FILES ${CORE_ENGINE_FILES} ${DISPLINELIB}
        main.c
        initialize_exp_database.h
        initialize_exp_database.c
        exp_configure.h
        common.h
        common.c)

# Build 対象のプログラムを指定
add_executable(core_engine_debugger main.c ${CORE_ENGINE_FILES})

# 必要なライブラリ類を指定
target_link_libraries(core_engine_debugger
        iconv
)