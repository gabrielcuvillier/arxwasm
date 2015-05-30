set(NACL 1)
set(PNACL 1)

set(PLATFORM_NAME	          "PNaCl")				     
set(PLATFORM_C_COMPILER     "clang")				      
set(PLATFORM_CXX_COMPILER   "clang++")			       
set(PLATFORM_TOOLCHAIN	    "linux_pnacl")		  
set(PLATFORM_ARCH           "pnacl")		
set(PLATFORM_ARCH_INTERNAL  "le32-nacl")
set(PLATFORM_EXE_SUFFIX     ".pexe")				    
set(PLATFORM_SDK_ROOT       "$ENV{NACL_SDK_ROOT}")
set(PLATFORM_SDK_INC_DIR    "${PLATFORM_SDK_ROOT}/include")
set(PLATFORM_SDK_LIB_DIR    "${PLATFORM_SDK_ROOT}/lib/${PLATFORM_ARCH}/Release")
set(PLATFORM_TOOLCHAIN_DIR  "${PLATFORM_SDK_ROOT}/toolchain/${PLATFORM_TOOLCHAIN}")
set(PLATFORM_ROOT_DIR       "${PLATFORM_TOOLCHAIN_DIR}/${PLATFORM_ARCH_INTERNAL}")

set(CMAKE_SYSTEM_NAME       "Linux")
set(CMAKE_SYSTEM_PROCESSOR  "${PLATFORM_ARCH_INTERNAL}")
set(CMAKE_FIND_ROOT_PATH 	  "${PLATFORM_ROOT_DIR}" )
set(CMAKE_AR                "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-ar")
set(CMAKE_RANLIB            "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-ranlib")
set(CMAKE_STRIP             "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-strip")
set(CMAKE_C_COMPILER        "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-${PLATFORM_C_COMPILER}")
set(CMAKE_CXX_COMPILER      "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-${PLATFORM_CXX_COMPILER}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Directories
include_directories(${PLATFORM_SDK_INC_DIR})

# Force include directory of FreeType on NACL (it is not clear why FreeType is not detected without this)
set(FREETYPE_INCLUDE_DIRS	${PLATFORM_ROOT_DIR}/usr/include/freetype2)

# Regal library is used as a wrapper to OPENGL ES 2
set(OPENGL_INCLUDE_DIR 		${PLATFORM_ROOT_DIR}/usr/include)
set(OPENGL_gl_LIBRARY    	${PLATFORM_ROOT_DIR}/usr/lib/libRegal.a)
set(OPENGL_LIBRARIES	    ${PLATFORM_ROOT_DIR}/usr/lib/libglslopt.a) # this is required too

# PPAPI libraries
set(PPAPI_SIMPLE_LIBRARY  "-Wl,-undefined=PSUserMainGet" "${PLATFORM_SDK_LIB_DIR}/libppapi_simple.a")
set(NACL_IO_LIBRARY       "${PLATFORM_SDK_LIB_DIR}/libnacl_io.a")
set(PPAPI_GLES2_LIBRARY   "${PLATFORM_SDK_LIB_DIR}/libppapi_gles2.a")
set(PPAPI_CPP_LIBRARY     "${PLATFORM_SDK_LIB_DIR}/libppapi_cpp.a")
set(PPAPI_LIBRARY         "-lppapi")

macro(pnacl_finalize target)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMENT "Finalizing ${target} to ${target}.pexe"
    COMMAND "cp" "$<TARGET_FILE:${target}>" "$<TARGET_FILE:${target}>_unstripped.bc"
    COMMAND "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-finalize" "$<TARGET_FILE:${target}>" "-o"  "$<TARGET_FILE:${target}>${PLATFORM_EXE_SUFFIX}")
endmacro()

macro(pnacl_compress target)
  add_custom_target(pnacl-compress
    COMMAND "${PLATFORM_TOOLCHAIN_DIR}/bin/${PLATFORM_ARCH}-compress" "$<TARGET_FILE:${target}>${PLATFORM_EXE_SUFFIX}")
endmacro()
