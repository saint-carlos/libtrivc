macro(install_header FILE_NAME)
	install(FILES ${FILE_NAME}
		COMPONENT headers
		DESTINATION include/trivc)
endmacro(install_header)

install_header(compiler.h)
install_header(trivc.h)
install_header(string.h)
install_header(atomic.h)
install_header(numeric.h)
install_header(buf.h)
install_header(stdcommon.h)

add_subdirectory(sys)
