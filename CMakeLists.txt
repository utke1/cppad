# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# =============================================================================
# Some constants
# =============================================================================
# Set the minimum required version of cmake for this project.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# cppad_version is used by set_version.sh to get the version number.
SET(cppad_version      "20141230" )
SET(cppad_url          "http://www.coin-or.org/CppAD" )
SET(cppad_description  "Differentiation of C++ Algorithms" )


# Set name of this project and create the variables 
# cppad_BINARY_DIR and cppad_SOURCE_DIR. 
# project(projectname [CXX] [C] [Java])
PROJECT(cppad)

# Add this directory to the list of C++ preprocessor include directories
# for the entire project.
# include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
INCLUDE_DIRECTORIES( ${cppad_SOURCE_DIR} )



# =============================================================================
# Some system cmake language extensions
# =============================================================================
# CHECK_CXX_SOURCE_COMPILES(source variable)
# Checks whether the code given in source will compile, link and run and
# return zero status. You can set 
# CMAKE_REQUIRED_LIBRARIES, CMAKE_REQUIRED_FLAGS and CMAKE_REQUIRED_INCLUDES 
# accordingly if additional libraries or compiler flags are required. 
INCLUDE(CheckCXXSourceRuns)

# ============================================================================
# Some local cmake language
# ============================================================================
# dos_path_to_unix(dos_path unix_path)
INCLUDE(cmake/dos_path_to_unix.cmake)
# 
# add_to_list(variable_list constant_value)
INCLUDE(cmake/add_to_list.cmake)
#
# command_line_arg(variable default type description)
INCLUDE(cmake/command_line_arg.cmake)
#
# optional_package(prefix_variable description)
INCLUDE(cmake/optional_package.cmake)
#
# add_cppad_cxx_flags(target_name)
INCLUDE(cmake/add_cppad_cxx_flags.cmake)
#
# sources_libs_define(prefix_name sources libs define)
INCLUDE(cmake/sources_libs_define.cmake)
#
# check_source_runs(source variable)
INCLUDE(cmake/check_source_runs.cmake)
# =============================================================================
# command line arguments
# =============================================================================
# cmake_install_datadir
command_line_arg(cmake_install_datadir share STRING 
	"directory, below prefix, where cmake installs cppad data files"
)
# -----------------------------------------------------------------------------
# cmake_install_docdir
command_line_arg(cmake_install_docdir "" STRING 
	"directory, below prefix, where cmake installs cppad documentation files"
)
# -----------------------------------------------------------------------------
# cmake_install_includedirs
command_line_arg(cmake_install_includedirs include STRING 
	"directories, below prefix, where cmake installs include files"
)
# -----------------------------------------------------------------------------
# cmake_install_libdirs
command_line_arg(cmake_install_libdirs lib STRING 
	"directories, below prefix, where cmake installs library files"
)
# -----------------------------------------------------------------------------
# cmake_install_prefix
command_line_arg(cmake_install_prefix /usr PATH 
	"cppad install prefix"
)
SET(CMAKE_INSTALL_PREFIX "${cmake_install_prefix}" 
	CACHE PATH "value copied from cmake_install_prefix" FORCE
)
# -----------------------------------------------------------------------------
# cmake_install_postfix
command_line_arg(cmake_install_postfix NOTFOUND STRING 
	"cppad install postfix"
)
# -----------------------------------------------------------------------------
# cppad_cxx_flags
command_line_arg(cppad_cxx_flags NOTFOUND STRING 
	"extra flags used with cppad source (besides debug and release flags)"
)
# -----------------------------------------------------------------------------
# cppad_documentation
command_line_arg(cppad_documentation "" STRING 
	"install cppad documentation: use camke_install_docdir instead"
)
STRING(COMPARE NOTEQUAL "${cppad_documentation}" "" cppad_documentation_error)
IF( cppad_documentation_error )
	MESSAGE(FATAL_ERROR 
"cppad_documentation is no longer an option, use cmake_install_docdir instead"
	)
ENDIF( cppad_documentation_error )
# -----------------------------------------------------------------------------
# External package prefixes
#
# adolc_prefix
optional_package(adolc_prefix "adolc install prefix")
#
# colpack_prefix
optional_package(colpack_prefix "colpack install prefix")
#
# eigen_prefix
optional_package(eigen_prefix "eigen install prefix")
#
# fadbad_prefix
optional_package(fadbad_prefix "fadbad install prefix")
#
# ipopt_prefix
optional_package(ipopt_prefix "ipopt install prefix")
#
# sacado_prefix
optional_package(sacado_prefix "sacado install prefix")
# -----------------------------------------------------------------------------
# cppad_has_colpack
#
IF( colpack_prefix )
	SET(cppad_has_colpack 1)
ELSE( colpack_prefix )
	SET(cppad_has_colpack 0)
