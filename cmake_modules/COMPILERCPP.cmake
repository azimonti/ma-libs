set(CMAKE_CXX_VISIBILITY_PRESET hidden )
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 14              )
set(CMAKE_CXX_EXTENSIONS OFF           )
set(CMAKE_CXX_STANDARD_REQUIRED ON     )
set(CMAKE_CXX_EXTENSIONS OFF           )
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 0  )

if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG       "${CMAKE_C_FLAGS_DEBUG}       /DDEBUG /D_DEBUG /Od /Zi /DLOGGING")
    set(CMAKE_CXX_FLAGS_DEBUG     "${CMAKE_CXX_FLAGS_DEBUG}     /DDEBUG /D_DEBUG /Od /Zi /DLOGGING")
    set(CMAKE_C_FLAGS_RELEASE     "${CMAKE_C_FLAGS_RELEASE}     /Ox /Ot /Oi /Ob2 /Oy /GS-")
    set(CMAKE_CXX_FLAGS_RELEASE   "${CMAKE_CXX_FLAGS_RELEASE}   /Ox /Ot /Oi /Ob2 /Oy /GS-")
else()
    set(CMAKE_C_FLAGS_DEBUG       "${CMAKE_C_FLAGS_DEBUG}       -DDEBUG -D_DEBUG -DLOGGING")
    set(CMAKE_CXX_FLAGS_DEBUG     "${CMAKE_CXX_FLAGS_DEBUG}     -DDEBUG -D_DEBUG -DLOGGING")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT SKIPBOUNDARYCHECKS)
        set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}    -fsanitize=undefined  -fsanitize=address")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -fsanitize=undefined  -fsanitize=address")
    endif()
endif()

if (MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/wd4459>)  # suppress "declaration of 'identifier' hides global declaration"
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/wd4251>)  # suppress "class 'type1' needs to have dll-interface to be used by clients of class"

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)          # suppress deprecation warning for old functions (e.g. strcpy)
    add_definitions( /fp:fast )                         # common params for FPU
    add_definitions( /MP )                              # multi-process compilation
elseif (APPLE)
    # disable OpenGL deprecation warnings
    add_definitions( -DGL_SILENCE_DEPRECATION )
    add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wno-c99-extensions)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-float-conversion>)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wconversion)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-float-conversion>)
endif()

if(NOUNITYBUILD)
    set(UNITYBUILD OFF)
else()
    set(UNITYBUILD ON )
endif()
