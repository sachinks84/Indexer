#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include <assert.h>
#include<search.h>
#include "Indexer_tree.h"


void *newIndexer( int base, int len ) {
  indexer *ptr = malloc ( sizeof( indexer ) );
  memset(ptr,0, sizeof(indexer) );
  ptr->base = base ;
  ptr->len = len;
  node *freePtr  =  malloc( sizeof (node) );
  memset( freePtr, 0, sizeof(node ) );
  freePtr->len = len;
  freePtr->base = base;
  ptr->list = freePtr;  
  // fast 
  ptr->allocTree = NULL;
  ptr->freeTree = NULL;
  freeLenNode *p = allocfreeLenNode( len, freePtr );
  insert_freeLenNode( ptr,p );
  return (void *)ptr;
}

int  allocIndex( void  *v, int size ) {
   indexer *p = (indexer *)v;
   node *curr = p->list ;
   tree *root = p->freeTree;
   tree *tmp = root ;
   tree *prev = NULL;
   if ( size <= 0 ) return -1;
    if ( root == NULL) return -1;
   //twalk( root, printTree);
   //printf( "walk done\n" );
   // Best fit allocation from the freeLenNode tree
   while( tmp)  {
      freeLenNode *list = tmp->list ;
      node *n = list->list;
      assert( n  );
      if ( n->len > size ) {
         prev = tmp;
         tmp = tmp->left;

      } else if ( n->len < size ) {
         tmp = tmp->right;
         break;
      } else {
         //exact match
         prev = tmp;
         break;
      }
   }
   //printf( "done tree parsing\n");
   if ( !prev  || ( prev->list->len < size ) )  return -1 ;
   freeLenNode *list = prev->list; 
   curr  = list->list;
   //printf( "curr is %x, base %d, len %d \n" , curr, curr->base, curr->len );
   deleteNodeFromTree( p, curr);

   //  break the node if the size is bigger than size
   if ( curr->len > size ) { 

      node *ptr =  malloc( sizeof(node) );
      memset(ptr, 0, sizeof(node) );
      ptr->base = curr->base + size; 
      ptr->len = curr->len - size;
      curr->len = size;
      node *next = NULL; 
      if ( curr->next ) {
         next = NEXT( curr);
         next->prev = ptr;
      }
      curr->next = ptr;
      ptr->next = next;
      ptr->prev = curr;
      addNodeToTree(p,ptr);

      //add ptr ro freeNode tree
   }

   SET_OCC( curr );
   // add curr to allocNode tree
   insert_allocNode( p, curr );
   // add node to occupied list 
   return curr->base;

}

int freeIndex( indexer *v, int base ) {
   indexer *p = ( indexer *)v;
   if ( !p) return -1;
   if ( base < p->base || base > ( p->base + p->len) )
      return -1;
   // find node for this base 
   node *curr = find_allocNode(p, base);
   if  ( !curr ) return -1;
   if ( IS_FREE( curr ) ) {   
      //error
      assert( 0 );
   }
   SET_CLEAR( curr );
   remove_allocNode(p , curr);
   //merge the nodes on the left 
   node *pl = curr->prev;
   node *rn = NEXT(curr);
   if ( pl &&IS_FREE ( pl ) ){
      deleteNodeFromTree( p, pl );
      pl->next = rn; 
      pl->len += curr->len;
      free(curr);
      curr = pl;
      if ( rn ) {
         rn->prev = curr;
      }
   }
   // check if the right node can be merged 
   if ( rn && IS_FREE(rn) ) {
      deleteNodeFromTree( p, rn );
      curr->len += rn->len;
      curr->next = rn->next;
      if ( rn->next ) {
         rn->next->prev = curr;
      }
      free(rn);
   }
   addNodeToTree( p, curr);
   return base;
}

void printIndexer ( void *h ) {
   indexer *handle = ( indexer *)h;
   printf( " allocTree %x freeTree %x\n" , handle->allocTree, handle->freeTree );
   twalk( handle->freeTree, printTree );


}

void printNode ( void *h ) {
   indexer *handle = ( indexer *)h;
   if ( !handle) return;
   node *curr = handle->list;
   while(curr ) {
      int fr = IS_FREE( curr );
      printf ( "curr %x base %d ,Len %d, free %d\n",curr, curr->base, curr->len, fr);
      curr = NEXT( curr  );
   } 

}

#if 0 
int main() {
   int arr[100000];
   int len =5;
   indexer *handle = newIndexer(0,len );
   printf( " allocTree %x freeTree %x \n", handle->allocTree, handle->freeTree);
   int i;
   for ( i =0; i < len ; i++) {
      arr[i] = allocIndex( handle, 1 );
      if ( arr[i]== -1) {
         printf( "failed to allocated mem for %d\n",i);
      }
      assert(arr[i]  != -1);
      //printf(" %d => index %d \n",i, arr[i]);
   }
   for (  i = 0 ; i < len; i++) {
      freeIndex(handle,arr[i]);
   }
   printf("#######\n");
   printNode( handle);
   printf( " allocTree %x freeTree %x \n", handle->allocTree, handle->freeTree);
   return 1;

   int h1 =allocIndex( handle,  20 );
   int h2 =allocIndex( handle,  10 );
   //int h3 = allocIndex( handle,  10 );
   //printf( " %d,%d,%d\n",h1,h2,h3);
   printf ( "alloced h1,h2 %d. %d\n",h1,h2);
   printNode(handle);
   freeIndex(handle,h2);
   printf( "after freeing h2 %d\n",h2);
   freeIndex(handle,h1);
   printf( "after freeing h1 %d\n",h1);
   printNode(handle);

}
#endif
