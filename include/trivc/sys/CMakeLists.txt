macro(install_sys_header FILE_NAME)
	install(FILES ${FILE_NAME}
		COMPONENT headers
		DESTINATION include/trivc/sys)
endmacro(install_sys_header)

install_sys_header(compiler.h)
install_sys_header(trivc.h)
install_sys_header(string.h)
install_sys_header(numeric.h)
