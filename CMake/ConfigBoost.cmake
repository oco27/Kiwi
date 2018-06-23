
message(STATUS "Boost - searching the static Boost library 1.63.0 with system")
# -----------------------------------------------------------------------------#

# Boost internal directory
# -----------------------------------------------------------------------------#
if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
	set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
endif()
set(KIWI_BOOST_INTERN_DIR "${KIWI_DEPENDENCIES_DIR}/boost")

# Local OS Specific Variables
# -----------------------------------------------------------------------------#
set(BOOST_URL_APPLE "http://sourceforge.net/projects/boost/files/boost/1.63.0")
set(BOOST_URL_LINUX "http://sourceforge.net/projects/boost/files/boost/1.63.0")
set(BOOST_URL_WIN32 "http://sourceforge.net/projects/boost/files/boost/1.63.0")

set(BOOST_PKG_NAME_APPLE "boost_1_63_0")
set(BOOST_PKG_NAME_LINUX "boost_1_63_0")
set(BOOST_PKG_NAME_WIN32 "boost_1_63_0")

set(BOOST_PKG_EXT_APPLE "tar.gz")
set(BOOST_PKG_EXT_LINUX "tar.gz")
set(BOOST_PKG_EXT_WIN32 "zip")

set(BOOST_INTERN_INCLUDE_DIR_APPLE "${KIWI_BOOST_INTERN_DIR}")
set(BOOST_INTERN_INCLUDE_DIR_LINUX "${KIWI_BOOST_INTERN_DIR}")
set(BOOST_INTERN_INCLUDE_DIR_WIN32 "${KIWI_BOOST_INTERN_DIR}")

set(BOOST_INTERN_LIBRARIES_DIR_APPLE "${KIWI_BOOST_INTERN_DIR}/stage/lib")
set(BOOST_INTERN_LIBRARIES_DIR_LINUX "${KIWI_BOOST_INTERN_DIR}/stage/lib")
set(BOOST_INTERN_LIBRARIES_DIR_WIN32 "${KIWI_BOOST_INTERN_DIR}/stage64/lib")

set(BOOST_INTERN_CMD_UNPACK_APPLE tar xzf ./)
set(BOOST_INTERN_CMD_UNPACK_LINUX tar xzf ./)
set(BOOST_INTERN_CMD_UNPACK_WIN32 mv 7z x)

set(BOOST_INTERN_CMD_CONFIGURE_APPLE "./bootstrap.sh toolset=clang macosx-version-min=10.9 link=static")
set(BOOST_INTERN_CMD_CONFIGURE_LINUX "./bootstrap.sh toolset=gcc link=static")
set(BOOST_INTERN_CMD_CONFIGURE_WIN32 "bootstrap.bat")

set(BOOST_INTERN_CMD_COMPILE_APPLE "./b2 address-model=64 --with-system stage")
set(BOOST_INTERN_CMD_COMPILE_LINUX "./b2 --with-system stage")
set(BOOST_INTERN_CMD_COMPILE_WIN32 "b2 --toolset=msvc-14.0 -j4 --with-system --stagedir=stage64 variant=release architecture=x86 address-model=64 link=static")

# Local Generic Variables
# -----------------------------------------------------------------------------#
if(APPLE)
	set(BOOST_URL ${BOOST_URL_APPLE})
	set(BOOST_PKG_NAME ${BOOST_PKG_NAME_APPLE})
	set(BOOST_PKG_EXT ${BOOST_PKG_EXT_APPLE})
	set(BOOST_INTERN_INCLUDE_DIR ${BOOST_INTERN_INCLUDE_DIR_APPLE})
	set(BOOST_INTERN_LIBRARIES_DIR ${BOOST_INTERN_LIBRARIES_DIR_APPLE})
	set(BOOST_INTERN_CMD_UNPACK ${BOOST_INTERN_CMD_UNPACK_APPLE})
	set(BOOST_INTERN_CMD_CONFIGURE ${BOOST_INTERN_CMD_CONFIGURE_APPLE})
	set(BOOST_INTERN_CMD_COMPILE ${BOOST_INTERN_CMD_COMPILE_APPLE})
elseif(UNIX)
	set(BOOST_URL ${BOOST_URL_LINUX})
	set(BOOST_PKG_NAME ${BOOST_PKG_NAME_LINUX})
	set(BOOST_PKG_EXT ${BOOST_PKG_EXT_LINUX})
	set(BOOST_INTERN_INCLUDE_DIR ${BOOST_INTERN_INCLUDE_DIR_LINUX})
	set(BOOST_INTERN_LIBRARIES_DIR ${BOOST_INTERN_LIBRARIES_DIR_LINUX})
	set(BOOST_INTERN_CMD_UNPACK ${BOOST_INTERN_CMD_UNPACK_LINUX})
	set(BOOST_INTERN_CMD_CONFIGURE ${BOOST_INTERN_CMD_CONFIGURE_LINUX})
	set(BOOST_INTERN_CMD_COMPILE ${BOOST_INTERN_CMD_COMPILE_LINUX})
