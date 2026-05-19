cmake_minimum_required(VERSION 3.20)

project(city_manager C)

set(CMAKE_C_STANDARD 11)

# -------------------- MAIN APPLICATION --------------------

add_executable(city_manager
        main.c
        operation.c
)

# -------------------- MONITOR PROCESS --------------------

add_executable(monitor
        monitor_reports.c
)

# -------------------- PHASE 3 HUB --------------------

add_executable(city_hub
        city_hub.c
)

# -------------------- SCORER WORKER --------------------

add_executable(scorer
        scorer.c
)