ENDIF( colpack_prefix )
# =============================================================================
# automated system configuration
# =============================================================================
# cppad_install_includedir
SET(cppad_install_includedir NOTFOUND)
FOREACH(dir ${cmake_install_includedirs})
	IF( NOT cppad_install_includedir )
		SET(cppad_install_includedir ${dir})
	ENDIF( NOT cppad_install_includedir )
ENDFOREACH(dir ${cmake_install_includedirs})
# -----------------------------------------------------------------------------
# cppad_install_libdir
SET(cppad_install_libdir NOTFOUND)
FOREACH(dir ${cmake_install_libdirs})
	IF( NOT cppad_install_libdir )
		SET(cppad_install_libdir ${dir})
	ENDIF( NOT cppad_install_libdir )
ENDFOREACH(dir ${cmake_install_libdirs})
# -----------------------------------------------------------------------------
# cppad_cxx_flags_sacado
IF( ${cppad_cxx_flags} MATCHES ".*-std=c[+][+]11.*" )
	SET(cppad_cxx_flags_sacado "${cppad_cxx_flags} -D HAS_C99_TR1_CMATH" )
ELSE( ${cppad_cxx_flags} MATCHES ".*-std=c[+][+]11.*" )
	SET(cppad_cxx_flags_sacado ${cppad_cxx_flags} )
