pkg_check_modules(JSONCPP QUIET jsoncpp)
set (required_vars JSONCPP_LIBRARIES)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (JSONCPP DEFAULT_MSG ${required_vars})

mark_as_advanced (JSONCPP_LIBRARIES JSONCPP_INCLUDE_DIRS)
