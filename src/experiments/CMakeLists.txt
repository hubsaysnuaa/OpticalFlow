 set(app_name cross_correlation)
  # Celero benchmark
add_executable(${app_name}_benchmark overlap_add.cpp ${app_name}_benchmark.cpp)
target_link_libraries(${app_name}_benchmark
  celero
  ${OpenCV_LIBS})

set(app_name overlap_add)
 # google test
add_executable(${app_name}_tests tests/${app_name}_tests.cpp overlap_add.cpp ${app_name}.cpp)
target_link_libraries(${app_name}_tests
  gtest gtest_main
  ${OpenCV_LIBS})
add_test(NAME ${app_name}_tests  COMMAND ${app_name}_tests)

add_executable(${app_name}_benchmark tests/${app_name}_benchmark.cpp)
target_link_libraries(${app_name}_benchmark
  celero
  ${OpenCV_LIBS})

set(app_name dft)
add_executable(${app_name}_benchmark tests/${app_name}_benchmark.cpp)
target_link_libraries(${app_name}_benchmark
  celero
  ${OpenCV_LIBS})

set(app_name optical_flow_mat_benchmark)
add_executable(${app_name} ${app_name}.cpp)
target_link_libraries(${app_name}
  celero
  ${OpenCV_LIBS})
