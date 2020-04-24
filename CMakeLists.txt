cmake_minimum_required (VERSION 3.15)
project(FaceTracker CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 4 REQUIRED
  COMPONENTS core imgproc objdetect videoio highgui)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/IO.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/PDM.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/Patch.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/CLM.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/FDet.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/PAW.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/FCheck.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/lib/Tracker.cc
)

set(HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/IO.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/PDM.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/Patch.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/CLM.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/FDet.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/PAW.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/FCheck.h
    ${CMAKE_CURRENT_LIST_DIR}/src/include/FaceTracker/Tracker.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/include/)

target_link_libraries(${PROJECT_NAME} PUBLIC opencv_core)

add_executable(face_tracker ${CMAKE_CURRENT_LIST_DIR}/src/exe/face_tracker.cc)
target_link_libraries(face_tracker PUBLIC ${PROJECT_NAME} opencv_core opencv_highgui opencv_objdetect)

install(TARGETS face_tracker DESTINATION bin)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/FaceTracker)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/model DESTINATION model)
