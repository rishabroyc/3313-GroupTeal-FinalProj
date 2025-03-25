if(NOT EXISTS "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: /mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/install_manifest.txt")
endif()

file(READ "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
  if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "/usr/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
    endif()
  else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
  endif()
endforeach()

