// RUN: not %clang_cc1 %s -fsyntax-only -fmessage-length 75 -o /dev/null 2>&1 | FileCheck %s -strict-whitespace

// Test case for the text diagnostics source column conversion crash.

// There are two problems to be tested:
//
// 1. The assertion in startOfNextColumn() and startOfPreviousColumn()
//    should not be raised when the byte index is greater than the column index
//    since the non-ascii characters may use more than one bytes to store a
//    character in a column.
//
// 2. The length of the caret line should be equal to the number of columns of
//    source line, instead of the length of the source line.  Otherwise, the
//    assertion in selectInterestingSourceRegion will be raised because the
//    removed columns plus the kept columns are not greater than the max column,
//    which means that we should not remove any column at all.

// NOTE: This file is encoded in UTF-8 and intentionally contains some
// non-ASCII characters.

__attribute__((format(printf, 1, 2)))
extern int printf(const char *fmt, ...);

void test1(Unknown* b);  // αααα αααα αααα αααα αααα αααα αααα αααα αααα αααα αααα
// CHECK: unknown type name 'Unknown'
// CHECK-NEXT: void test1(Unknown* b);  // αααα αααα αααα αααα αααα αααα αααα ααα...
// CHECK-NEXT: {{^           \^$}}

void test2(Unknown* b);  // αααα αααα αααα αααα αααα αααα αααα αααα αααα

// CHECK: unknown type name 'Unknown'
// CHECK-NEXT: void test2(Unknown* b);  // αααα αααα αααα αααα αααα αααα αααα αααα αααα
// CHECK-NEXT: {{^           \^$}}

void test3() {
   /* αααα αααα αααα αααα αααα αααα αααα αααα αααα αααα */ printf("%d", "s");
}
// CHECK:       format specifies type 'int' but the argument has type 'char *'
// CHECK-NEXT:   ...αααα αααα αααα αααα αααα αααα αααα αααα αααα */ printf("%d", "s");
// CHECK-NEXT: {{^                                                             ~~   \^~~$}}
// CHECK-NEXT: {{^                                                             %s$}}
