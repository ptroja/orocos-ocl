IF ( BUILD_TESTS)
IF ( BUILD_KUKA_TEST )

    INSTALL_FILES( /cpf [^.]*.cpf)  
    INSTALL_FILES( /cpf [^.]*.dtd)  

ENDIF ( BUILD_KUKA_TEST )
ENDIF ( BUILD_TESTS)
