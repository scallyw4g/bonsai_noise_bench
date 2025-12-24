cmake_minimum_required(VERSION 4.2.1)
set(CMAKE_CXX_STANDARD 17)

project(BonsaiNoiseBench)
add_executable(BonsaiNoiseBench "src/bonsai_main.cpp")

add_subdirectory(external/FastNoise2)
target_link_libraries(BonsaiNoiseBench FastNoise2)

target_include_directories(BonsaiNoiseBench PUBLIC "${CMAKE_SOURCE_DIR}")
target_include_directories(BonsaiNoiseBench PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(BonsaiNoiseBench PUBLIC "${CMAKE_SOURCE_DIR}/external")

if (LINUX)
    target_link_libraries(BonsaiNoiseBench pthread X11 dl GL)
    add_compile_definitions(BONSAI_LINUX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif (LINUX)

add_compile_definitions(BONSAI_INTERNAL)

set(CMAKE_CXX_FLAGS "-ferror-limit=2000 -mssse3 -mavx -mavx2 -mfma -mvaes -Weverything -Wno-reserved-identifier -Wno-reserved-id-macro -Wno-unknown-warning-option -Wno-unsafe-buffer-usage -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-missing-prototypes -Wno-zero-as-null-pointer-constant -Wno-format-nonliteral -Wno-cast-qual -Wno-unused-function -Wno-four-char-constants -Wno-old-style-cast -Wno-float-equal -Wno-global-constructors -Wno-cast-align -Wno-switch-enum -Wno-switch-default -Wno-covered-switch-default -Wno-undef -Wno-c99-extensions -Wno-dollar-in-identifier-extension -Wno-class-varargs -Wno-unused-value -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-implicit-int-float-conversion -Wno-extra-semi-stmt -Wno-reorder-init-list -Wno-unused-macros -Wno-padded -Wno-gnu-zero-variadic-macro-arguments -Wno-atomic-implicit-seq-cst -Wno-nonportable-system-include-path -Wno-nonportable-include-path")
