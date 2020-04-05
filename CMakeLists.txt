cmake_minimum_required(VERSION 3.7.0)

project(SimpleUndoRedo)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

add_subdirectory(undo_redo)
add_subdirectory(undo_redo_tests)
add_subdirectory(undo_redo_example)

