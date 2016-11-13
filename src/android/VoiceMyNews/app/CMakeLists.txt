cmake_minimum_required(VERSION 3.4.1)

set(VOICEMYNEWS_SUPPORTED_ARCHS x86 x86_64 arm64-v8a armeabi-v7a)
set(VOICEMYNEWS_TARGET_LIBS "")

set(VOICEMYNEWS_ANDROID ./)
set(VOICEMYNEWS_CORE ../../../core)

include(../../../core/CMakeLists.txt)

include_directories(
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/include
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/include/v8
  ${VOICEMYNEWS_ANDROID}/src/main/cpp
  ${VOICEMYNEWS_CORE}
)

add_library(
  voicemynews-android
  SHARED

  ${VOICEMYNEWS_ANDROID}/src/main/cpp/V8HelloWorld.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/V8HelloWorld.cpp
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventDataBinding.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventDataBinding.cpp
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopBinding.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopBinding.cpp
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopPlatform.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopPlatform.cpp
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/utils/StringHelpers.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/utils/StringHelpers.cpp
)

add_library(v8-android-libplatform SHARED IMPORTED GLOBAL)
add_library(v8-android-base STATIC IMPORTED GLOBAL)
add_library(v8-android-libbase STATIC IMPORTED GLOBAL)
add_library(v8-android-snapshot STATIC IMPORTED GLOBAL)
add_library(v8-android-libsampler STATIC IMPORTED GLOBAL)

if(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
    set(V8-ARCH arm64)
elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a")
    set(V8-ARCH arm)
elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "x86_64")
    set(V8-ARCH x86_64)
elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "x86")
    set(V8-ARCH ia32)
endif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")

set_target_properties(v8-android-libplatform PROPERTIES IMPORTED_LOCATION "/Users/cosnita/work-personal/voicemynews-deps/release/${V8-ARCH}/libv8_libplatform.a")
set_target_properties(v8-android-base PROPERTIES IMPORTED_LOCATION "/Users/cosnita/work-personal/voicemynews-deps/release/${V8-ARCH}/libv8_base.a")
set_target_properties(v8-android-libbase PROPERTIES IMPORTED_LOCATION "/Users/cosnita/work-personal/voicemynews-deps/release/${V8-ARCH}/libv8_libbase.a")
set_target_properties(v8-android-snapshot PROPERTIES IMPORTED_LOCATION "/Users/cosnita/work-personal/voicemynews-deps/release/${V8-ARCH}/libv8_nosnapshot.a")
set_target_properties(v8-android-libsampler PROPERTIES IMPORTED_LOCATION "/Users/cosnita/work-personal/voicemynews-deps/release/${V8-ARCH}/libv8_libsampler.a")

if(${CMAKE_ANDROID_ARCH_ABI} IN_LIST VOICEMYNEWS_SUPPORTED_ARCHS)
    list(APPEND VOICEMYNEWS_TARGET_LIBS
        "-Wl,--whole-archive"
        v8-android-libplatform
        v8-android-base
        v8-android-libbase
        v8-android-snapshot
        v8-android-libsampler
        "-Wl,--no-whole-archive"
    )
endif()

target_link_libraries(
  voicemynews-android
  voicemynews-core
  ${VOICEMYNEWS_TARGET_LIBS}
)