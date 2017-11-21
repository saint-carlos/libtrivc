install(FILES compiler.h DESTINATION include/trivc)
install(FILES trivc.h DESTINATION include/trivc)
install(FILES string.h DESTINATION include/trivc)
install(FILES atomic.h DESTINATION include/trivc)

add_subdirectory(sys)
