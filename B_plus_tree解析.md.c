/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
typedef struct node 
+-------------------------------------------+
|  void ** pointers;                        |
|  int * keys;                              |
|  struct node * parent;                    |
|  bool is_leaf;                            |
|  int num_keys;                            |
|  struct node * next; // Used for queue.   |
+-------------------------------------------+

typedef struct record {
	int value;
} record;

main
    node * root;
    insert(root, key, value)
        //just ignore duplicates
        IF find(root, key, false) !=NULL ;then 
            return root
        FI
        
        //create a key value
        record *record_ptr=make_record(value)
            record *new_record=(record*)malloc(sizeof(record))
            new_record->value=value
            return new_record
        
        //init a bplus tree
        IF root==NULL;THEN
            return start_new_tree(key, record_ptr)
                >root=make_leaf()
                    node *leaf=make_node()
                         node *new_node=malloc(sizeof(node))
                         +------------------------+
                         |  void ** pointers;     |malloc(4*sizeof(void *))
                         |  int * keys;           |malloc(4*sizeof(int))
                         |  struct node * parent; |
                         |  bool is_leaf;         |false
                         |  int num_keys;         |0
                         |  struct node * next;   |NULL
                         +------------------------+
                         return new_node
                    leaf->is_leaf=true
                >   return leaf
                +------------------------+                          
                |  void ** pointers;     |malloc(4*sizeof(void *)) =>pointers[0]=record_ptr; [3]=NULL
                |  int * keys;           |malloc(4*sizeof(int))    =>keys[0]=key   
                |  struct node * parent; |                         =>NULL                          
                |  bool is_leaf;         |false          =>true 
                |  int num_keys;         |0                        =>++           
                |  struct node * next;   |NULL                      
                +------------------------+      
         FI   
         
         //case:the tree alread exists                                        
         leaf=find_leaf(root,key,fasle) 
             node *c=root
             while(!c->is_leaf) 
             do
                 i=0
                 while(i<c->num_keys)
                 do
                     IF key>c->kes[i];THEN
                        i++
                     ELSE
                        break
                     FI      
                     c=(node*)c->pointers[i]                                      
                 done
             done
             return c
             
         //case: leaf has room for key and pointer
         IF leaf->num_keys < order -1;THEN
             leaf=insert_into_leaf(leaf,key,pointer)
                 ins_pos=0
                 while ins_pos < leaf->num_keys && leaf->keys[ins_pos] < key
                 do
                     ins_pos++
                 done 
                 
                 FOR (i=leaf->num_keys; i>ins_pos; i--)   ->前插，后移空位
                 do
                     leaf->keys[i] = leaf->keys[i-1]
                     leaf->pointers[i] = leaf->pointers[i-1]
                 done
                 
                 leaf->keys[ins_pos] = key
                 leaf->pointers[ins_pos] = pointer
                 leaf->num_key++
                 return leaf
         FI
         
         //case: leaf must be split
         insert_into_leaf_after_splitting(root, leaf, key, pointer)
             
         
     举例：
     > p 4
     [4 6] 1 ->  查根节点
     Leaf [4 5] -> 查叶子节点
     Record at 2311250 -- key 4, value 4.

     root:4 6
     1 2| 4 5 | 6 8 9
     结构：root={pointers[]={pointers = 0x23111e0, keys = 0x2311190, parent = 0x0, is_leaf = false, num_keys = 2, next = 0x0}
                             pointers:{0x2311030,0x2311270,0x2311110,0x0},
                             keys:{4,6,6,8}, 
     p *(node*)(0x2311030):={pointers = 0x2311080, keys = 0x2311060, parent = 0x23111b0, is_leaf = true, num_keys = 2, next = 0x0}
                             pointers:
                             keys:{1,2}
     p *((node*)0x2311270):= {pointers = 0x23112c0, keys = 0x23112a0, parent = 0x23111b0, is_leaf = true, num_keys = 2, next = 0x0}   
                             pointers:
                             keys:{4,5}   
     p *(node*)0x2311110:={pointers = 0x2311160, keys = 0x2311140, parent = 0x23111b0, is_leaf = true, num_keys = 3, next = 0x0}       
                             pointers:
                             keys:{6,8,9}    
     例子2：
     > t
     23 | 
     6 | 38 54 | 
     4 | 9 | 34 | 44 | 65 87 | 
     1 2 | 4 5 | 6 8 | 9 18 19 | 23 27 28 | 34 35 36 | 38 41 43 | 44 45 | 54 57 | 65 74 77 | 87 90 98 | 
     > p 27
     [23] 1 ->
     [38 54] 0 ->
     [34] 0 ->
     Leaf [23 27 28] ->
     Record at 23116d0 -- key 27, value 27.                                               
