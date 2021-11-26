#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# Copyright Ostap developers
# =============================================================================
#                                   1.5.99.0 (Sep 13, 2020, 13:46 [UTC])
#     .oooooo.                .                        
#    d8P'  `Y8b             .o8                        
#   888      888  .oooo.o .o888oo  .oooo.   oo.ooooo.  
#   888      888 d88(  "8   888   `P  )88b   888' `88b 
#   888      888 `"Y88b.    888    .oP"888   888   888 
#   `88b    d88' o.  )88b   888 . d8(  888   888   888 
#    `Y8bood8P'  8""888P'   "888" `Y888""8o  888bod8P' 
#                                            888       
#                                           o888o      
#                                                    
#  Simple interactive PyRoot-based analysis environment to provide access
#  to zillions useful decorators for ROOT (and not only ROOT!) objects&classes
# 
# - https://github.com/OstapHEP/ostap
# - https://ostaphep.github.io/ostap-tutorials
# - https://github.com/OstapHEP/ostap-tutorials
# 
# =============================================================================
__all__ = (
    'banner'       , ## Ostap banner 
    'version'      , ## Ostap version 
    'version_int'  , ## Ostap version as integer 
    'version_info' , ## version info as named tuple
    'build_date'   , ## Ostap build/reelase date (as string)
    'build_time'   , ## Ostap build/release time (as datetime)
    )
# =============================================================================
## the  actual version of Ostap 
__version__ = "1.5.99.0"
__date__    = "Sep 13, 2020, 13:46 [UTC]"
# =============================================================================
import datetime 
from   collections import namedtuple
# =============================================================================
version      = __version__
VersionInfo  = namedtuple("VersionInfo", ('major','minor','patch','tweak' ) )
version_info = VersionInfo ( 1 ,
                             5 ,
                             99 ,
                             0 )
build_date   = __date__
build_time   = datetime.datetime.utcfromtimestamp ( 1600004784 )
version_int  = version_info.tweak                   +\
               version_info.patch             * 100 +\
               version_info.minor       * 100 * 100 +\
               version_info.major * 100 * 100 * 100 
               
# =============================================================================
##
import ostap.fixes.fixes
# =============================================================================
## Banner
banner = r"""
                                        1.5.99.0 (Sep 13, 2020, 13:46 [UTC])
     .oooooo.                .
    d8P'  `Y8b             .o8
   888      888  .oooo.o .o888oo  .oooo.   oo.ooooo.
   888      888 d88(  "8   888   `P  )88b   888' `88b
   888      888 `"Y88b.    888    .oP"888   888   888
   `88b    d88' o.  )88b   888 . d8(  888   888   888
    `Y8bood8P'  8""888P'   "888" `Y888""8o  888bod8P'
                                            888
                                           o888o
                                           
 - https://github.com/OstapHEP/ostap
 - https://ostaphep.github.io/ostap-tutorials
 - https://github.com/OstapHEP/ostap-tutorials
"""
   
# =============================================================================
##                                                                      The END 
# =============================================================================
