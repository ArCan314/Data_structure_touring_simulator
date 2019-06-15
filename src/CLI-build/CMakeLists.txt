cmake_minimum_required (VERSION 3.10.2)

project (Touring_Simulaor_CLI)

aux_source_directory(../headers HEADERS)

add_compile_options(-std=c++11 -O3 -Wall)
message(STATUS "optional: -std=c++11 -O3 -Wall")

add_executable(Touring_Simulaor_CLI ${HEADERS} ./main.cc)