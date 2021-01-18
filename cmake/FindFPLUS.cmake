

macro(DO_DOWNLOAD_FPLUS)

    include(ExternalProject)
    ExternalProject_Add(functional_plus
            GIT_REPOSITORY https://github.com/Dobiasd/FunctionalPlus.git
            GIT_TAG v0.2.10-p0

            SOURCE_DIR "${CMAKE_BINARY_DIR}/thirdparty/fplus"

            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""

            LOG_DOWNLOAD ON
            LOG_BUILD ON
            )

    ExternalProject_Get_Property(functional_plus install_dir)
    set(FPLUS_INCLUDE_DIRS ${install_dir}/include)
endmacro()