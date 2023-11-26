cmake_minimum_required(VERSION 3.20)
project(Cluster_Calculations_2023_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(Lab1/Lab1)
include_directories(Lab2/Lab2)
include_directories(Lab3/Lab3)

find_package(MPI REQUIRED)

add_executable(Cluster_Calculations
        BasicOperationMeasuring/BasicOperationMeasuring/BasicOperationMeasuring.cpp
        FormulaCalculator/FormulaCalculator/FormulaCalculator.cpp
        Lab1/Lab1/Lab1.cpp
        Lab1/Lab1/mpi_mvm.cpp
        Lab1/Lab1/mpi_mvm.h
        Lab1/Lab1/parallel_mvm.cpp
        Lab1/Lab1/parallel_mvm.h
        Lab1/Lab1/serial_mvm.cpp
        Lab1/Lab1/serial_mvm.h
        Lab1/Lab1/time_measuring.cpp
        Lab1/Lab1/time_measuring.h
        Lab1/Lab1/utilities.cpp
        Lab1/Lab1/utilities.h
        Lab2/Lab2/Lab2.cpp
        Lab2/Lab2/mpi_mm.cpp
        Lab2/Lab2/mpi_mm.h
        Lab2/Lab2/parallel_mm.cpp
        Lab2/Lab2/parallel_mm.h
        Lab2/Lab2/serial_mm.cpp
        Lab2/Lab2/serial_mm.h
        Lab2/Lab2/time_measuring.cpp
        Lab2/Lab2/time_measuring.h
        Lab2/Lab2/utilities.cpp
        Lab2/Lab2/utilities.h
        Lab3/Lab3/Lab3.cpp
        Lab3/Lab3/mpi_les.cpp
        Lab3/Lab3/mpi_les.h
        Lab3/Lab3/parallel_les.cpp
        Lab3/Lab3/parallel_les.h
        Lab3/Lab3/serial_les.cpp
        Lab3/Lab3/serial_les.h
        Lab3/Lab3/time_measuring.cpp
        Lab3/Lab3/time_measuring.h
        Lab3/Lab3/utilities.cpp
        Lab3/Lab3/utilities.h
        Lab4/Lab4/Lab4.cpp
        Lab4/Lab4/parallel_sort.cpp
        Lab4/Lab4/serial_sort.cpp
        Lab4/Lab4/time_measuring.cpp
        Lab4/Lab4/utilities.cpp
        Lab5/Lab5/Lab5.cpp
        Lab5/Lab5/parallel_graph.cpp
        Lab5/Lab5/serial_graph.cpp
        Lab5/Lab5/time_measuring.cpp
        Lab5/Lab5/utilities.cpp
        TestRunner/TestRunner/TestRunner.cpp)

target_link_libraries(Cluster_Calculations PUBLIC MPI::MPI_CXX)
