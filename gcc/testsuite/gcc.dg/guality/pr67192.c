/* PR debug/67192 */
/* { dg-do run } */
/* { dg-options "-g -Wmisleading-indentation" } */

volatile int cnt = 0;

__attribute__((noinline, noclone)) static int
last (void)
{
  return ++cnt % 5 == 0;
}

__attribute__((noinline, noclone)) static void
do_it (void)
{
  asm volatile ("" : : "r" (&cnt) : "memory");
}

__attribute__((noinline, noclone)) static void
f1 (void)
{
  for (;; do_it())
    {
      if (last ())
	break;
    }
  do_it (); /* { dg-final { gdb-test 27 "cnt" "5" } } */
}

__attribute__((noinline, noclone)) static void
f2 (void)
{
  while (1)
    {
      if (last ())
	break;
      do_it ();
    }
  do_it (); /* { dg-final { gdb-test 39 "cnt" "10" } } */
}

void (*volatile fnp1) (void) = f1;
void (*volatile fnp2) (void) = f2;

int
main ()
{
  asm volatile ("" : : "r" (&fnp1) : "memory");
  asm volatile ("" : : "r" (&fnp2) : "memory");
  fnp1 ();
  fnp2 ();
  return 0;
}
