# Algebra matrices

Generic matrices over typed ring elements.

```cmake
include(FetchContent)

FetchContent_Declare(
    algebra_matrices
    GIT_REPOSITORY git@github.com:nathanjrussell/algebra-matrices.git
    GIT_TAG v0.2.0
)

FetchContent_MakeAvailable(algebra_matrices)

target_link_libraries(my_app PRIVATE algebra::matrices)
```
