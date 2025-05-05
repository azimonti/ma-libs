if(MSVC)
    if(BUILDSUITE)
        set(COPYDLLBIN suite.test)
    else()
        set(COPYDLLBIN ${TEST_BIN})
    endif()
    macro(${TEST_PROJECT_NAME}copyDLL)
        ADD_CUSTOM_COMMAND( TARGET ${COPYDLLBIN} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<$<CONFIG:Release>:${BIN_DIR}/$<CONFIG>/hdf5.dll>
            $<$<CONFIG:Debug>:${BIN_DIR}/$<CONFIG>/hdf5_D.dll>
            ${CMAKE_SOURCE_DIR}/build/$<CONFIG>
            )
        ADD_CUSTOM_COMMAND( TARGET ${COPYDLLBIN} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<$<CONFIG:Release>:${BIN_DIR}/$<CONFIG>/hdf5_fortran.dll>
            $<$<CONFIG:Debug>:${BIN_DIR}/$<CONFIG>/hdf5_fortran_D.dll>
            ${CMAKE_SOURCE_DIR}/build/$<CONFIG>
            )
        ADD_CUSTOM_COMMAND( TARGET ${COPYDLLBIN} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<$<CONFIG:Release>:${BIN_DIR}/$<CONFIG>/hdf5_f90cstub.dll>
            $<$<CONFIG:Debug>:${BIN_DIR}/$<CONFIG>/hdf5_f90cstub_D.dll>
            ${CMAKE_SOURCE_DIR}/build/$<CONFIG>
            )
    endmacro()
endif()
