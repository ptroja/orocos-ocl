
# This gathers all the .cpp files into the variable 'SRCS'
FILE( GLOB HPPS [^.]*.hpp )
FILE( GLOB SRCS [^.]*.cpp )

INCLUDE_DIRECTORIES ( ${BOOST} ${BOOST_OPTIONS} )

GLOBAL_ADD_COMPONENT( orocos-ocl-common ${SRCS} )
GLOBAL_ADD_INCLUDE( ocl ${HPPS})
     
