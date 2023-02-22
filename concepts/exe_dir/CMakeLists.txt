cmake_minimum_required(VERSION 3.11)

project(concept_exedir LANGUAGES C Fortran)

file(GENERATE OUTPUT .gitignore CONTENT "*")

add_executable(exe_dir realpath.c main.f90)
