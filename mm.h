#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <cstdint>
#include <cstring>

#define MM_MAX_STRUCT_NAME 32

struct vm_page;

struct vm_page_family
{
  char struct_name[MM_MAX_STRUCT_NAME];
  uint32_t struct_size;
  vm_page *first_page;
};

struct vm_page_for_families
{
  vm_page_for_families *next;
  vm_page_family families[0];
};

#define MAX_FAMILIES_PER_VM_PAGE (SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families *)) / sizeof(vm_page_family)

typedef enum
{
  MM_FALSE,
  MM_TRUE,
} vm_bool_t;

struct block_meta_data
{
  vm_bool_t is_free;
  uint32_t block_size;
  uint32_t offset;
  struct block_meta_data *prev;
  struct block_meta_data *next;
};

struct vm_page
{
  struct vm_page *next;
  struct vm_page *prev;
  struct vm_page_family *pg_family; /*back pointer*/
  block_meta_data meta_block;
  char page_memory[0]; // First data block in VM page
};

#define offset_of(container_structure, field_name) ((size_t) & (((container_structure *)0)->field_name))

#define MM_GET_PAGE_FROM_META_BLOCK(block_meta_data_ptr) ((void *)((char *)block_meta_data_ptr - block_meta_data_ptr->offset))

#define NEXT_META_BLOCK(block_meta_data_ptr) (block_meta_data_ptr->next)

#define NEXT_META_BLOCK_BY_SIZE(block_meta_data_ptr) (block_meta_data *)((char *)(block_meta_data_ptr + 1) + block_meta_data_ptr->block_size)

#define PREV_META_BLOCK(block_meta_data_ptr) (block_meta_data_ptr->prev)

#define MARK_VM_PAGE_EMPTY(vm_page_ptr) \
  vm_page_ptr->meta_block.next = NULL;  \
  vm_page_ptr->meta_block.prev = NULL;  \
  vm_page_ptr->meta_block.is_free = MM_TRUE;

#define mm_bind_blocks_for_allocation(allocated_meta_block, free_meta_block) \
  free_meta_block->prev = allocated_meta_block;                              \
  free_meta_block->next = allocated_meta_block->next;                        \
  allocated_meta_block->next = free_meta_block;                              \
  if (free_meta_block->next)                                                 \
    free_meta_block->next->prev = free_meta_block;

#define ITERATE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr, curr)       \
  {                                                                       \
    uint32_t count = 0;                                                   \
    for (curr = (vm_page_family *)&vm_page_for_families_ptr->families[0]; \
         curr->struct_size && count < MAX_FAMILIES_PER_VM_PAGE;           \
         curr++, count++)

#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr, curr) }

#endif // VM_PAGE_H
