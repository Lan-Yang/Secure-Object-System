#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool isRoot(void) {return getuid()==0;}

int main(void)
{ uid_t user_id;
  user_id = getuid();
  printf("%u\n",user_id);
  if (isRoot())printf("root\n");
  else printf("other\n");
  return 0;
}
