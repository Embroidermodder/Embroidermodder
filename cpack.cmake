# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
# https://cmake.org/cmake/help/latest/cpack_gen/wix.html

if(WIN32)
set(CPACK_GENERATOR "WIX")
set(CPACK_SOURCE_GENERATOR "WIX")
else(WIN32)
set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "DEB")
endif(WIN32)
set(CPACK_PACKAGE_DIRECTORY build)
set(CPACK_PACKAGE_VENDOR "The Embroidermodder Team")
set(CPACK_SOURCE_IGNORE_FILES .git .github)

set(CPACK_WIX_PRODUCT_ICON assets/embroidermodder2.ico)

#set(CPACK_RESOURCE_FILE_LICENSE ZLIB-LICENSE.txt)

include(CPack)
