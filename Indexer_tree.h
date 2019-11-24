#ifndef INDEX_INDEXER_TREE_H
#define INDEX_INDEXER_TREE_H
#include "Indexer_fast.h"
int allocNodeCompare( const void *n1, const  void *n2 ) ;
int insert_allocNode( indexer *index, node *p ) ;
void remove_allocNode ( indexer *index, node *p ) ;
node *find_allocNode( indexer *index, int base ) ;
freeLenNode* allocfreeLenNode( int len, node *ptr ) ;
int freeLenNodeCompare( const void *n1, const  void *n2 ) ;
void printTree( const void *n , VISIT order, int level ) ;
int insert_freeLenNode( indexer *index, freeLenNode *p ) ;
void remove_freeLenNode ( indexer *index, freeLenNode *p ) ;
freeLenNode *find_freeLenNode( indexer *index, int len ) ;
freeLenNode *addNodeToTree( indexer *index, node *p ) ;
node *deleteNodeFromTree( indexer *index, node *p) ;

#endif // INDEX_INDEXER_TREE_H

