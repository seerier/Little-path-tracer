cmake_minimum_required(VERSION 3.15)

project(RayTracingWeekend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#add_executable(RayTW "src/InOneWeekend/main.cpp")
#set_target_properties(RayTW PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OneWeekend_build)

#add_executable(RayNW "src/TheNextWeek/main.cpp")
#set_target_properties(RayNW PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/NextWeek_build)
#target_include_directories(RayNW PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/external)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/TheRestOfYourLife/base ${CMAKE_CURRENT_SOURCE_DIR}/src/external)
add_executable(RayRL "src/TheRestOfYourLife/cmd/main.cpp")

add_executable(pi "src/TheRestOfYourLife/cmd/pi.cpp")
add_executable(integrate_x_sq "src/TheRestOfYourLife/cmd/integrate_x_sq.cpp")
add_executable(estimate_halfway "src/TheRestOfYourLife/cmd/estimate_halfway.cpp")
add_executable(sphere_importance "src/TheRestOfYourLife/cmd/sphere_importance.cpp")
add_executable(sphere_plot "src/TheRestOfYourLife/cmd/sphere_plot.cpp")
add_executable(cos_cubed "src/TheRestOfYourLife/cmd/cos_cubed.cpp")
add_executable(cos_density "src/TheRestOfYourLife/cmd/cos_density.cpp")