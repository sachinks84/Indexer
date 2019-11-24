from ctypes import CDLL 
import os
import pdb
import random
import time 
import unittest 

class IndexTester(unittest.TestCase):
   def setUp( self ):
       libfile = os.getcwd() + "/BestFitIndexer.so" 
       self.lib = CDLL( libfile )
       base = self.base = random.randrange(100,500 )
       num = self.num = 100000 #random.randrange( 100,200)
       # Indexer allocates nunbers from base to base + num -1 
       self.handle = self.lib.newIndexer(base,num)

   def testInValidIndex( self ):
       print ( "Invalid size test " )
       handle = self.handle
       self.assertEqual( self.lib.allocIndex( handle, self.num + 1 ), -1 )
   
   def testFullRangeTest( self ):
       print ( "Full size test " )
       handle = self.handle
       self.assertEqual( self.lib.allocIndex( handle, self.num ), self.base )
       self.assertEqual( self.lib.freeIndex( handle, self.base ), self.base )
       
   def testBitAllocTest( self ):
       print( "Single alloc test of %d Indices of 1 unit each" % self.num ) 
       index = {}
       handle = self.handle
       for i in xrange( self.num ):
          index[i ] = self.lib.allocIndex( handle, 1 )
          self.assertEqual( index[i ] , self.base + i ) 
       # check that we have run out of space    
       self.assertEqual( self.lib.allocIndex( handle, 1 ) , -1 )   
       # Free Alloction
       randomIndices = index.values()   
       random.shuffle( randomIndices )
       for i in randomIndices:
           self.assertEqual( self.lib.freeIndex( handle, i ), i )
       #self.lib.printNode( self.handle )
       #self.lib.printIndexer( self.handle )
       self.assertEqual( self.lib.allocIndex( handle, self.num ), self.base )
       self.assertEqual( self.lib.freeIndex(handle, self.base ), self.base )
   
   def testNIndexAlloc( self ):
       # check if the lib returns n indices that add up to num of indices
       numReqs = 20
       print( "%d random requests adding to max indices " % numReqs ) 
       arr = random.sample( xrange (1, self.num + 1 ), numReqs )
       arr.sort()
       prevIdx = 0 
       req = []
       for i in xrange( numReqs ):
          req.append( arr[i] - prevIdx )
          prevIdx = arr[i]
       req.append( self.num - arr[ numReqs - 1 ] )
       #assert sum(req ) == self.num
       index = {}
       handle = self.handle
       for reqLen in  req : 
           idx = self.lib.allocIndex( handle, reqLen )
           assert idx is not -1
           index[ idx ] = reqLen
       self.assertEqual( self.lib.allocIndex( handle, 1 ) , -1 )    
       expectedIdx = self.base
       # check if there is no overlap
       for key in sorted( index.keys() ):
          self.assertEqual( key, expectedIdx )
          expectedIdx += index[ key ]
          #self.assertEqual( self.lib.freeIndex( handle, key ), key )

       # remove fist n requests 
       numReqs = random.randrange( 2,5 )
       keys = sorted( index.keys() )
       requestlen = 0
       for i in xrange( numReqs ):
           self.assertEqual( self.lib.freeIndex( handle, keys[0 ]  ), keys[0] ) 
           requestlen = requestlen + index[ keys[ 0 ] ]
           del index[  keys[ 0 ] ]
           del keys[ 0 ]
       #now check if requestlen indices can be alloced from base
       idx = self.lib.allocIndex( handle, requestlen )
       self.assertEqual( idx, self.base ) 
       index[ idx ] = requestlen

       for key in index.keys():
           self.assertEqual( self.lib.freeIndex( handle, key ), key )  

       self.assertEqual( self.lib.allocIndex( handle, self.num ), self.base )
       self.assertEqual( self.lib.freeIndex(handle, self.base ), self.base )
          
           
class IndexTester2 ( IndexTester ): 
   def setUp( self ):
       libfile = os.getcwd() + "/Indexer.so" 
       self.lib = CDLL( libfile )
       base = self.base = random.randrange(100,500 )
       num = self.num = random.randrange( 1000,2000)
       # Indexer allocates nunbers from base to base + num -1 
       self.handle = self.lib.newIndexer(base,num)
        
unittest.main()
