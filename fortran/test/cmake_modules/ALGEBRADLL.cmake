if(MSVC)
    if(BUILDSUITE)
        set(COPYDLLBIN suite.test)
    else()
        set(COPYDLLBIN ${TEST_BIN})
    endif()
    macro(${TEST_PROJECT_NAME}copyDLL)
        ADD_CUSTOM_COMMAND( TARGET ${COPYDLLBIN} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<$<CONFIG:Release>:../../../../libalgebra/build/$<CONFIG>/algebra_f.dll>
            $<$<CONFIG:Debug>:../../../../libalgebra/build/$<CONFIG>/algebra_fd.dll>
            ${CMAKE_BINARY_DIR}/../../$<CONFIG>
            )
    endmacro()
endif()
