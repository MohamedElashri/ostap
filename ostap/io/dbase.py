#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# @file ostap/io/dbase.py
# 
# Helper module to use databases
#
# @author Vanya BELYAEV Ivan.Belyaev@itep.ru
# @date   2020-05-16
# =============================================================================
""" Helper module to use databases
"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2020-05-16"
__version__ = "$Revision:$" 
# =============================================================================
__all__ = (
    'whichdb'    , ## guess database type  
    'dbopen'     , ## open database
    'Item'       , ## item: named tuple (time,payload)
    'TmpDB'      , ## mixing for tempoirary database 
    )
# =============================================================================
import sys, os, collections
from   ostap.logger.logger  import getLogger
if '__main__' == __name__ : logger = getLogger ( 'ostap.io.dbase' )
else                      : logger = getLogger ( __name__         )
# =============================================================================
from ostap.io.sqlitedict import SqliteDict, issqlite3
# =============================================================================
## named tuple to DB-item: (time, payload)
Item = collections.namedtuple ( 'Item', ( 'time' , 'payload' ) )
# =============================================================================
if sys.version_info < ( 3, 0 ) :
    import anydbm                   as std_db
    from   whichdb import whichdb   as std_whichdb 
else :
    import dbm                      as std_db
    std_whichdb = std_db.whichdb
# =============================================================================
## Check for Berkeley DB
# =============================================================================
use_bsddb3     = False
use_berkeleydb = False

## make a try to use berkeleydb
if  ( 3 , 6 ) <= sys.version_info :
    
    try :        
        import berkeleydb
        use_berkeleydb   = True
        
        berkeleydb_open_mode = {
            'r' : berkeleydb.db.DB_RDONLY ,
            'w' : 0                       ,
            'c' : berkeleydb.db.DB_CREATE , 
            'n' : berkeleydb.db.DB_CREATE
            }
        
        ## open Berkeley DB 
        def berkeleydb_open ( filename                          ,
                              flags    = 'c'                    ,
                              mode     = 0o660                  ,
                              filetype = berkeleydb.db.DB_HASH  ,
                              dbenv    = None                   ,
                              dbname   = None                   ,
                              decode   = lambda s : s           ,
                              encode   = lambda s : s           ) :            
            """ Open Berkeley DB
            """
            assert flags in berkeleydb_open_mode, \
                   "berkeleydb_open: invali dpoe mode %s" % flags
            
            db = berkeleydb.db.DB ( dbenv )
            db.open ( filename , dbname , filetype , berkeleydb_open_mode [ flags ]  , mode )
            
            return db
                
    except ImportError :
        
            berkeleydb      = None 
            use_berkeleydb  = False 

## make a try for dbddb3 
if ( 3 , 3 ) <= sys.version_info < ( 3 , 10 ) : 
    
    try :        
        import bsddb3
        use_bsddb3  = True        
    except ImportError  :        
        bsddb3      = None 
        use_bsddb3  = False 


# =============================================================================
##  Guess which db package to use to open a db file.
#  
#   Return values:
#  - None if the database file can't be read;
#  - empty string if the file can be read but can't be recognized
#  - the name of the dbm submodule (e.g. "ndbm" or "gnu") if recognized.
#   
# Importing the given module may still fail, and opening the
# database using that module may still fail.
# 
#  - Actually it is a bit extended  form of <code>dbm.whichdb</code>
#   that accounts for  <code>bsddb3</code> and <code>sqlite3</code>
def whichdb ( filename  ) :
    """Guess which db package to use to open a db file.
    
    Return values:
    
    - None if the database file can't be read;
    - empty string if the file can be read but can't be recognized
    - the name of the dbm submodule (e.g. 'ndbm' or 'gnu') if recognized.
    
    Importing the given module may still fail, and opening the
    database using that module may still fail.
    
    - Actually it is a bit extended  form of `dbm.whichdb`
    that accounts for `bsddb3` and `sqlite3`
    """
    
    ## use the standard function 
    tst = std_whichdb ( filename  )

    ## identified or non-existing DB  ? 
    if tst or tst is None     : return tst
    
    ## sqlite3 ?
    if issqlite3 ( filename ) : return 'sqlite3'

    import io , struct
    
    try :
        
        with io.open ( filename  ,'rb' ) as f :
            # Read the start of the file -- the magic number
            s16 = f.read(16)
            
    except OSError :
        return None
    
    s = s16[0:4]
    
    # Return "" if not at least 4 bytes
    if len ( s ) != 4:
        return ""

    if s == b'root'  :
        return 'root'

    # Convert to 4-byte int in native byte order -- return "" if impossible
    try:
        ( magic, ) = struct.unpack("=l", s)
    except struct.error:
        return ""

    # Check for GNU dbm
    if magic in (0x13579ace, 0x13579acd, 0x13579acf):
        return "dbm.gnu"

    # Check for old Berkeley db hash file format v2
    if magic in ( 0x00061561 , 0x61150600 ):
        return "bsddb185"

    # Later versions of Berkeley db hash file have a 12-byte pad in
    # front of the file type
    try:
        (magic,) = struct.unpack("=l", s16[-4:])
    except struct.error:
        return ""

    # Check for BSD hash
    if magic in ( 0x00061561 , 0x61150600 ):
        return "berkeleydb" if use_berkeleydb else "bsddb3"

    ## unknown 
    return ""


# =====================================================================
## Open or create database at path given by *file*.
# 
#  Optional argument *flag* can be 'r' (default) for read-only access, 'w'
#  for read-write access of an existing database, 'c' for read-write access
#  to a new or existing database, and 'n' for read-write access to a new
#  database.
# 
#  Note: 'r' and 'w' fail if the database doesn't exist; 'c' creates it
#  only if it doesn't exist; and 'n' always creates a new database.
# 
#  - Actually it is a bit extended  form of <code>dbm.open</code>, that
#    accounts for <code>bsbdb3</code> and <code>sqlite3</code>
def dbopen ( file , flag = 'r' , mode = 0o666 , concurrent = True , **kwargs ):
    """Open or create database at path given by *file*.
    
    Optional argument *flag* can be 'r' (default) for read-only access, 'w'
    for read-write access of an existing database, 'c' for read-write access
    to a new or existing database, and 'n' for read-write access to a new
    database.
    
    Note: 'r' and 'w' fail if the database doesn't exist; 'c' creates it
    only if it doesn't exist; and 'n' always creates a new database.
    
    - Actually it is a bit extended  form of `dbm.open` that  accounts for `bsddb3` and `sqlite3`
    """

    if 'n' in flag and os.path.isfile ( file ) :
        os.unlink ( file ) 
 
    check = whichdb ( file ) if 'n' not in flag  else None

    if 'c' in flag and '' == check :
        check = None 
        os.unlink ( file ) 
        
    # 'n' flag is specified  or dbase does not exist and c flag is specified 
    if 'n' in flag or ( check is None and 'c' in flag ) : 
                
        if concurrent and use_berkeleydb :
            return berkeleydb_open ( file , flag , mode , **kwargs ) 

        if concurrent and use_bsddb3     :
            return bsddb3.hashopen ( file , flag , mode , **kwargs ) 

        if concurrent :
            return SqliteDict ( filename = file , flag = flag , **kwargs )

        return std_db.open ( file , flag , mode ) 

    if use_berkeleydb and check in ( 'berkeleydb' , 'bsddb3' , 'dbhash' ) :
        return berkeleydb_open ( file , flag , mode , **kwargs ) 

    if use_bsddb3     and check in ( 'berkeleydb' , 'bsddb3' , 'bsddb' , 'dbhash' , 'bsddb185' ) :
        return bsddb3.hashopen ( file , flag , mode , **kwargs ) 

    if check == 'sqlite3' :
        return SqliteDict ( filename = file , flag = flag , **kwargs )

    return std_db.open ( file , flag , mode )  
    
# =============================================================================
## get disk size of data-base-like object
#  @code
#  num, size = dbsize ( 'mydb' ) 
#  @endcode  
def dbsize  ( filename  ) :
    """Get disk  size of data-base=like object
    >>> num, size = dbsize ( 'mydb' ) 
    """
    size = 0
    num  = 0

    if os.path.exists ( filename  ) and os.path.isfile ( filename   ) :        
        size += os.path.getsize ( filename  )
        num  += 1
        
    for suffix in ( '.db'  ,
                    '.dir' , '.pag' ,
                    '.bak' , '.dir' , '.dat' ) :
        nfile = filename + suffix 
        if os.path.exists (  nfile ) and os.path.isfile ( nfile ) :
            size += os.path.getsize ( nfile  )
            num  += 1
            
    return num, size 


# ============================================================================
## @class TmpDB
#  Mixin class fo rtemporary databases
class TmpDB(object) :
    """Mixin class for temporary databases
    """

    def __init__ ( self            ,
                   suffix          ,
                   remove  = True  ,
                   keep    = False ) :
        
        self.__keep = True if keep  else False
        
        ## create temporary file name 
        import ostap.utils.cleanup as CU 
        fname = CU.CleanUp.tempfile ( prefix = 'ostap-tmpdb-' ,
                                      suffix = suffix         ,
                                      keep   = self.keep      )
        
        self.__tmp_name = fname        
        self.__remove   = True if ( remove and not self.keep ) else False 
        
    @property
    def tmp_name ( self ) :
        """``tmp_name'' : get the generated temporary file nale
        """
        return self.__tmp_name
    
    @property
    def remove ( self ) :
        """``remove'':  remove the temporary file immediately (just after``clonse''),
        otherwise remove it at the shutdown
        """
        return self.__remove
    
    @property
    def keep   ( self )  :
        """``keep'' keep the file and not delete it
        """
        return self.__keep 
    
    ## remove the file 
    def clean  ( self ) :
        """remove the file
        """
        fname = self.nominal_dbname 
        if self.remove and os.path.exists ( fname ) :
            import ostap.utils.cleanup as CU
            CU.CleanUp.remove_file ( fname ) 
            
# =============================================================================
if '__main__' == __name__ :
    
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )
    
# =============================================================================
##                                                                      The END 
# =============================================================================
        
        
        
        
    
    
    
    
