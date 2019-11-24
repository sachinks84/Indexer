#ifndef INDEX_INDEXER_FAST_H
#define INDEX_INDEXER_FAST_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include <assert.h>
#include<search.h>
typedef struct node_ {
  int base ; //index
  int len; // len
  struct node_ *prev;
  struct node_ *next; // lets steal a bit here to indicate occupied 
  struct node_ *freelist ;  // pointer to next avaliable free node of same len #Fast
  struct node_ *prevFreelist;
} node;

#define IS_OCCUPIED( curr )   ( ( (intptr_t)(curr->next) & 0x1 ) == 1 )
#define IS_FREE( curr ) !(IS_OCCUPIED( curr ) )
#define NEXT( curr )  (node *)((intptr_t )curr->next & ~1 )
#define SET_OCC( curr ) curr->next = (node *)((intptr_t)curr->next | 0x1 ) 
#define SET_CLEAR( curr ) curr->next = (node *)( (intptr_t)curr->next & ~0x1 ) 

typedef struct indexer_ {
   int base;
   int len;
   node *list ;
   void *freeTree;
   void *allocTree;
} indexer;


// Node in freeTree; 
typedef struct  {
   int len;       // key to index freeListNodes tree by size
   node *list ;
} freeLenNode;


typedef struct tree_ {
   freeLenNode *list;
   struct tree_ *left;
   struct tree_ *right;
} tree;

#endif // INDEX_INDEXER_FAST_H

