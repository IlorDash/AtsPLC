#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_PRIV_INCLUDEDIRS := params MigrateAnyData DataMigrations HttpServer
COMPONENT_SRCDIRS := . MigrateAnyData HttpServer