 

# -------------------------------------------------------------------------
# - mygsl
#

if ( BAYEUX_WITH_MYGSL )
  set ( _mygsl_DIR ${_install_prefix}/${CMAKE_INSTALL_LIBDIR}/cmake/mygsl )
  ExternalProject_Add( mygsl 
    DEPENDS datatools
    SVN_REPOSITORY ${BAYEUX_COMPONENTS_SVN_BASE_URL}/mygsl/${BAYEUX_MYGSL_PATH}
    SVN_USERNAME ${_svn_username} 
    SVN_PASSWORD ${_svn_password}
    TIMEOUT 10
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${_install_prefix} 
   	 -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
	 -DMYGSL_WITH_DEBUG=0 
	 -DMYGSL_WITH_DOC=${BAYEUX_WITH_DOCS} 
	 -DMYGSL_WITH_TEST=${BAYEUX_WITH_TESTS} 
         -Ddatatools_DIR:PATH=${_datatools_DIR}
    CMAKE_GENERATOR "Unix Makefiles"
    BUILD_COMMAND make -j${BAYEUX_PARALLEL_JOBS}
    INSTALL_COMMAND make install
  )
endif ( BAYEUX_WITH_MYGSL ) 

