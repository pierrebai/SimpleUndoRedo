
add_library(undo_redo
   src/undo_stack.cpp                inc/dak/utility/undo_stack.h
)

target_include_directories(undo_redo PUBLIC
   inc)

target_compile_features(undo_redo PUBLIC cxx_std_20)

