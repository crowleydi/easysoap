noinst_PROGRAMS = validator

validator_SOURCES = \
    validator.cpp UserlandValidatorHandler.cpp WhichToolkitHandler.cpp 
validator_LDADD = $(STATIC_EASYSOAP_LIBS) $(NETWORK_LIBS) $(SSL_LIBS)
validator_DEPENDENCIES = $(STATIC_EASYSOAP_LIBS)
INCLUDES = $(EASYSOAP_INCLUDES)

EXTRA_DIST = README.TXT UserlandValidatorHandler.h WhichToolkitHandler.h 
