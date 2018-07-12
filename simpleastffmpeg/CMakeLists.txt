cmake_minimum_required(VERSION 3.4.1)

#指定项目名称，一般和项目的文件夹名对应
#project(JNIDemo10-2018-03-02)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../FFmpegjni/src/main/jniLibs/${ANDROID_ABI})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

#c++ 开发 有些功能麻烦，就以后就使用C吧
#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
# if(COMPILER_SUPPORTS_CXX11)
#       set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#elseif(COMPILER_SUPPORTS_CXX0X)
#        set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
# else()
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lvirt")
#     message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
# endif()

MESSAGE("####################################################################")
message("ANDROID_ABI:..............." ${ANDROID_ABI})
message("ANDROID_SYSROOT: .........." ${ANDROID_SYSROOT})
message("ANDROID_NDK: .............." ${ANDROID_NDK})
message("ANDROID_TOOLCHAIN_PREFIX: ." ${ANDROID_TOOLCHAIN_PREFIX})
message("ANDROID_TOOLCHAIN_SUFFIX: ." ${ANDROID_TOOLCHAIN_SUFFIX})
message("ANDROID_TOOLCHAIN_NAME: ..." ${ANDROID_TOOLCHAIN_NAME})
message("ANDROID_TOCHAIN_ROOT: ....." ${ANDROID_TOOLCHAIN_ROOT})
MESSAGE("####################################################################")


add_library( avcodec-57
             SHARED
             IMPORTED)
set_target_properties( avcodec-57
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavcodec-57.so)

add_library( avfilter-6
             SHARED
             IMPORTED)
set_target_properties( avfilter-6
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavfilter-6.so)

add_library( avformat-57
             SHARED
             IMPORTED)
set_target_properties( avformat-57
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavformat-57.so)

add_library( avutil-55
             SHARED
             IMPORTED)
set_target_properties( avutil-55
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavutil-55.so)

add_library( swresample-2
             SHARED
             IMPORTED)
set_target_properties( swresample-2
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswresample-2.so)

add_library( swscale-4
             SHARED
             IMPORTED)
set_target_properties( swscale-4
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswscale-4.so)

include_directories(src/main/cpp/include)


aux_source_directory(src/main/cpp/  DIR_LIB_SRCS)
add_library(ffmpeg-dev SHARED ${DIR_LIB_SRCS})


find_library( log-lib
              log )
find_library( # Sets the name of the path variable.
              android-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              android )

#位置也要注意，A B C D E F...A 引用B，B引用C 或D或E，或F或不引用，但是顺序不能变了，后者被前者引用
target_link_libraries( ffmpeg-dev
                       avcodec-57
                       avfilter-6
                       avformat-57
                       avutil-55
                       swresample-2
                       swscale-4
                       ${log-lib}
                        android)