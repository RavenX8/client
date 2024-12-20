add_compiler_flags(-DASIO_STANDALONE)
add_compiler_flags(-Wall -Wextra)

if(DEBUG)
  add_definitions(-D_DEBUG -DDEBUG)
else()
  add_definitions(-DNDEBUG)
endif()

set(COPY_SCRIPT_PATH "")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(64BIT TRUE)
  message("-- detected 64bit")
else()
  set(64BIT FALSE)
  message("-- detected 32bit")
endif()