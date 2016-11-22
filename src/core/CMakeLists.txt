cmake_minimum_required(VERSION 3.4.1)

if(NOT VOICEMYNEWS_CORE)
  set(VOICEMYNEWS_CORE ./)
endif(NOT VOICEMYNEWS_CORE)

include_directories(
  ${VOICEMYNEWS_CORE}
)

add_library(
  voicemynews-core
  SHARED

  ${VOICEMYNEWS_CORE}/analytics/Analytics.h
  ${VOICEMYNEWS_CORE}/events/EventData.h
  ${VOICEMYNEWS_CORE}/events/EventLoop.h
  ${VOICEMYNEWS_CORE}/events/EventNames.h
  ${VOICEMYNEWS_CORE}/io/fs/exceptions/FileNotFoundException.h
  ${VOICEMYNEWS_CORE}/io/fs/exceptions/FileNotFoundException.cpp
  ${VOICEMYNEWS_CORE}/io/fs/FileUtils.h
  ${VOICEMYNEWS_CORE}/io/fs/FileUtils.cpp
  ${VOICEMYNEWS_CORE}/io/fs/Require.h
  ${VOICEMYNEWS_CORE}/io/fs/Require.cpp
  ${VOICEMYNEWS_CORE}/network/HttpClientInterface.h
  ${VOICEMYNEWS_CORE}/voice/VoiceSupport.h
  ${VOICEMYNEWS_CORE}/voice/VoiceSupport.cpp
)

target_link_libraries(
  voicemynews-core
)