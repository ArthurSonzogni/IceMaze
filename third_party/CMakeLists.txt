include(FetchContent)
#set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

# ------------------------------------------------------------------------------
# Freetype library for loading fonts.
# ------------------------------------------------------------------------------
FetchContent_Declare(smk GIT_REPOSITORY git@github.com:arthursonzogni/smk)
FetchContent_GetProperties(smk)
if(NOT smk_POPULATED)
  FetchContent_Populate(smk)
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
