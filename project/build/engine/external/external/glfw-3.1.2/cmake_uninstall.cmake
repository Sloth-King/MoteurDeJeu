
<<<<<<< HEAD
if (NOT EXISTS "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt\"")
endif()

file(READ "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt" files)
=======
if (NOT EXISTS "/home/andrew/Desktop/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/andrew/Desktop/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt\"")
endif()

file(READ "/home/andrew/Desktop/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt" files)
=======
if (NOT EXISTS "/home/abdellah/Documents/Programmation/Moteur de jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/abdellah/Documents/Programmation/Moteur de jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt\"")
endif()

file(READ "/home/abdellah/Documents/Programmation/Moteur de jeux/MoteurDeJeu/project/build/engine/external/external/glfw-3.1.2/install_manifest.txt" files)
>>>>>>> c0997a85b15fe58e23bfd2a822796427f0d6f916
>>>>>>> a44f5fbd5d83f03e3ade1e467f6da563d77d4209
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/usr/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/usr/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

