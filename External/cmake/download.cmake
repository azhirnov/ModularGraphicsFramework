# download sources from git repository

ExternalProject_Add( "External.Download"
	# download
	DOWNLOAD_DIR		"${EXTERNALS_PATH}"
	GIT_REPOSITORY		https://github.com/azhirnov/ModularGraphicsFramework-Externals.git
	GIT_TAG				ca59680a37ea47d19b103cff22fca2b734f1154e	#master
	EXCLUDE_FROM_ALL	1
	LOG_DOWNLOAD		1
	# update
	#UPDATE_COMMAND		""
	PATCH_COMMAND		""
	UPDATE_DISCONNECTED	1
	LOG_UPDATE			1
	# configure
	SOURCE_DIR			"${EXTERNALS_PATH}"
	CONFIGURE_COMMAND	""
	# build
	BINARY_DIR			""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
set_property( TARGET "External.Download" PROPERTY FOLDER "External" )
