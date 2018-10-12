# There's almost certainlyt a much better solution than this.

find_package(Python3)

function(install_python_module_files path_to_module)
    if(Python3_EXECUTABLE)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import distutils.sysconfig as s; print(s.get_python_lib(True,False,'${CMAKE_INSTALL_PREFIX}'))"
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE _python_lib_path)

        if(NOT _python_lib_path)
            message(FATAL_ERROR "Could not determine python module installation path")
        endif()
    else()
        message(FATAL_ERROR "Could not find Python 3 interpreter")
    endif()

    install(
        DIRECTORY "${path_to_module}"
        DESTINATION "${_python_lib_path}"
        FILES_MATCHING PATTERN "*.so" PATTERN "*.py")
endfunction()

