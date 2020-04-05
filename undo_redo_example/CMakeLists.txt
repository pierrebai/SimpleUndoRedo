add_executable(undo_redo_example simple_undo_redo.cpp)

target_link_libraries(undo_redo_example PUBLIC undo_redo)

target_compile_features(undo_redo_example PUBLIC cxx_std_20)

target_include_directories(undo_redo_example PUBLIC
   "${PROJECT_SOURCE_DIR}/undo_redo/inc")

