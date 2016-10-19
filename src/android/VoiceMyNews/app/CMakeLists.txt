cmake_minimum_required(VERSION 3.4.1)

set(VOICEMYNEWS_ANDROID ./)
set(VOICEMYNEWS_CORE ../../../core)

include(../../../core/CMakeLists.txt)

include_directories(
  ${VOICEMYNEWS_ANDROID}/src/main/cpp
  ${VOICEMYNEWS_CORE}
)

add_library(
  voicemynews-android
  SHARED

  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopBinding.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/bindings/events/EventLoopBinding.cpp
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/utils/StringHelpers.h
  ${VOICEMYNEWS_ANDROID}/src/main/cpp/utils/StringHelpers.cpp
)

target_link_libraries(
  voicemynews-core voicemynews-android
)