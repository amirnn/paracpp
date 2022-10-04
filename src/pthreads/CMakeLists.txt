# pthreads

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(pthr pthr.cc)
target_link_libraries(pthr ${CONAN_LIBS} Threads::Threads)


add_executable(pthr3 pthr3.cc)
target_link_libraries(pthr3 ${CONAN_LIBS} Threads::Threads)