cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(VitaTester)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaTester")
set(VITA_TITLEID  "VITATESTR")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(VitaTester
  src/main.c
  src/font.c
  icons/analog.png.h
  icons/background.png.h
  icons/circle.png.h
  icons/cross.png.h
  icons/dpad.png.h
  icons/finger_blue.png.h
  icons/finger_gray.png.h
  icons/ltrigger.png.h
  icons/rtrigger.png.h
  icons/select.png.h
  icons/square.png.h
  icons/start.png.h
  icons/triangle.png.h
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
