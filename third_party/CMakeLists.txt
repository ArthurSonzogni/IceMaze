include(FetchContent)

FetchContent_Declare(smk GIT_REPOSITORY https://github.com/ArthurSonzogni/smk)
FetchContent_GetProperties(smk)
if(NOT smk_POPULATED)
  FetchContent_Populate(smk)
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
