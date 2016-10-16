cmake_minimum_required(VERSION 3.4.1)

set(VOICEMYNEWS_CORE ./)

include_directories(
  ${VOICEMYNEWS_CORE}
)

add_library(
  voicemynews-core
  SHARED

  ${VOICEMYNEWS_CORE}/io/fs/exceptions/FileNotFoundException.cpp
  ${VOICEMYNEWS_CORE}/io/fs/FileUtils.cpp
  ${VOICEMYNEWS_CORE}/io/fs/Require.cpp
  ${VOICEMYNEWS_CORE}/voice/VoiceSupport.cpp)

target_link_libraries(
  voicemynews-core
)