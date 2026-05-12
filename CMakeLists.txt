cmake_minimum_required(VERSION 3.20)
project(city_manager C)

set(CMAKE_C_STANDARD 11)

add_executable(city_manager main.c operation.c)

add_executable(monitor monitor_reports.c)

add_executable(city_hub city_hub.c)

add_executable(scorer scorer.c)