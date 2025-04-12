#include <stdio.h>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

#include "mm.h"

#include <bits/stdc++.h>
using namespace std;

static vm_page_for_families *first_vm_page_for_families = NULL;
static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init()
{
  SYSTEM_PAGE_SIZE = getpagesize();
}

static inline uint32_t
mm_max_page_allocatable_memory(int units)
{
  return (uint32_t)((SYSTEM_PAGE_SIZE * units) - offset_of(vm_page, page_memory));
}

static void *mm_get_new_vm_page_from_kernel(int units)
{
  void *vm_page = mmap(0, units * SYSTEM_PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, 0, 0);

  if (vm_page == MAP_FAILED)
  {
    std::cerr << "Error: VM page allocation failed" << std::endl;
    return nullptr;
  }

  std::memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
  return vm_page;
}

/*Function to return a page to kernel*/
static void mm_return_vm_page_to_kernel(void *vm_page, int units)
{

  if (munmap(vm_page, units * SYSTEM_PAGE_SIZE))
  {
    cout << "Error : Could not munmap VM page to kernel" << endl;
  }
}

vm_page *allocate_vm_page(vm_page_family *page_family)
{
  vm_page *page = (vm_page *)mm_get_new_vm_page_from_kernel(1);

  MARK_VM_PAGE_EMPTY(page);

  page->meta_block.block_size = mm_max_page_allocatable_memory(1);
  page->meta_block.offset = offset_of(vm_page, meta_block);

  page->next = NULL;
  page->prev = NULL;

  page->pg_family = page_family;

  if (page_family->first_page == NULL)
  {
    page_family->first_page = page;
    return page;
  }

  page->next = page_family->first_page;
  page_family->first_page->prev = page;
  page_family->first_page = page;
  return page;
}

void deallocate_vm_page(vm_page *page)
{
  vm_page_family *family = page->pg_family;

  if (family->first_page == page)
  {
    family->first_page = page->next;
    if (page->next)
    {
      page->next->prev = NULL;
    }
    page->next = NULL;
    page->prev = NULL;
    mm_return_vm_page_to_kernel((void *)page, 1);
    return;
  }

  if (page->next)
  {
    page->next->prev = page->prev;
  }
  page->prev->next = page->next;
  mm_return_vm_page_to_kernel((void *)page, 1);
}

void mm_instantiate_page_family(const char *struct_name, uint32_t struct_size)
{
  vm_page_family *page_family_curr = NULL;
  vm_page_for_families *page_families_curr = NULL;

  if (struct_size > SYSTEM_PAGE_SIZE)
  {
    cout << "Error : Structure size exceeds the system page size" << endl;
    return;
  }

  if (first_vm_page_for_families == NULL)
  {
    first_vm_page_for_families = (vm_page_for_families *)mm_get_new_vm_page_from_kernel(1);
    first_vm_page_for_families->next = NULL;
    strncpy(first_vm_page_for_families->families[0].struct_name, struct_name, MM_MAX_STRUCT_NAME);
    first_vm_page_for_families->families[0].struct_size = struct_size;
    return;
  }

  uint32_t count = 0;

  ITERATE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, page_family_curr)
  {
    if (strncmp(page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME) != 0)
    {
      count++;
      continue;
    }
    assert(0); // Structure already exists
  }
  ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, page_family_curr);

  if (count == MAX_FAMILIES_PER_VM_PAGE)
  {
    page_families_curr = (vm_page_for_families *)mm_get_new_vm_page_from_kernel(1);
    page_families_curr->next = first_vm_page_for_families;
    first_vm_page_for_families = page_families_curr;
    page_family_curr = &first_vm_page_for_families->families[0];
  }

  strncpy(page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME);
  page_family_curr->struct_size = struct_size;
}

static void mm_union_free_blocks(block_meta_data *first, block_meta_data *second)
{
  assert(first->is_free == MM_TRUE && second->is_free == MM_TRUE);

  first->block_size += second->block_size + sizeof(block_meta_data);

  first->next = second->next;

  if (second->next)
  {
    second->next->prev = first;
  }
}

vm_bool_t mm_is_vm_page_empty(vm_page *page)
{
  if (page->meta_block.next == NULL && page->meta_block.prev == NULL && page->meta_block.is_free == MM_TRUE)
  {
    return MM_TRUE;
  }
  return MM_FALSE;
}

// int main()
// {
//   mm_init();
//   cout << "VM page size = " << SYSTEM_PAGE_SIZE << std::endl;

//   void *addr1 = mm_get_new_vm_page_from_kernel(1);
//   void *addr2 = mm_get_new_vm_page_from_kernel(1);

//   cout << "page 1 = " << addr1 << ", page 2 = " << addr2 << std::endl;

//   return 0;
// }
