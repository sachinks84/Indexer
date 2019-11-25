# Indexer
This is an index allocation library that manages a block of indices.


## This project contains 2 Index manager libraries 
* Indexer.so  
* BestFitIndexer.so -> faster version and avoids fragmentation 


### Initialize the index manager 
```
void *newIndexer( int base, int len );
```

### This API returns the base address of a contiguous block of length size   
```
int allocIndex( void  *handle, int size );
```

### Free the previous alloced index starting at index base 
```
int freeIndex( void *handle, int base );
```
### Performance of BestFitIndexer on x86 XENON CPU
* allocation time of 1 million indices ~ 1 second  
* deletion time for 1 million indices in random order ~ 1.8 seconds 

