include(FetchContent)
if(NOT TARGET njr::algebra_finite_fields)
    FetchContent_Declare(
        njr_algebra_finite_fields
        GIT_REPOSITORY git@github.com:nathanjrussell/algebra-finite-fields.git
        GIT_TAG c1c229769b6382796fcbe90b23e3001b0e952646
    )
    FetchContent_MakeAvailable(njr_algebra_finite_fields)
endif()
