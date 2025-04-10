#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.27)
project(SnowSim)

set(ENV{PATH} "/opt/homebrew/opt/qt@5/bin:$ENV{PATH}")
set(CMAKE_PREFIX_PATH "/Users/rob/Software/hdf5/install" ${CMAKE_PREFIX_PATH})
# set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5/bin" ${CMAKE_PREFIX_PATH})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# See the documentation for a guide on how to enable/disable specific components
#
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Set the path to your CRY library
# Replace this with the actual location of your libCRY.a file
set(CRY_LIBRARY_PATH /Users/rob/Projects/aria/cry_v1.7/lib/libCRY.a)
set(CRY_INCLUDE_DIRS /Users/rob/Projects/aria/cry_v1.7/install/include)
include_directories(${CRY_INCLUDE_DIRS})


#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries
#
add_executable(snowsim snowsim.cc ${sources} ${headers})
target_include_directories(snowsim PRIVATE include ${HDF5_INCLUDE_DIRS})
target_link_libraries(snowsim PRIVATE ${Geant4_LIBRARIES} ${CRY_LIBRARY_PATH} ${HDF5_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build SnowSim. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
# set(SnowSim_SCRIPTS
#   init_vis.mac
#   vis.mac
#   cryvis.mac
#   crysetup.mac
#   cryverbose.mac
#   )

set(SCRIPT_DIR ${PROJECT_SOURCE_DIR}/mac)
file(GLOB SnowSim_SCRIPTS "${SCRIPT_DIR}/*.mac")

foreach(_script ${SnowSim_SCRIPTS})
  get_filename_component(FILE_NAME ${_script} NAME)
  configure_file(
    ${_script}
    ${PROJECT_BINARY_DIR}/${FILE_NAME}
    COPYONLY
    )
endforeach()