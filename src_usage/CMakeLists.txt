
FILE(GLOB_RECURSE SOURCE_FILES "*.cpp")
#FILE(GLOB SOURCE_FILES "*.cpp")



# Function to add executable and link libraries
function(create_executable file)
    get_filename_component(program ${file} NAME_WE)
    add_executable(${program} ${file})

    target_link_libraries(${program} ${PTOOLS_LIB})

endfunction()

# Loop through the source files and create executables
foreach(file ${SOURCE_FILES})
    create_executable(${file})
endforeach()



