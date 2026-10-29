include(FetchContent)
if(NOT TARGET algebra::finite_fields)
    FetchContent_Declare(
        algebra_finite_fields
        GIT_REPOSITORY git@github.com:nathanjrussell/algebra-finite-fields.git
        GIT_TAG 74b3d569fb48818f4c10f3c32889f2128485f1a7
    )
    FetchContent_MakeAvailable(algebra_finite_fields)
endif()
