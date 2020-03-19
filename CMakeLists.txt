
#Specify the version being used as well as the language
cmake_minimum_required(VERSION 2.6)


# globals
# Name of the files that contain the main for the player and dealer
set(CLIENT_MAIN "src/main.cpp")
set(SERVER_MAIN "src/dealer.cpp")

# end globals


#Name of the project
project(Poker++)

# make cmake aware that we have all our header files in the 
# include subdirectory (the include path should contain that)
include_directories(include)

# find_package can include third party packages into cmake
# the pkgconfig tool is included which is used by some libraries
# to find out how they should be used whne compiling
# e.g.,  try on the command line: pkg-config --cflags glib-2.0
find_package(PkgConfig REQUIRED)

# now that we can use pkg-config, let's use it to find everything 
# about glib-2.0; everything that is found will be 
# prefixed by GLIB_PKG
pkg_check_modules(GTK_PKG gtkmm-3.0)

# the _FOUND postfix will be set if the package has actually been found
if(${GTK_PKG_FOUND})
  #print message for user:
  #message(STATUS "Found:glib-2.0")
  #make sure "include directories" contains the glib header directories
  include_directories(${GTK_PKG_INCLUDE_DIRS})
  #make sure "link directories" contains the glib library directories
  link_directories(${GTK_PKG_LIBRARY_DIRS})
else()
  #print message for user:
  message(FATAL_ERROR "gtkmm-3.0 not found. It is required. exiting.")
endif()

# Add all source files
file(GLOB SOURCES "src/*.cpp")
add_library(LIB ${SOURCES}) 


#This tells cmake to create the executable based on those sources
add_executable(poker_client ${CLIENT_MAIN})

# Add later when we decide the name of the server source file
add_executable(poker_server ${SERVER_MAIN})

# link executables to our source files
target_link_libraries (poker_client ${LIB})
target_link_libraries (poker_server ${LIB})

# tells cmake to use the required libraries (in this case glib)
target_link_libraries (poker_client ${GTK_PKG_LIBRARIES})
target_link_libraries (poker_server ${GTK_PKG_LIBRARIES})

# copy over glade files to build directory
file(GLOB GLADE_FILES "src/*.glade")
file(COPY ${GLADE_FILES} DESTINATION ".")

# copy over image card files over
file(GLOB IMAGE_FILES "src/cards/*.png")
file(COPY ${IMAGE_FILES} DESTINATION "cards/.")