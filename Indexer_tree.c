#include "Indexer_fast.h"

int allocNodeCompare( const void *n1, const  void *n2 ) {
 return (( ( node *) n1)->base - ( ( node *)n2)->base );
}

int insert_allocNode( indexer *index, node *p ) {
   void *ptr = tsearch( (void *)p, &(index->allocTree),  allocNodeCompare);
   assert( ptr != NULL );
   return 1;
}
void remove_allocNode ( indexer *index, node *p ) { 
   tdelete((void *)p,&( index->allocTree), allocNodeCompare );
}

node *find_allocNode( indexer *index, int base ) { 
   node tmp;
   tmp.base = base;
   node **ptr = (node ** )tfind( (void *)&tmp, &( index->allocTree ), allocNodeCompare );
   if ( !ptr ) return NULL;
   return *ptr;
}
/////////////////////////////////////////


freeLenNode* allocfreeLenNode( int len, node *ptr ) {
   freeLenNode *p = malloc( sizeof(freeLenNode) );
   memset(p , 0, sizeof( freeLenNode ) );
   if ( !p ) return NULL;
   p->len = len;
   assert( ptr != NULL);
   p->list = ptr;
   return p;
}

int freeLenNodeCompare( const void *n1, const  void *n2 ) {
    return (( ( freeLenNode *) n1)->len - ( ( freeLenNode *)n2)->len );
}

void printTree( const void *n , VISIT order, int level ) {
   freeLenNode *ptr = *(freeLenNode **)n;
   if (order == leaf || order == postorder ) {
      printf(" node->x %d  order -> %d level %d ",ptr->len, order,level);
      node *p = ptr->list;
      while ( p ) {
      printf( "%d," , p->base);
      p = p->freelist;
      }
      printf("\n");
   }
}

int insert_freeLenNode( indexer *index, freeLenNode *p ) {
   assert ( p->list != NULL );
   void *ptr = tsearch( (void *)p, &(index->freeTree), freeLenNodeCompare ); 
   assert( ptr != NULL );
   return 1;
}

void remove_freeLenNode ( indexer *index, freeLenNode *p ) { 
   assert ( p->list == NULL);
   tdelete((void *)p,&( index->freeTree), freeLenNodeCompare );
   free(p);
}

freeLenNode *find_freeLenNode( indexer *index, int len ) { 
   freeLenNode tmp;
   tmp.len = len;
   freeLenNode **ptr = (freeLenNode ** )tfind( (void *)&tmp, &( index->freeTree ), freeLenNodeCompare );
   if ( !ptr ) return NULL;
   return *ptr;
}

freeLenNode *addNodeToTree( indexer *index, node *p ) {
    freeLenNode *ptr = find_freeLenNode( index, p->len ); 
    if ( !ptr ) {
       freeLenNode *ptr = allocfreeLenNode(  p->len, p );
        insert_freeLenNode( index, ptr);
        return ptr;
    }
    p->freelist = ptr->list;
    p->prevFreelist = NULL;
    if ( ptr->list ) {
           ptr->list->prevFreelist = p;
    }
    ptr->list = p;
    return ptr;
}

node *deleteNodeFromTree( indexer *index, node *p) {
   freeLenNode *ptr = find_freeLenNode( index, p->len );
   assert( ptr );
   node *prev = p->prevFreelist;
   node *next = p->freelist;
   if ( !prev ) {
      if ( next ) {
         next->prevFreelist = NULL;
      }
      ptr->list = next;
   }else {
      prev->freelist = next;
      if ( next ) {
         next->prevFreelist = prev;
      }
   }
   if ( ptr->list == NULL ) {
      // we are the only element 
      //printf( "removing freelen node of %d\n",p->len );
      remove_freeLenNode( index, ptr);
   }
   p->prevFreelist = p->freelist = NULL;
   return p;
}

