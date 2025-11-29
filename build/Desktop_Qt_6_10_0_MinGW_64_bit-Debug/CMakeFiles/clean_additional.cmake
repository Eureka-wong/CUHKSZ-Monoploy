# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CUHKSZ_Tycoon_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CUHKSZ_Tycoon_autogen.dir\\ParseCache.txt"
  "CUHKSZ_Tycoon_autogen"
  )
endif()
