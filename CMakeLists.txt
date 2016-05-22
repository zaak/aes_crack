cmake_minimum_required(VERSION 2.8)
project(aes_crack)

enable_language(ASM_NASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall")

set(SOURCE_FILES aes.h aes.nasm)

add_executable(aes_crack main.c ${SOURCE_FILES})
