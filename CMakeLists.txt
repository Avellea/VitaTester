cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(VitaTester C)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaTester")
set(VITA_TITLEID  "VITATESTR")
set(VITA_VERSION  "01.00")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

file(GLOB ICON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png.h")

add_executable(VitaTester
  src/main.c
  src/font.c
  ${ICON_HEADERS}
)

target_include_directories(VitaTester PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/icons
)

target_compile_options(VitaTester PRIVATE
  -Wall
  -Wextra
)

target_link_libraries(VitaTester
  vita2d
  SceDisplay_stub
  SceAppMgr_stub
  SceGxm_stub
  SceSysmodule_stub
  SceCtrl_stub
  SceTouch_stub
  SceMotion_stub
  SceCommonDialog_stub
  freetype
  png
  bz2
  z
  m
)

vita_create_self(VitaTester.self VitaTester)
vita_create_vpk(VitaTester.vpk ${VITA_TITLEID} VitaTester.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
