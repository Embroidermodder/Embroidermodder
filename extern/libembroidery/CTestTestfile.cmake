# Generate tests by removing the ".c" extension from the filenames in
# "test/" then passing to "new_test".
file(GLOB TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/test "test/*")
foreach(FILE ${TEST_FILES})
        string(REPLACE ".c" "" TEST_NAME ${FILE})
        message("-- Adding test ${TEST_NAME}.")
        add_test(test_${TEST_NAME} test_${TEST_NAME})
endforeach()
