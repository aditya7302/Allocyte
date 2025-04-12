#include "uapi_mm.h"

struct emp
{
  char name[32];
  uint32_t emp_id;
};

struct student
{
  char name[32];
  uint32_t rollno;
  uint32_t marks_phy;
  uint32_t marks_chem;
  uint32_t marks_maths;
  struct student *next;
};

int main(int argc, char **argv)
{
  mm_init();
  MM_REG_STRUCT(emp);
  MM_REG_STRUCT(student);
  return 0;
}
