# Algebra matrices

Generic matrices over typed ring elements.

```cmake
include(FetchContent)

FetchContent_Declare(
    njr_algebra_matrices
    GIT_REPOSITORY git@github.com:nathanjrussell/algebra-matrices.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(njr_algebra_matrices)

target_link_libraries(my_app PRIVATE njr::algebra_matrices)
```
