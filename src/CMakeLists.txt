add_library(algebra_matrices INTERFACE)
add_library(algebra::matrices ALIAS algebra_matrices)
target_compile_features(algebra_matrices INTERFACE cxx_std_20)
target_include_directories(algebra_matrices INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(algebra_matrices INTERFACE
    algebra::core
)
