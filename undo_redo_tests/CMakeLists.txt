
add_library(undo_redo_tests SHARED
   undo_stack_tests.cpp
)

target_link_libraries(undo_redo_tests PUBLIC undo_redo)

target_compile_features(undo_redo_tests PUBLIC cxx_std_20)

target_include_directories(undo_redo_tests PUBLIC
   "${PROJECT_SOURCE_DIR}/undo_redo/inc")

