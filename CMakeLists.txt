# FindICU requires 3.7
cmake_minimum_required(VERSION 3.7)

set(CMAKE_SUPPRESS_REGENERATION true)

# use ICU_ROOT
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Project settings
project(test_icu LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(ICU REQUIRED COMPONENTS i18n uc)

add_executable(test_icu test_icu.cpp)
target_link_libraries(test_icu ICU::i18n ICU::uc)

# Testing

enable_testing()

add_test(NAME test_icu
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_icu)
