include(FetchContent)

# ---------------------------------------------------------------------------
# Forward ENABLE_SSL and DISABLE_CRYPT into the packets sub-project so that
# osIROSE-common-core compiles with the matching preprocessor definitions.
# Both flags must be set as CACHE BOOL FORCE before FetchContent_MakeAvailable
# so the sub-project's own option() declarations pick them up.
# ---------------------------------------------------------------------------
if(ENABLE_SSL)
  set(ENABLE_SSL ON CACHE BOOL "Enable TLS/SSL in osIROSE-common-core" FORCE)
else()
  set(ENABLE_SSL OFF CACHE BOOL "Enable TLS/SSL in osIROSE-common-core" FORCE)
endif()

if(DISABLE_CRYPT)
  set(DISABLE_CRYPT ON CACHE BOOL "Disable XOR encryption in osIROSE-common-core" FORCE)
else()
  set(DISABLE_CRYPT OFF CACHE BOOL "Disable XOR encryption in osIROSE-common-core" FORCE)
endif()

# ---------------------------------------------------------------------------
# Fetch the packets project.
# For local development you can bypass the git download by setting:
#   -DFETCHCONTENT_SOURCE_DIR_OSIROSE_COMMON=/path/to/local/packets
# on the CMake command line.
# ---------------------------------------------------------------------------
FetchContent_Declare(
        osirose_common
        GIT_REPOSITORY https://github.com/dev-osrose/packets
        GIT_TAG idl-module-updates
)

FetchContent_MakeAvailable(osirose_common)

# Create a clean umbrella interface target
if(NOT TARGET osirose_common)
  add_library(osirose_common INTERFACE)

  target_link_libraries(osirose_common
          INTERFACE
          osIROSE-common-core
          osIROSE-common-rosecommon
  )

  add_library(osirose_common::osirose_common
          ALIAS osirose_common
  )
endif()
