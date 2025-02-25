#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# @file ostap/parallel/parallel_gaudi.py
# This is a modified verison of the
# original <code>GaudiMP.Parallel</code> module coded by Pere MATO
# @author Pere Mato (pere.mato@cern.ch)
# 
# =============================================================================
"""
This is a modified verison of the `GaudiMP.Parallel` module by Pere MATO

GaudiMP.Parallel module:
- This module provides 'parallel' processing support for GaudiPyhton.
It is adding some sugar on top of public domain packages such as
the 'multiprocessing' or the 'pp' packages. The interface can be made
independent of the underlying implementation package.
Two main class are defined: Task and WorkManager
"""
# =============================================================================
__all__ = (
    'Task'        , ## Base class for Task 
    'WorkManager' , ## Task-manager 
    )
# =============================================================================
import sys, os, time
from   itertools                 import repeat , count
from   ostap.utils.progress_bar  import progress_bar
from   ostap.logger.logger       import getLogger
from   ostap.parallel.task       import Task, TaskManager 
# =============================================================================
logger  = getLogger('ostap.parallel.parallel_gaudi')
# =============================================================================
if ( 3 , 3 ) <= sys.version_info  : from collections.abc import Sized
else                              : from collections     import Sized 
# =============================================================================
import multiprocessing     as MP

# =============================================================================
class pool_context :
    def __init__  ( self , pool ) :
        self.__pool = pool
    def __enter__ ( self ) :
        sys.stdout .flush ()
        sys.stderr .flush ()
        return self.__pool
    def __exit__  ( self, *_ ) :
        self.__pool.close ()
        self.__pool.join  ()
        sys.stdout .flush ()
        sys.stderr .flush ()
 
# =============================================================================
class WorkManager(TaskManager) :
    """ Class to in charge of managing the tasks and distributing them to
        the workers. They can be local (using other cores) or remote
        using other nodes in the local cluster """

    def __init__( self                     , 
                  ncpus     = 'autodetect' ,
                  ppservers = None         ,
                  pp        = False        ,
                  silent    = False        ,
                  progress  = True         , **kwargs ) :

        ##
        if isinstance ( ncpus , int ) and 1 <= ncpus : pass
        else                                         : ncpus = MP.cpu_count()

        if pp        :
            logger.warning ( "WorkManager: option ``pp'' is ignored" )
        if ppservers :
            logger.warning ( "WorkManager: option ``ppservers'' is ignored" )
        
        ## initialize the base class 
        TaskManager.__init__  ( self , ncpus = ncpus , silent = silent , progress = progress )        
        
        self.pool   = MP.Pool ( self.ncpus )

    # =========================================================================
    ## process the bare <code>executor</code> function
    #  @param job   function to be executed
    #  @param jobs_args the arguments, one entry per job 
    #  @return iterator to results 
    #  @code
    #  mgr  = WorkManager  ( .... )
    #  job  = ...
    #  args = ...
    #  for result in mgr.iexecute ( func , args ) :
    #  ...
    #  ... 
    #  @endcode
    #  It is a "bare minimal" interface
    #  - no statistics
    #  - no summary printout 
    #  - no merging of results  
    def iexecute ( self , job , jobs_args , progress = False ) :
        """Process the bare `executor` function
        >>> mgr  = WorkManager  ( .... )
        >>> job  = ...
        >>> args = ...
        >>> for result in mgr.iexecute ( job , args ) :
        ...
        ...
        It is a ``minimal'' interface
        - no statistics
        - no summary prin
        - no merging of results  
        """
                
        with pool_context ( self.pool ) as pool :

            ## create and submit jobs 
            jobs = pool.imap_unordered ( job , jobs_args )
            
            njobs  = len ( jobs_args ) if isinstance ( jobs_args , Sized ) else None            
            silent = self.silent or not progress
            
            ## retrive (asynchronous) results from the jobs
            for result in progress_bar ( jobs , max_value = njobs , silent = silent ) :
                yield result                

    # ========================================================================-
    ## get PP-statistics if/when posisble 
    def get_pp_stat ( self ) : 
        """Get PP-statistics if/when posisble 
        """
        return None 
            
# =============================================================================
if '__main__' == __name__ :
    
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )    
    logger.info ("Module ``%s'' is used for multiprocessing" % MP.__name__ )
        
# =============================================================================
##                                                                      The END 
# =============================================================================
