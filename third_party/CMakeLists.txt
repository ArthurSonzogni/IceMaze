include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED FALSE)

FetchContent_Declare(smk
  GIT_REPOSITORY https://github.com/ArthurSonzogni/smk
  GIT_TAG 1252746a98ac9473fdaf1ab64028aa13cf111a0d
)

FetchContent_GetProperties(smk)
if(NOT smk_POPULATED)
  FetchContent_Populate(smk)
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
