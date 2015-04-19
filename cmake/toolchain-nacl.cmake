set(NACL 1)

set(PLATFORM_NAME	          "NaCl")				     
set(PLATFORM_C_COMPILER     "gcc")				      
set(PLATFORM_CXX_COMPILER   "g++")			       
set(PLATFORM_TOOLCHAIN	    "linux_x86_newlib")		  
set(PLATFORM_ARCH           "x86_64-nacl")			
set(PLATFORM_EXE_SUFFIX     ".nexe")				    
set(PLATFORM_SDK_ROOT       "$ENV{NACL_SDK_ROOT}")
set(PLATFORM_SDK_INCLUDEDIR "${PLATFORM_SDK_ROOT}/include")
set(PLATFORM_SDK_LIBDIR     "${PLATFORM_SDK_ROOT}/lib/newlib_x86_64/Release")
set(PLATFORM_PREFIX         "${PLATFORM_SDK_ROOT}/toolchain/${PLATFORM_TOOLCHAIN}")

set(CMAKE_SYSTEM_NAME       "Linux")
set(CMAKE_SYSTEM_PROCESSOR  "x86_64")
set(CMAKE_FIND_ROOT_PATH 	  "${PLATFORM_PREFIX}/${PLATFORM_ARCH}" )
set(CMAKE_AR                "${PLATFORM_PREFIX}/bin/${PLATFORM_ARCH}-ar")
set(CMAKE_RANLIB            "${PLATFORM_PREFIX}/bin/${PLATFORM_ARCH}-ranlib")
set(CMAKE_STRIP             "${PLATFORM_PREFIX}/bin/${PLATFORM_ARCH}-strip")
set(CMAKE_C_COMPILER        "${PLATFORM_PREFIX}/bin/${PLATFORM_ARCH}-${PLATFORM_C_COMPILER}")
set(CMAKE_CXX_COMPILER      "${PLATFORM_PREFIX}/bin/${PLATFORM_ARCH}-${PLATFORM_CXX_COMPILER}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Directories

include_directories(${PLATFORM_SDK_INCLUDEDIR})

# Force include directory of FreeType on NACL (it is not clear why FreeType is not detected without this)
set(FREETYPE_INCLUDE_DIRS	${PLATFORM_PREFIX}/${PLATFORM_ARCH}/usr/include/freetype2)

# Regal library is used as a wrapper to OPENGL ES 2
set(OPENGL_INCLUDE_DIR 		${PLATFORM_PREFIX}/${PLATFORM_ARCH}/usr/include)
set(OPENGL_gl_LIBRARY    	${PLATFORM_PREFIX}/${PLATFORM_ARCH}/usr/lib/libRegal.a)
set(OPENGL_LIBRARIES	    ${PLATFORM_PREFIX}/${PLATFORM_ARCH}/usr/lib/libglslopt.a) # this is required too

# PPAPI libraries
set(PPAPI_SIMPLE_LIBRARY  "-Wl,-undefined=PSUserCreateInstance" "${PLATFORM_SDK_LIBDIR}/libppapi_simple.a")
set(NACL_IO_LIBRARY       "${PLATFORM_SDK_LIBDIR}/libnacl_io.a")
set(PPAPI_GLES2_LIBRARY   "${PLATFORM_SDK_LIBDIR}/libppapi_gles2.a")
set(PPAPI_CPP_LIBRARY     "${PLATFORM_SDK_LIBDIR}/libppapi_cpp.a")
set(PPAPI_LIBRARY         "-lppapi")

 
