DEPENDENT_OPTION( BUILD_PERFORMER "Build Performer Components" OFF  "BUILD_AXES;KDL_FOUND" OFF)

#Always process subdirs
ADD_SUBDIRECTORY( test )

IF ( BUILD_PERFORMER )

  # This gathers all the .cpp files into the variable 'SRCS'
  FILE( GLOB SRCS [^.]*.cpp )
  FILE( GLOB HPPS [^.]*.hpp )
  
  GLOBAL_ADD_COMPONENT( orocos-performer ${SRCS} )
  INCLUDE_DIRECTORIES(${PROJ_SOURCE_DIR}/hardware/axes)
  COMPONENT_ADD_DEPS( orocos-performer orocos-axes )
  COMPONENT_ADD_LIBS( orocos-performer orocos-kdl)
  
  IF ( BUILD_COMEDI )
    INCLUDE_DIRECTORIES(${PROJ_SOURCE_DIR}/hardware/comedi)
    COMPONENT_ADD_DEPS( orocos-performer orocos-comedi )
  ENDIF ( BUILD_COMEDI )
  
  GLOBAL_ADD_INCLUDE( ocl ${HPPS})
  
ENDIF ( BUILD_PERFORMER )
