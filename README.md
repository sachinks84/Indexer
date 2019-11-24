# Indexer
This is a index allocation library that manages a block of indices 

# Initialize the index manager 
void *newIndexer( int base, int len );

# This API returns the base address of a contiguous block of size indices   
int allocIndex( void  *handle, int size );
# Free the previous alloced index starting at index base 
int freeIndex( void *handle, int base );

# Indexer.so is a simple FCFS index allocator. 

# BestFitIndexer.so uses an enlarged memory foot print to make alloc o log ( n ) /free O ( 1 ) .
# The least free index block greater than or equal to the requested size is allocated 
