# download sources from git repository

ExternalProject_Add( Download_Externals
	GIT_REPOSITORY		https://github.com/azhirnov/ModularGraphicsFramework-Externals.git
	GIT_TAG				master
	SOURCE_DIR			"${EXTERNALS_PATH}/src"
	BINARY_DIR			""
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
set_property( TARGET "Download_Externals" PROPERTY FOLDER "External" )
