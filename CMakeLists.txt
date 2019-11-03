cmake_minimum_required (VERSION 3.11)

add_subdirectory(third_party)

project(IceMazeWeb
  LANGUAGES CXX
  VERSION 0.1.0
)

add_executable(icemaze
  src/Level.cpp
  src/Level.hpp
  src/LevelGenerator.cpp
  src/LevelGenerator.hpp
  src/License.hpp
  src/activity/Activity.hpp
  src/activity/Credit.cpp
  src/activity/Credit.hpp
  src/activity/IntroScreen.cpp
  src/activity/IntroScreen.hpp
  src/activity/LevelActivity.cpp
  src/activity/LevelActivity.hpp
  src/activity/LevelExplorer.cpp
  src/activity/LevelExplorer.hpp
  src/activity/LevelGeneratorActivity.cpp
  src/activity/LevelGeneratorActivity.hpp
  src/activity/LevelGeneratorSizeSelector.cpp
  src/activity/LevelGeneratorSizeSelector.hpp
  src/activity/Menu.cpp
  src/activity/Menu.hpp
  src/main.cpp
  src/resources.cpp
  src/resources.hpp
  src/util/auto_reset.hpp
)
target_include_directories(icemaze PRIVATE ${CMAKE_CURRENT_DIR}/src)
target_include_directories(icemaze PRIVATE src)
target_link_libraries(icemaze smk)
set_target_properties(icemaze PROPERTIES CXX_STANDARD 17)

# Detect emscripten is used.
if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$") 

  # Analytics.
  option(ADD_GOOGLE_ANALYTICS "Add google analytics script" ON)
  if (ADD_GOOGLE_ANALYTICS)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/google-analytics.html google-analytics)
  endif(ADD_GOOGLE_ANALYTICS)

  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -s DEMANGLE_SUPPORT=1")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -s DISABLE_EXCEPTION_CATCHING=2")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -s WASM=1")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -s TOTAL_MEMORY=134217728")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -s BINARYEN_TRAP_MODE='allow'")

  # Release flags
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " --closure 1")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " --js-opts 3")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " --llvm-lto 3")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " --llvm-opts 3")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -O3")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -flto")
  set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " -fno-exceptions")

  # Copy the index.html file.
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/index.html
    ${CMAKE_CURRENT_BINARY_DIR}/index.html
  )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/style.css
    ${CMAKE_CURRENT_BINARY_DIR}/style.css
  )

  # Allow some files to be fetched.
  file(GLOB_RECURSE files "./resources/**")
  foreach(file ${files})
    file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} "${file}")
    set_property(TARGET icemaze APPEND_STRING PROPERTY LINK_FLAGS " --preload-file \"${file}@/${relative_file}\"")
    MESSAGE(STATUS " --preload-file \"${file}@/${relative_file}\"")
  endforeach()
endif()

install(TARGETS icemaze RUNTIME DESTINATION "bin")
install(DIRECTORY resources DESTINATION share/icemaze)
