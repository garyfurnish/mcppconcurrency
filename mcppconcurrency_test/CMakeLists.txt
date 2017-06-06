include_directories(../mcppconcurrency/include/)
find_path(BANDIT_INCLUDE_PATH bandit/bandit.h PATHS ../../bandit)
include_directories(${BANDIT_INCLUDE_PATH})
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_compile_options(-fPIE)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_executable(mcppconcurrency_test
  main.cpp
  internal_conditional_variable.cpp
  )
target_link_libraries(mcppconcurrency_test mcppconcurrency mcpputil pthread)
