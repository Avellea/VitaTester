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
  FILE icons/analog.png icons/analog.png
  FILE icons/background.png icons/background.png
  FILE icons/circle.png icons/circle.png
  FILE icons/cross.png icons/cross.png
  FILE icons/dpad.png icons/dpad.png
  FILE icons/finger_blue.png icons/finger_blue.png
  FILE icons/finger_gray.png icons/finger_gray.png
  FILE icons/ltrigger.png icons/ltrigger.png
  FILE icons/rtrigger.png icons/rtrigger.png
  FILE icons/select.png icons/select.png
  FILE icons/square.png icons/square.png
  FILE icons/start.png icons/start.png
  FILE icons/triangle.png icons/triangle.png
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
