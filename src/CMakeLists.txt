configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h @ONLY)

# Hacker typer project
add_executable(hacker-typer main.cpp)
if (WIN32)
target_link_libraries(
  hacker-typer
  PRIVATE project_options
          project_warnings)
else()
target_link_libraries(
  hacker-typer
  PRIVATE project_options
          project_warnings
          ncurses)
endif()

