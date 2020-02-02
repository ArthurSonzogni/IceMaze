include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED FALSE)

FetchContent_Declare(smk
  GIT_REPOSITORY https://github.com/ArthurSonzogni/smk
  GIT_TAG 8c49563e7e9f2f89
)

FetchContent_GetProperties(smk)
if(NOT smk_POPULATED)
  FetchContent_Populate(smk)
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
