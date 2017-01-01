cmake_minimum_required(VERSION 3.4.1)

set(VOICEMYNEWS_TARGET_SOURCES "")

if(NOT VOICEMYNEWS_CORE)
  set(VOICEMYNEWS_CORE ./)
endif(NOT VOICEMYNEWS_CORE)

include_directories(
  ${VOICEMYNEWS_CORE}
)

list(APPEND VOICEMYNEWS_TARGET_SOURCES
  ${VOICEMYNEWS_CORE}/analytics/Analytics.h
  ${VOICEMYNEWS_CORE}/analytics/Analytics.cpp
  ${VOICEMYNEWS_CORE}/analytics/AnalyticsConstants.h
  ${VOICEMYNEWS_CORE}/analytics/WebBrowser.h
  ${VOICEMYNEWS_CORE}/config/Config.h
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
  ${VOICEMYNEWS_CORE}/voice/VoiceSupport.cpp)

if (DEV_BUILD)
  list(APPEND VOICEMYNEWS_TARGET_SOURCES
    ${VOICEMYNEWS_CORE}/config/ConfigDev.cpp)
elseif (PROD_BUILD)
  list(APPEND VOICEMYNEWS_TARGET_SOURCES
    ${VOICEMYNEWS_CORE}/config/ConfigProd.cpp)
endif (DEV_BUILD)

# ${VOICEMYNEWS_CORE}/config/ConfigDev.cpp
# ${VOICEMYNEWS_CORE}/config/ConfigProd.cpp

add_library(
  voicemynews-core
  SHARED

  ${VOICEMYNEWS_TARGET_SOURCES}
)

target_link_libraries(
  voicemynews-core
)