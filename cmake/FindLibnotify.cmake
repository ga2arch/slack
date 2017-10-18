find_package(PkgConfig)
pkg_check_modules(LIBNOTIFY QUIET libnotify)
set (required_vars LIBNOTIFY_LIBRARIES)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (LIBNOTIFY DEFAULT_MSG ${required_vars})

mark_as_advanced(LIBNOTIFY_INCLUDE_DIRS LIBNOTIFY_LIBRARIES)