elseif(WIN32)
	set(BOOST_URL ${BOOST_URL_WIN32})
	set(BOOST_PKG_NAME ${BOOST_PKG_NAME_WIN32})
	set(BOOST_PKG_EXT ${BOOST_PKG_EXT_WIN32})
	set(BOOST_INTERN_INCLUDE_DIR ${BOOST_INTERN_INCLUDE_DIR_WIN32})
	set(BOOST_INTERN_LIBRARIES_DIR ${BOOST_INTERN_LIBRARIES_DIR_WIN32})
	set(BOOST_INTERN_CMD_UNPACK ${BOOST_INTERN_CMD_UNPACK_WIN32})
	set(BOOST_INTERN_CMD_CONFIGURE ${BOOST_INTERN_CMD_CONFIGURE_WIN32})
	set(BOOST_INTERN_CMD_COMPILE ${BOOST_INTERN_CMD_COMPILE_WIN32})
endif()

set(BOOST_PKG_FILE "${BOOST_PKG_NAME}.${BOOST_PKG_EXT}")
set(BOOST_PKG_PATH "${BOOST_URL}/${BOOST_PKG_FILE}")

# Start the script
# -----------------------------------------------------------------------------#
set(Boost_USE_STATIC_LIBS ON) # Use the static library

# Use the boost internal library
# -----------------------------------------------------------------------------#
if(NOT ${USE_SYSTEM_BOOST})

	# Search for the boost internal library
	# ---------------------------------------------------------------------------#
	set(Boost_INCLUDE_DIR "${BOOST_INTERN_INCLUDE_DIR}")
	set(Boost_LIBRARY_DIR "${BOOST_INTERN_LIBRARIES_DIR}")
	find_package(Boost 1.63.0 COMPONENTS system QUIET)

	# Set up for the boost internal library
	# ---------------------------------------------------------------------------#
	if(NOT Boost_FOUND)
		set(BOOST_FOLDER "boost_1_63_0")

		# Remove the current invalid boost folder
		# -------------------------------------------------------------------------#
		if(EXISTS ${KIWI_BOOST_INTERN_DIR})
			message(STATUS "Remove current invalid boost folder ${KIWI_BOOST_INTERN_DIR}")
			file(REMOVE_RECURSE ${KIWI_BOOST_INTERN_DIR})
		endif()

		# Download the boost package
		# -------------------------------------------------------------------------#
		if(NOT EXISTS "${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_FILE}")
			message(STATUS "Download ${BOOST_PKG_FILE}")
			file(DOWNLOAD ${BOOST_PKG_PATH} "${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_FILE}" SHOW_PROGRESS)
		endif()

		# Unpack the boost package
		# -------------------------------------------------------------------------#
		message(STATUS "Unpack ${BOOST_PKG_FILE} to ${KIWI_BOOST_INTERN_DIR}")
		execute_process(COMMAND ${BOOST_INTERN_CMD_UNPACK}${BOOST_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
		file(RENAME ${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_NAME} ${KIWI_BOOST_INTERN_DIR})

		# Configure the boost library
		# -------------------------------------------------------------------------#
		message(STATUS "Configure the boost library")
		execute_process(COMMAND ${BOOST_INTERN_CMD_CONFIGURE} WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})

		# Compile the boost library
		# -------------------------------------------------------------------------#
		message(STATUS "Compile the boost library")
		execute_process(COMMAND ${BOOST_INTERN_CMD_COMPILE} WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})

		# Find boost library now it is compiled
		# -------------------------------------------------------------------------#
		find_package(Boost 1.63.0 COMPONENTS system)

	endif()

else()

	# Find boost library on the system
	# ---------------------------------------------------------------------------#
	find_package(Boost 1.63.0 COMPONENTS system)

endif(NOT ${USE_SYSTEM_BOOST})

if(Boost_FOUND)
	message(STATUS "Boost include directory: " ${Boost_INCLUDE_DIRS})
	message(STATUS "Boost library directory: " ${Boost_LIBRARY_DIRs})
	message(STATUS "Boost libraries:" ${Boost_LIBRARIES})
else()
	message(SEND_ERROR "Boost not installed")
endif()
message(STATUS "")
