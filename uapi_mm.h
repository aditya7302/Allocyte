#ifndef __UAPI_MM__
#define __UAPI_MM__

#include <cstdint>
#include <string>

void mm_init();

void mm_instantiate_page_family(const char *struct_name, uint32_t struct_size);

#define MM_REG_STRUCT(struct_name) (mm_instantiate_page_family(#struct_name, sizeof(struct_name)))

#endif /*__UAPI_MM__ */
