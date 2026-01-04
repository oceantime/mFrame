# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-exceptions -funwind-tables")

add_definitions(-DFT2_BUILD_LIBRARY)
add_definitions(-DANDROID)

if ((${ANDROID_ARM_NEON}) AND (${ANDROID_ABI} MATCHES arm*))
    add_definitions(-DARM_NEON_OPT)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=softfp -mfpu=neon")
endif ()


#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
#set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")


include_directories("./src/")
include_directories("./src/wmcanvas")
include_directories("./src/wmcanvas/GL")
include_directories("./src/support")
include_directories("./src/platform/Android")
include_directories("./src/commandbuffer/")
include_directories("./src/webgl/")
include_directories("./android/")
include_directories("./android/png")
include_directories("./android/png/thirdparty")
include_directories("./android/memory")
include_directories("./android/3d")
include_directories("./android/3d/jsc")
include_directories("./android/3d/util")
include_directories("./android/3d/view")
include_directories("./android/freetype-prebuilt/include")

set(SRC_FILES
        #root srcs
        ./src/WmCanvas.cpp
        ./src/WmCanvasManager.cpp

        # wmcanvas srcs
        ./src/wmcanvas/WmCanvas2dContext.cpp
        ./src/wmcanvas/WmCanvasState.cpp
        ./src/wmcanvas/WmConvert.cpp
        ./src/wmcanvas/WmFontStyle.cpp
        ./src/wmcanvas/GL/WmFrameBufferObject.cpp
        ./src/wmcanvas/WmGlyphCache.cpp
        ./src/wmcanvas/WmPath.cpp
        ./src/wmcanvas/WmPath2D.cpp
        ./src/wmcanvas/WmPathStroker.cpp
        ./src/wmcanvas/GL/WmShader.cpp
        ./src/wmcanvas/GL/WmShaderManager.cpp
        ./src/wmcanvas/WmStrSeparator.cpp
        ./src/wmcanvas/GL/WmTexture.cpp
        ./src/wmcanvas/WmTreemap.cpp
        ./src/wmcanvas/WmWebglContext.cpp
        ./src/wmcanvas/WmFontManager.cpp

        # platform srcs
        ./src/platform/Android/WmCanvas2DContextImpl.cpp
        ./src/platform/Android/WmCanvas2DContextAndroid.cpp
        ./src/platform/Android/WmCanvasAndroid.cpp
        ./src/platform/Android/WmFreeTypeWrap.cpp
        ./src/platform/Android/WmFont.cpp
        ./src/platform/Android/WmFontCache.cpp
        ./src/platform/Android/WmFontFamily.cpp
        ./src/platform/Android/WmFontManagerAndroid.cpp
        ./src/platform/Android/WmFrameBufferObjectImpl.cpp
        ./src/platform/Android/WmPreCompiledShaders.cpp
        ./src/platform/Android/WmSystemFontInformation.cpp


        ./src/platform/Android/egl/WmEGLPbufferContext.cpp
        ./src/platform/Android/egl/WmEGLWindowContext.cpp
        ./src/platform/Android/egl/WmSharedEGLContext.cpp


        # support
        ./src/support/CharacterSet.cpp
        ./src/support/Encode.cpp
        ./src/support/FileUtils.cpp
        ./src/wmcanvas/GL/WmLUtil.cpp
        ./src/support/Log.cpp
        ./src/support/Util.cpp
        ./src/support/Value.cpp

        ./src/commandbuffer/WmCommandBuffer.cpp
        ./src/commandbuffer/WmCommandDecoder.cpp
        ./src/webgl/WmCommandDecoderWebGL.cpp
        ./src/webgl/WmWebGLRenderContextInner.cpp
        ./src/webgl/WmWebGLRenderContext.cpp
        )

