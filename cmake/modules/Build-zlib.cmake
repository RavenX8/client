# zlib 1.2.3 (in-tree) as a regular static library target, so it builds with
# whatever toolchain is active (MSVC, clang-cl cross, or native gcc/clang).
set(ZLIB_SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/zlib-1.2.3)

add_library(zlib STATIC
  ${ZLIB_SOURCE_DIR}/adler32.c
  ${ZLIB_SOURCE_DIR}/compress.c
  ${ZLIB_SOURCE_DIR}/crc32.c
  ${ZLIB_SOURCE_DIR}/deflate.c
  ${ZLIB_SOURCE_DIR}/gzio.c
  ${ZLIB_SOURCE_DIR}/inffast.c
  ${ZLIB_SOURCE_DIR}/inflate.c
  ${ZLIB_SOURCE_DIR}/inftrees.c
  ${ZLIB_SOURCE_DIR}/trees.c
  ${ZLIB_SOURCE_DIR}/uncompr.c
  ${ZLIB_SOURCE_DIR}/zutil.c
)
target_include_directories(zlib SYSTEM PUBLIC ${ZLIB_SOURCE_DIR})
set_target_properties(zlib PROPERTIES FOLDER 3rdparty POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  target_compile_options(zlib PRIVATE /w)
else()
  target_compile_options(zlib PRIVATE -w)
endif()

if(NOT TARGET utils::zlib)
  add_library(utils::zlib ALIAS zlib)
endif()

set(ZLIB_INCLUDE_DIR "${ZLIB_SOURCE_DIR}")
set(ZLIB_LIBRARIES zlib)
