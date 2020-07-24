/*
 * Exercise 8-7.
 * 
 * malloc accepts a size request without checking its plausibility, free 
 * believes that the block it is asked to free contains a valid size field.  
 * Improve these routines so they tak more pains with error checking
 */

/* 
  * No.  My environmnet (not sure if it's the linux or compiler) supports 2008,
  * which doesn't support sbrk so the text's code won't run.  And it's Friday.
  * But here are some thoughts:

malloc
======
As the size is unsigned the only way I can see a value is implausible is if it's
excessively large, so malloc could impose a size limit or morecore could limit
the number of units.

free
====
We can check that the size and the address are both multiples the header size.
But that is quite a weak test because the header is quite small.  We can also
check that not more than any maximum we have for malloc.
*/