if (WMCANVAS)
    add_definitions(-DWMCANVAS)
    file(GLOB android_root_src "./android/*.cpp")
    file(GLOB native_png_src "./android/png/*.cpp")
    file(GLOB native_png_thirdparty_src "./android/png/thirdparty/*.c")
    file(GLOB native_3d_src "./android/3d/*.cpp")
    file(GLOB native_3d_jsc_src "./android/3d/jsc/*.cpp")
    file(GLOB native_3d_util_src "./android/3d/util/*.cpp")
    file(GLOB native_3d_view_src "./android/3d/view/*.cpp")
    file(GLOB native_android_manager_src "./android/manager/*.cpp")
else()
    message(" not enter the wmcanvas >>>>>>>>>> ")
    list(APPEND SRC_FILES ./android/WmCanvasJNI.cpp)
endif()


if (WMCANVAS_RUNTIME)
    add_definitions(-DWMCANVAS_RUNTIME)
endif()



add_library( # Sets the name of the library.
        wmcanvas
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ${native_png_src}
        ${native_png_thirdparty_src}
        ${native_3d_src}
        ${native_3d_jsc_src}
        ${native_3d_util_src}
        ${native_3d_view_src}
        ${android_root_src}
        ${native_android_manager_src}
        ${SRC_FILES} )

# Set output name based on WMCANVAS_RUNTIME flag
if (WMCANVAS_RUNTIME)
    set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME "wmcanvas_runtime")
else()
    set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME "WmCanvas")
endif()


find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


add_library(freetype SHARED IMPORTED)
set_target_properties(freetype PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/android/freetype-prebuilt/libs/${ANDROID_ABI}/libfreetype.so)



#target_compile_options(wmcanvas PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties(wmcanvas PROPERTIES LINK_FLAGS -fsanitize=address)


target_link_libraries( # Specifies the target library.
        wmcanvas
        GLESv1_CM
        GLESv2
        android
        EGL
        atomic
        jnigraphics
        freetype
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


# ---------------------------- distribute headers  ----------------------------
if (WMCANVAS_DISTRIBUTE_HEADER)
    set(DIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/distribution)
    message ("DIST_DIR=${DIST_DIR}")
    function(copy_header_under_dir)
        set(DIR ${ARGV0})
        set(PAR ${ARGV1})
        message("copy_header_under_dir called:${DIR} - ${PAR}")
        file(GLOB TARGET_HEADERS "./${PAR}/${DIR}/*.h")
        file (GLOB TARGET_HEADERS_2 "./${PAR}/${DIR}/*.hpp")
        list (APPEND TARGET_HEADERS ${TARGET_HEADERS_2})

        foreach(header_file ${TARGET_HEADERS})
            get_filename_component(FN ${header_file} NAME)
            message("copy header file: ${header_file}, ${FN}")
            add_custom_command(TARGET wmcanvas POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E
                    copy "${header_file}" "${DIST_DIR}/wmcanvas_core/include/${DIR}/${FN}"
                    COMMENT "Copying ${FN} to output directory")
        endforeach(header_file)
    endfunction()

    copy_header_under_dir("." "src")
    copy_header_under_dir("wmcanvas", "src")
    copy_header_under_dir("wmcanvas/GL", "src")
    copy_header_under_dir("commandbuffer" "src")
    copy_header_under_dir("webgl" "src")
    copy_header_under_dir("support" "src")
    copy_header_under_dir("platform/Android" "src")
    copy_header_under_dir("platform/Android/egl" "src")


    copy_header_under_dir("freetype-prebuilt/include" "android")
    copy_header_under_dir("freetype-prebuilt/include/freetype" "android")
    copy_header_under_dir("freetype-prebuilt/include/freetype/config" "android")
    copy_header_under_dir("freetype-prebuilt/include/freetype/internal" "android")


    # 生成freetyp-prebuilt
#    copy_header_under_dir("freetype-prebuilt/include")
#    copy_header_under_dir("freetype-prebuilt/include/freetype")
#    copy_header_under_dir("freetype-prebuilt/include/freetype/config")
#    copy_header_under_dir("freetype-prebuilt/include/freetype/internal")

endif()
