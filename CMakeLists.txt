cmake_minimum_required(VERSION 3.29)
project(rayTracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(rayTracer main.cpp
        headers/vec3.h
        headers/colour.h
        headers/ray.h
        headers/hittable.h
        headers/hittable_list.h
        headers/rt_weekend.h
        headers/interval.h
        headers/camera.h
        headers/material.h
)
