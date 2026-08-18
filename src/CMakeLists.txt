add_library(njr_algebra_matrices INTERFACE)
add_library(njr::algebra_matrices ALIAS njr_algebra_matrices)
target_compile_features(njr_algebra_matrices INTERFACE cxx_std_20)
target_include_directories(njr_algebra_matrices INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(njr_algebra_matrices INTERFACE
    njr::algebra_core
)
