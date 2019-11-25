#ifndef DISKTREE_H
#define DISKTREE_H

#include <fstream>
#include "metadados.h"
#include <string>


typedef struct pair
{
    long chave;
    long  cont;
} Pair;

//using namespace std;
#define M 5  // Order of B-tree: M link fields in each node

enum status {InsertNotComplete, Success, DuplicateKey,
     Underflow, NotFound};

typedef Pair dtype;

struct node {
   int n;        // Number of items stored in a node (n < M)
   dtype k[M-1]; // Data items (only the first n in use)
   long p[M];    // 'Pointers' to other nodes (n+1 in use)  
};

class Btree {
public:
   Btree(std::string TreeFileName);
   ~Btree();
   void insert(dtype x);
   void insert(const char *InpFileName);
   void print(){std::cout << "Contents:\n"; pr(root, 0);}
   void DelNode(dtype x);
   int ShowSearch(dtype *x);
   void preenche_indice_btree(const Metadado& mtd, const std::string& index_path, const std::string& chave, Btree *btree);
private:
   enum {NIL=-1};
   long root, FreeList;
   node RootNode;
   std::fstream file;
   status ins(long r, dtype x, dtype &y, long &u);
   void pr(long r, int nSpace);
   int NodeSearch(dtype x, const dtype *a, int n)const;
   status del(long r, dtype x);
   void ReadNode(long r, node &Node);
   void WriteNode(long r, const node &Node);
   void ReadStart();
   long GetNode();
   void FreeNode(long r);
};

#endif