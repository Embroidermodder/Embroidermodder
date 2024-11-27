cmake_minimum_required(VERSION 3.0)
project(libembroidery_java_test)

if (NOT DEFINED JAVA_HOME)
set(JAVA_HOME
    /usr/lib/jvm/java-11-openjdk-amd64
)
endif()

find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseJava)

set(SOURCES
    ../embroidery.c
    org/libembroidery/jni_wrapper.c
)

set(JAVA_CLASSES
    org/libembroidery/Main.java
    org/libembroidery/Pattern.java
)

add_library(libembroidery_java SHARED ${SOURCES})
target_link_libraries(libembroidery_java ${JAVA_JVM_LIBRARY})
target_include_directories(libembroidery_java PRIVATE
    ${JAVA_HOME}/include
    ${JAVA_HOME}/include/linux
    ${CMAKE_PROJECT_DIR}/..)
add_jar(libembrodiery_java_test ${JAVA_CLASSES} ENTRY_POINT Main)

# sudo add-apt-repository ppa:webupd8team/java
# sudo apt-get --allow-releaseinfo-change update
# sudo apt-get update
# sudo apt-get install oracle-java8-installer

# sudo apt-get update
# sudo apt-get install default-jre default-jdk
