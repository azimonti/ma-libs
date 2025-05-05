if(MSVC)
    if(BUILDSUITE)
        set(COPYDLLTESTBIN suite.test)
    else()
        set(COPYDLLTESTBIN ${TEST_BIN})
    endif()
    macro(TEST_FcopyDLL)
        ADD_CUSTOM_COMMAND( TARGET ${COPYDLLTESTBIN} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<$<CONFIG:Release>:../../../libtest/build/$<CONFIG>/test_f.dll>
            $<$<CONFIG:Debug>:../../../libtest/build/$<CONFIG>/test_fd.dll>
            ${CMAKE_BINARY_DIR}/../$<CONFIG>
            )
    endmacro()
endif()
