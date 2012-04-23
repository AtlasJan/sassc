#include <cstdio>
#include <iostream>
#include "prelexer.hpp"

using namespace Sass::Prelexer;
using std::cout;
using std::endl;

void print_slice(const char *s, const char *t) {
  if (t) {
    printf("succeeded with %ld characters:\t", t - s);
    while (s < t) putchar(*s++);
    putchar('\n');
  }
  else {
    printf("failed\n");
  }
}

#define check_twice(matcher, inp1, inp2) \
  (printf("Attempting to match %s\n", #matcher), \
   print_slice(inp1, matcher(inp1)), \
   print_slice(inp2, matcher(inp2)), \
   putchar('\n'))

char num1[]     = "123foogoo";
char num2[]     = ".456e7";
char num3[]     = "-23.45";
char ident1[]   = "-webkit-box-sizing: border-box;";
char interp1[]  = "#{$stuff + $more_stuff}";
char atkwd1[]   = "@media screen blah blah { ... }";
char idnm1[]    = "#-blah_hoo { color: red; }";
char classnm1[] = ".hux-boo23 { color: blue; }";
char percent1[] = "110%; color: red; }";
char dim1[]     = "12px; color: blue; }";
char uri1[]     = "url(  'www.bork.com/bork.png' )";
char ident2[]   = "url ()// not a uri!";
char func1[]    = "lighten(indigo, 20%)";
char exact1[]   = "='blah'] { ... }";
char inc1[]     = "~='foo'] { ... }";
char dash1[]    = "|='bar'] { ... }";
char pre1[]     = "^='hux'] { ... }";
char suf1[]     = "$='baz'] { ... }";
char sub1[]     = "*='bum'] { ... }";
char ws1[]      = "  /* hello */\t\n//blah\n  /*blah*/ significant";
char hex1[]     = "#1a2b3c#f1ab";
char hex2[]     = "#abc-zippo";
char nonhex1[]  = "#ab blah";
char nonhex2[]  = "#abc123blah";
char var1[]     = "$hello blah";
char nonvar1[]  = "$ hello";
char anc1[]     = "  div {";
char nonanc1[]  = "  { blah";
char bi1[]      = "+2n + 42";
char bi2[]      = "23n+1";
char nonbi1[]   = "- n+2";
char fld1[]     = "blah/bloo/foo.txt";
char fld2[]     = "/bloo/blee";
char nonfld1[]  = "blah.txt";
char imp1[]     = "! /*hey*/ important;";
char nonimp1[]  = "!imblah";

extern const char slash_star[] = "/*";

prelexer ptr = 0;
template <prelexer mx>
void try_and_set(char* src) {
  const char* p = mx(src);
  if (p) ptr = mx;
}

prelexer ancestor = sequence< ancestor_of, negate< exactly<'{'> > >;


int main() {
  
  prelexer p = exactly<'x'>;
  prelexer q = exactly<'x'>;
  
  if (p == q) {
    cout << "Hey, we can compare instantiated functions! And these are the same!" << endl;
  }
  
  check_twice(identifier, ident1, num1);
  check_twice(interpolant, interp1, idnm1);
  check_twice(at_keyword, atkwd1, classnm1);
  check_twice(id_name, idnm1, interp1);
  check_twice(class_name, classnm1, num2);
  check_twice(number, num1, ident1);
  check_twice(number, num2, classnm1);
  check_twice(number, num3, ident1);
  check_twice(percentage, percent1, dim1);
  check_twice(dimension, dim1, percent1);
  check_twice(uri, uri1, ident2);
  check_twice(functional, func1, ident2);
  check_twice(exact_match, exact1, inc1);
  check_twice(class_match, inc1, pre1);
  check_twice(dash_match, dash1, suf1);
  check_twice(prefix_match, pre1, dash1);
  check_twice(suffix_match, suf1, pre1);
  check_twice(substring_match, sub1, suf1);
  check_twice(spaces_and_comments, ws1, num1);
  check_twice(hex, hex1, nonhex1);
  check_twice(hex, hex2, nonhex2);
  check_twice(variable, var1, nonvar1);
  check_twice(ancestor_of, anc1, nonanc1);
  check_twice(binomial, bi1, nonbi1);
  check_twice(binomial, bi2, nonbi1);
  check_twice(folder, fld1, nonfld1);
  check_twice(folder, fld2, nonfld1);
  check_twice(folders, fld1, nonfld1);
  check_twice(folders, fld2, nonfld1);
  check_twice(important, imp1, nonimp1);
  cout << count_interval<'\n'>(ws1, spaces_and_comments(ws1)) << endl;
  cout << count_interval<'*'>(ws1, spaces_and_comments(ws1)) << endl;
  cout << count_interval<exactly<slash_star> >(ws1, spaces_and_comments(ws1)) << endl;
  putchar(*(find_first<'{'>(pre1))), putchar('\n');
  
  try_and_set<exactly<'-'> >(ident1);
  prelexer ptr1 = ptr;
  try_and_set<exactly<'@'> >(atkwd1);
  prelexer ptr2 = ptr;
  cout << ptr << endl;
  if (ptr1 == ptr2) cout << "This shouldn't be the case!" << endl;
  else cout << "The prelexer pointers are different!" << endl;
  
  //ptr == prelexer(exactly<'x'>);

  return 0;
}