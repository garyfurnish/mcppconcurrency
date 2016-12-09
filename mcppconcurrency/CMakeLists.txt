include_directories(include)
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_compile_options(-fPIC)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
list (APPEND SRC_FILES
  src/accumulator_monoid.cpp
  src/aligned_monoid.cpp
  src/main.cpp
)
add_library(mcppconcurrency ${SRC_FILES})
INSTALL(DIRECTORY "include/mcppconcurrency" DESTINATION "include")
INSTALL(TARGETS mcppconcurrency
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib)
