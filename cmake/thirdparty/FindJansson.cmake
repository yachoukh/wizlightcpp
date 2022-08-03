# FindJansson
# -----------
#
# Find Jansson library, this modules defines:
#
# Jansson_INCLUDE_DIRS, where to find jansson.h
# Jansson_LIBRARIES, where to find library
# Jansson_FOUND, if it is found

find_path(Jansson_INCLUDE_DIR NAMES jansson.h)
find_library(Jansson_LIBRARY NAMES libjansson jansson)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	Jansson
	FOUND_VAR Jansson_FOUND
	REQUIRED_VARS Jansson_LIBRARY Jansson_INCLUDE_DIR
)

set(Jansson_LIBRARIES ${Jansson_LIBRARY})
set(Jansson_INCLUDE_DIRS ${Jansson_INCLUDE_DIR})

mark_as_advanced(Jansson_INCLUDE_DIR Jansson_LIBRARY)