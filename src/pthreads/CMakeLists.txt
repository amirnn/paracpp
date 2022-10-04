# pthreads

add_executable(pthr pthr.cc)
target_link_libraries(pthr ${CONAN_LIBS} Threads::Threads)


add_executable(pthr3 pthr3.cc)
target_link_libraries(pthr3 ${CONAN_LIBS} Threads::Threads)