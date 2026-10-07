# Allow explicit Python path via environment variable
if(DEFINED ENV{PYTHON} AND NOT Python3_EXECUTABLE)
    # PYTHON names an executable, whereas Python3_ROOT_DIR expects a directory.
    set(Python3_EXECUTABLE "$ENV{PYTHON}" CACHE FILEPATH "Python interpreter for builds")
endif()

# On Windows, prefer registry entries to avoid Cygwin/MSYS Python
# The registry is searched first by default, which finds native Windows Python
# installations rather than Cygwin/MSYS Python
if(WINDOWS)
    set(Python3_FIND_REGISTRY FIRST CACHE STRING "Python search order")
endif()

# Find Python 3 interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Set legacy variable name for compatibility with existing code
set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}" CACHE FILEPATH "Python interpreter for builds" FORCE)
mark_as_advanced(PYTHON_EXECUTABLE)
