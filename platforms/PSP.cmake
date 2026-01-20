find_package(PkgConfig REQUIRED)

add_executable(RetroEngine ${RETRO_FILES})

set(DEP_PATH psp)

pkg_check_modules(OGG ogg)

if(NOT OGG_FOUND)
    set(COMPILE_OGG TRUE)
    message(NOTICE "libogg not found, attempting to build from source")
else()
    message("found libogg")
    target_link_libraries(RetroEngine ${OGG_STATIC_LIBRARIES})
    target_link_options(RetroEngine PRIVATE ${OGG_STATIC_LDLIBS_OTHER})
    target_compile_options(RetroEngine PRIVATE ${OGG_STATIC_CFLAGS})
endif()

pkg_check_modules(VORBIS vorbis vorbisfile)

if(NOT VORBIS_FOUND)
    set(COMPILE_VORBIS TRUE)
    message(NOTICE "libvorbis not found, attempting to build from source")
else()
    message("found libvorbis")
    target_link_libraries(RetroEngine ${VORBIS_STATIC_LIBRARIES})
    target_link_options(RetroEngine PRIVATE ${VORBIS_STATIC_LDLIBS_OTHER})
    target_compile_options(RetroEngine PRIVATE ${VORBIS_STATIC_CFLAGS})
endif()

set(RETRO_MOD_LOADER OFF CACHE BOOL "Disable the mod loader" FORCE)
set(RETRO_USE_HW_RENDER OFF CACHE BOOL "Disable hardware rendering on PSP" FORCE)
set(RETRO_NETWORKING OFF CACHE BOOL "Disable networking on PSP" FORCE)
set(RETRO_DISABLE_PLUS ON CACHE BOOL "Disable Origins Plus Content on PSP" FORCE)

if(RETRO_MOD_LOADER)
    set_target_properties(RetroEngine PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()

target_compile_options(RetroEngine PRIVATE 
    -O3
    -ffast-math
    -fomit-frame-pointer
    -fno-exceptions
    -fno-rtti
    -finline-functions
    -funroll-loops
    -march=allegrex
    -mtune=allegrex
    -mno-check-zero-division
    -fsingle-precision-constant
)
target_compile_definitions(RetroEngine PRIVATE 
    RETRO_DISABLE_LOG=1
    USE_SW_REN=1
)
target_link_libraries(RetroEngine 
    pspaudiolib
    pspaudio
    pspgu
    pspgum
    pspge
    pspdisplay
    psprtc
    pspctrl
    psppower
    m
)

create_pbp_file(TARGET RetroEngine
    TITLE "RSDKv4")