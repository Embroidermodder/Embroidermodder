# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html

if(WIN32)

# https://cmake.org/cmake/help/latest/cpack_gen/wix.html

set(CPACK_GENERATOR "WIX")
set(CPACK_SOURCE_GENERATOR "WIX")
set(CPACK_WIX_PRODUCT_ICON ../embroidermodder2/embroidermodder2.ico)
set(CPACK_WIX_MODIFY_PATH ON)

else(WIN32)

# https://cmake.org/cmake/help/latest/cpack_gen/deb.html

set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "embroidermodder")
set(CPACK_DEBIAN_PACKAGE_RELEASE OFF)
set(CPACK_DEBIAN_PACKAGE_VERSION "2.0.0alpha")
set(CPACK_DEBIAN_PACAKGE_DEPENDS "qt6-base")

endif(WIN32)

set(CPACK_PACKAGE_DIRECTORY build)
set(CPACK_PACKAGE_VENDOR "The Embroidermodder Team")
set(CPACK_SOURCE_IGNORE_FILES .git .github)

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.md)

include(CPack)
