# SQLite amalgamation (in-tree) as a regular static library target.
set(SQLITE3_SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/sqlite3)

add_library(sqlite3 STATIC ${SQLITE3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 SYSTEM PUBLIC ${SQLITE3_SOURCE_DIR})
set_target_properties(sqlite3 PROPERTIES FOLDER 3rdparty OUTPUT_NAME libsqlite3 POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  target_compile_options(sqlite3 PRIVATE /w)
else()
  target_compile_options(sqlite3 PRIVATE -w)
  find_package(Threads REQUIRED)
  target_link_libraries(sqlite3 PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
endif()

if(NOT TARGET sqlite3::sqlite3)
  add_library(sqlite3::sqlite3 ALIAS sqlite3)
endif()

set(SQLITE3_INCLUDE_DIR "${SQLITE3_SOURCE_DIR}")
set(SQLITE3_LIBRARIES sqlite3)
