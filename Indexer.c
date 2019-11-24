#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include <assert.h>


typedef struct node_ {
  int base ; //index
  int len; // len
  struct node_ *prev;
  struct node_ *next; // lets steal a bit here to indicate validate
} node;

#define IS_OCCUPIED( curr )   ( ( (uint32_t)(curr->next) & 0x1 ) == 1 )
#define IS_FREE( curr ) !(IS_OCCUPIED( curr ) )
#define NEXT( curr )  (node *)((uint32_t )curr->next & ~1 )
#define SET_OCC( curr ) curr->next = (node *)((uint32_t)curr->next | 0x1 ) 
#define SET_CLEAR( curr ) curr->next = (node *)( (uint32_t)curr->next & ~0x1 ) 

typedef struct indexer_ {
   int base;
   int len;
   node *list ;
} indexer;

indexer *newIndexer( int base, int len ) {
  indexer *ptr = malloc ( sizeof( indexer ) );
  memset(ptr,0, sizeof(indexer) );
  ptr->base = base ;
  ptr->len = len;
  node *freePtr  =  malloc( sizeof (node) );
  memset( freePtr, 0, sizeof(node ) );
  //ptr->curr = base;
  freePtr->len = len;
  freePtr->base = base;
  ptr->list = freePtr;
  return ptr;
}

int  allocIndex( indexer *p, int size ) {
   node *curr = p->list ;
   while ( curr ) {
      //printf( " base %d len %d free %d\n", curr->base, curr->len, IS_FREE(curr));
      if ( IS_FREE(curr)  && curr->len >= size ) {
         break;           
      }
      curr = NEXT( curr );
   }
   if (!curr) return -1;
   //printf( " curr->base = %d len = %d\n", curr->base, curr->len);

   if ( curr->len > size ) { 

      node *ptr =  malloc( sizeof(node) );
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

   }

   SET_OCC( curr );
   // add to curr to tree/hash
   // add node to occupied list 
   return curr->base;

}

int freeIndex( indexer *p, int base ) {
   if ( !p) return -1;
   if ( base < p->base || base > ( p->base + p->len) )
      return -1;
      // find node for this base 
      node *curr = p->list;
      while ( curr ) {
             if ( curr->base == base  )
             {  break; }

             curr = NEXT( curr );
      }        
      if  ( !curr ) return -1;
      if ( IS_FREE( curr ) ) {   
         //error
         assert( 0 );
      }
      SET_CLEAR( curr );
      //merge the nodes on the left 
        node *pl = curr->prev;
        node *rn = NEXT(curr);
        if ( pl &&IS_FREE ( pl ) ){
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
           curr->len += rn->len;
           curr->next = rn->next;
           if ( rn->next ) {
             rn->next->prev = curr;
           }
           free(rn);
      }
      return base;
}

void printNode ( indexer *handle ) {
   if ( !handle) return;
   node *curr = handle->list;
   while(curr ) {
      int fr = IS_FREE( curr );
      printf ( "Base %d , Len %d : free %d \n",curr->base, curr->len, fr);
      curr = NEXT( curr  );
   } 

}

#if 0
int main() {
   int arr[100000];
   int len =100000;
   indexer *handle = newIndexer(0,len );
   int i;
   int req[] = {  45, 5 ,1 };
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
   return 1;
}
#endif 

