#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       -= core

TARGET = UCore
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/planner_core/core

INCLUDEPATH += ../../../src/planner_core/core

HEADERS += algorithm/algorithm.h algorithm/graph.h algorithm/math.h algorithm/merge.h algorithm/sort.h avl_tree.h bit_container.h \
 byte_range.h complex_hashset/block_chain.h complex_hashset/block.h complex_hashset/buffered_complex_hashset.h complex_hashset/complex_hashset_base.h \
 complex_hashset/_delayed_complex_hashset.h complex_hashset/delayed_complex_hashset.h complex_hashset/direct_complex_hashset.h complex_hashset/generator.h \
 compressed_stream.h config.h containers/complex_queue.h containers/complex_stack.h containers/complex_vector.h containers/external_queue.h \
 containers/external_vector.h delayed_buffer.h hash.h io/cached_file.h io/cache.h io/data_file.h io/data_file_unix.h io/data_file_win.h io/streamer.h \
 masked_bit_vector.h numeric_expression.h problem_instance_generator.h search_algorithms/astar_engine.h search_algorithms/batched_engine.h search_algorithms/bfs_engine.h \
 search_algorithms/dfs_engine.h search_algorithms/greedy_bfs_engine.h search_algorithms/queued_engine.h search_algorithms/search_engine_base.h search_algorithms/serial_engine.h \
 search_database.h search_queue.h state_space_solver.h thread_pool.h transition_system.h UError.h UExternalMemoryController.h UMatrix.hpp USSingleton.h utils/helpers.h \
 varset_system/boolvar_system.h varset_system/combinedvar_system.h varset_system/floatvar_system.h varset_system/multivar_system.h varset_system/transition_index.h \
 varset_system/UState.h varset_system/UTransition.h varset_system/varset_system_base.h

SOURCES += UError.cpp                    UMatrix.cpp   UMatrix.hpp               bit_container.cpp             masked_bit_vector.cpp         numeric_expression.cpp \
utils/helpers.cpp

#varset_system/UTransition.cpp  varset_system/UState.cpp varset_system/UTransitionSystem.cpp

HEADERS += 


LIBS += 

