# Install script for directory: /home/tuchi/muduo/muduo/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/tuchi/muduo/lib/libmuduo_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/tuchi/muduo/muduo/base/AsyncLogging.h"
    "/home/tuchi/muduo/muduo/base/Atomic.h"
    "/home/tuchi/muduo/muduo/base/BlockingQueue.h"
    "/home/tuchi/muduo/muduo/base/BoundedBlockingQueue.h"
    "/home/tuchi/muduo/muduo/base/Condition.h"
    "/home/tuchi/muduo/muduo/base/CountDownLatch.h"
    "/home/tuchi/muduo/muduo/base/CurrentThread.h"
    "/home/tuchi/muduo/muduo/base/Date.h"
    "/home/tuchi/muduo/muduo/base/Exception.h"
    "/home/tuchi/muduo/muduo/base/FileUtil.h"
    "/home/tuchi/muduo/muduo/base/GzipFile.h"
    "/home/tuchi/muduo/muduo/base/LogFile.h"
    "/home/tuchi/muduo/muduo/base/LogStream.h"
    "/home/tuchi/muduo/muduo/base/Logging.h"
    "/home/tuchi/muduo/muduo/base/Mutex.h"
    "/home/tuchi/muduo/muduo/base/ProcessInfo.h"
    "/home/tuchi/muduo/muduo/base/Singleton.h"
    "/home/tuchi/muduo/muduo/base/StringPiece.h"
    "/home/tuchi/muduo/muduo/base/Thread.h"
    "/home/tuchi/muduo/muduo/base/ThreadLocal.h"
    "/home/tuchi/muduo/muduo/base/ThreadLocalSingleton.h"
    "/home/tuchi/muduo/muduo/base/ThreadPool.h"
    "/home/tuchi/muduo/muduo/base/TimeZone.h"
    "/home/tuchi/muduo/muduo/base/Timestamp.h"
    "/home/tuchi/muduo/muduo/base/Types.h"
    "/home/tuchi/muduo/muduo/base/WeakCallback.h"
    "/home/tuchi/muduo/muduo/base/copyable.h"
    "/home/tuchi/muduo/muduo/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/tuchi/muduo/muduo/base/tests/cmake_install.cmake")

endif()

