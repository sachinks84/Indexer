#ifndef INDEX_INDEXER_H
#define INDEX_INDEXER_H

void *newIndexer( int base, int len );
int allocIndex( void  *handle, int size );
int freeIndex( void *handle, int base );
void printNode( void *handle );

#endif // INDEX_INDEXER_H

