include_directories(${SOURCE_DIR}/tokenlexeme 
                    ${SOURCE_DIR}/scanner
                    ${SOURCE_DIR}/parser)

link_directories(${BINARY_DIR}/Hello)

add_executable(vecomp ${SOURCE_DIR}/main.cpp)

target_link_libraries(vecomp tokenlexeme) 