ENDIF( ${cppad_cxx_flags} MATCHES ".*-std=c[+][+]11.*" )
MESSAGE(STATUS "cppad_cxx_flags_sacado = ${cppad_cxx_flags_sacado}")
# -----------------------------------------------------------------------------
# cppad_cxx_flags_skip_eigen
STRING(REGEX REPLACE 
	"(.*)-Wshadow(.*)"         "\\1-Wno-shadow\\2" 
	cppad_cxx_flags_eigen "${cppad_cxx_flags}"
)
MESSAGE(STATUS "cppad_cxx_flags_eigen = ${cppad_cxx_flags_eigen}")
# -----------------------------------------------------------------------------
# cppad_cxx_flags_skip_fadbad
STRING(REGEX REPLACE 
	"(.*)-Wshadow(.*)"         "\\1-Wno-shadow\\2" 
	cppad_cxx_flags_fadbad "${cppad_cxx_flags}"
)
MESSAGE(STATUS "cppad_cxx_flags_fadbad = ${cppad_cxx_flags_fadbad}")
# -----------------------------------------------------------------------------
# cppad_abs_includedir, cppad_abs_libdir, cppad_abs_datadir, cppad_abs_docdir
#
# cppad_abs_docdir is not used if cmake_install_docdir is ""
#
IF ( cmake_install_postfix )
	SET(cppad_abs_includedir 
"${cmake_install_prefix}/${cppad_install_includedir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
	SET(cppad_abs_libdir 
"${cmake_install_prefix}/${cppad_install_libdir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install library file directory" FORCE
	)
	SET(cppad_abs_datadir 
"${cmake_install_prefix}/${cmake_install_datadir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
	SET(cppad_abs_docdir 
"${cmake_install_prefix}/${cmake_install_docdir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
ELSE ( cmake_install_postfix )
	SET(cppad_abs_includedir 
		"${cmake_install_prefix}/${cppad_install_includedir}" 
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
	SET(cppad_abs_libdir 
		"${cmake_install_prefix}/${cppad_install_libdir}" 
		CACHE INTERNAL "cppad install library file directory" FORCE
	)
	SET(cppad_abs_datadir 
		"${cmake_install_prefix}/${cmake_install_datadir}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
	SET(cppad_abs_docdir 
		"${cmake_install_prefix}/${cmake_install_docdir}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
ENDIF ( cmake_install_postfix )
# -----------------------------------------------------------------------------
# boost_prefix
FIND_PACKAGE(Boost)
IF ( Boost_FOUND )
	# Extract the Boost prefix from Boost_INCLUDE_DIRS
	#
	# convert to using unix directory separator
	dos_path_to_unix("${Boost_INCLUDE_DIRS}" boost_include_dirs)
	#
	# convert to just one directory
	STRING(REGEX REPLACE
		"([^ ]+).*" "\\1"
		boost_include_dir "${boost_include_dirs}"
	)
	#
	# extract part before last backslash
	STRING(REGEX REPLACE 
		"([^ ]*)/[^/ ]*" "\\1" 
		boost_prefix "${boost_include_dir}" 
	) 
	MESSAGE(STATUS "boost_prefix = ${boost_prefix}")
	#
	# add boost include directories
	FOREACH(dir ${cmake_install_includedirs})
		IF( IS_DIRECTORY ${boost_prefix}/${dir} )
			INCLUDE_DIRECTORIES( ${boost_prefix}/${dir} )
			MESSAGE(STATUS "Found ${boost_prefix}/${dir}")
		ENDIF( IS_DIRECTORY ${boost_prefix}/${dir} )
	ENDFOREACH( dir )
	#
	# add boost link directories
	FOREACH(dir ${cmake_install_libdirs})
		IF( IS_DIRECTORY ${boost_prefix}/${dir} )
			LINK_DIRECTORIES( ${boost_prefix}/${dir} )
			MESSAGE(STATUS "Found ${boost_prefix}/${dir}")
		ENDIF( IS_DIRECTORY ${boost_prefix}/${dir} )
	ENDFOREACH( dir )
ENDIF ( Boost_FOUND )
# -----------------------------------------------------------------------------
# ipopt_LIBRARIES and ipopt_LIBRARY_DIRS
IF( ipopt_prefix )
	FIND_PACKAGE(PkgConfig)
	IF( NOT PKG_CONFIG_FOUND )
		MESSAGE(FATAL_ERROR "Using ipopt_prefix but cannot find pkg-config")
	ENDIF( NOT PKG_CONFIG_FOUND )
	#
	# Set the system environment variable PKG_CONFIG_PATH
	FOREACH(dir ${cmake_install_libdirs})
		IF(EXISTS "${ipopt_prefix}/${dir}/pkgconfig/ipopt.pc")
			SET( ENV{PKG_CONFIG_PATH} ${ipopt_prefix}/${dir}/pkgconfig )
		ENDIF(EXISTS "${ipopt_prefix}/${dir}/pkgconfig/ipopt.pc")
	ENDFOREACH(dir)
	#
	# pkg_check_modules(<PREFIX> [REQUIRED] <MODULE> [<MODULE>]*)
	# ipopt_LIBRARIES      ... only the libraries (w/o the '-l')
	# ipopt_LIBRARY_DIRS   ... the paths of the libraries (w/o the '-L')
	pkg_check_modules(ipopt ipopt)
	IF( NOT ipopt_FOUND )
		MESSAGE(FATAL_ERROR 
"For all directories dir in cmake_install_libdirs, cannot find the file
	ipopt_prefix/dir/pkgconfig/ipopt.pc 
where
	ipopt_prefix = ${ipopt_prefix}
	cmake_install_libdirs = ${cmake_install_libdirs}
"
		)
	ENDIF( NOT ipopt_FOUND )
ENDIF( ipopt_prefix )
# =============================================================================
# Currently building tests as normal executables
# =============================================================================
# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
#
# Initialize list of tests as empty
SET(check_depends "")
#
ADD_SUBDIRECTORY(cppad)
ADD_SUBDIRECTORY(pkgconfig)
#
ADD_SUBDIRECTORY(compare_c)
add_to_list(check_depends check_compare_c)
#
ADD_SUBDIRECTORY(example)
add_to_list(check_depends check_example)
#
ADD_SUBDIRECTORY(introduction)
add_to_list(check_depends check_introduction)
#
ADD_SUBDIRECTORY(print_for)
add_to_list(check_depends check_print_for)
#
ADD_SUBDIRECTORY(test_more)
add_to_list(check_depends check_test_more)
#
ADD_SUBDIRECTORY(multi_thread)
add_to_list(check_depends check_multi_thread)
#
ADD_SUBDIRECTORY(speed)
add_to_list(check_depends check_speed)
#
IF ( ipopt_prefix )
	ADD_SUBDIRECTORY(cppad_ipopt)
	add_to_list(check_depends check_cppad_ipopt)
ENDIF( ipopt_prefix )
#
ADD_CUSTOM_TARGET(check DEPENDS ${check_depends})
#
# =============================================================================
# install procedure
# =============================================================================
# install(DIRECTORY dirs... DESTINATION <dir>
#	[FILE_PERMISSIONS permissions...]
#	[DIRECTORY_PERMISSIONS permissions...]
#	[USE_SOURCE_PERMISSIONS] [OPTIONAL]
#	[CONFIGURATIONS [Debug|Release|...]]
#	[COMPONENT <component>] [FILES_MATCHING]
#	[[PATTERN <pattern> | REGEX <regex>]
#	[EXCLUDE] [PERMISSIONS permissions...]] [...]
# )
# Note a trailing / in the source directory name drops the source directory
# name during the copy.
#
# During install copy all the cppad include files to 
# ${cppad_abs_includedir}/cppad  
INSTALL( 
	DIRECTORY "cppad/" 
	DESTINATION ${cppad_abs_includedir}/cppad
	FILES_MATCHING PATTERN "*.h"
)
INSTALL( 
	DIRECTORY "cppad/" 
	DESTINATION ${cppad_abs_includedir}/cppad
	FILES_MATCHING PATTERN "*.hpp"
)
INSTALL( 
	FILES "cppad/local/cppad_colpack.cpp"
	DESTINATION ${cppad_abs_datadir}/cppad
)
#
# During install copy doc direcrory to cppad_abs_docdir/cppad
STRING( COMPARE NOTEQUAL "${cmake_install_docdir}" "" cppad_documentation )
IF ( cppad_documentation )
	INSTALL( DIRECTORY "doc/" DESTINATION ${cppad_abs_docdir}/cppad )
ENDIF ( cppad_documentation )
