#include <unistd.h>

typedef struct header {
  size_t size;
  struct header *next;
} Header;

struct queue {
  Header *front, *back;
} list;

#define HEADER_SIZE sizeof(Header)
#define NALLOC 1024
#define NUNITS(bytes) (((bytes + HEADER_SIZE - 1) / HEADER_SIZE) + 1)
#define MEM_BLOCK(blk) ((void *) blk + 1)

Header base;
Header *root = NULL;

unsigned long long get_currently_allocated_size () {

  int bytes = 0, nunits = 0;
  Header *head = list.front;

    while (head) {
      nunits = head->size;
      bytes += nunits - HEADER_SIZE - 1;
      head = head->next;
    }
  return bytes;
}
void *mem_alloc (size_t size) {

  Header *increase_heap (size_t nunits);
  Header *curr, *prev;
  size_t nunits = size + HEADER_SIZE + 1;

  if (root == NULL) {
    base.next = root = &base;
    base.size = 0;
  }

  prev = root;
  curr = prev->next;

  while (curr) {

    if (curr == root)
        curr = increase_heap (nunits);

      if (curr->size > nunits) {

          if (curr->size ==nunits) {
              prev->next = curr->next;

          } else {
              curr->size -= nunits;
              curr += curr->size;
              curr->size = nunits;
          }

          if (list.front == NULL)
              list.back = list.front = curr;
          else
              list.back = list.back->next = curr;

          root = prev;

        return MEM_BLOCK(curr);
     }

      prev = curr;
      curr = curr->next;
  }
}
Header *increase_heap (size_t nunits) {

  void *freemem;
  Header *block;

  if (nunits < NALLOC)
    nunits = NALLOC;

  freemem = (void *) sbrk (nunits * HEADER_SIZE);

  if (freemem == (void *) -1)
    return NULL;

  block = (Header *) freemem;
  block->size = nunits;
  block->next = root;

  root->next = block;

  return block;
}
void mem_free (void* p) {

  Header *prev = list.front;
  Header *block = p - 1;

  if (list.front == block)
      list.front = block->next;

  if (list.back == block)
      list.back = block->next;

  block->next = root->next;
  root->next = block;

}
