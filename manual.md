
## Intro
Cake works as an extension for MSVC on Windows and as an extension for GCC on Linux.
This approach makes Cake useful in real and existing programs. 

When applicable, Cake uses the same command line options of MSVC and GCC.

## Static analyzer
For static analyzer concepts of ownership and nullable pointers visit  [ownership](ownership.html) 

## Include directories

On Windows, Cake can be used on the command line similarly to MSVC.
Cake reads the `INCLUDE` variable, the same variable used by MSVC to locate the include directories.

Additionally, you can run Cake outside the Visual Studio command prompt by placing the file `cakeconfig.h` in 
the same directory or above the source files, and specifying the directories using #pragma dir.

If Cake doesn't find `cakeconfig.h` in the local directories, it will try to locate it in the 
same path as the Cake executable.

The **-autoconfig** option generates the `cakeconfig.h` automatically on both Windows and Linux.

To manually discover which directories are included, you can run the command:


```
echo %INCLUDE%
```

at Visual Studio command prompt.

To find out what are the directories used by GCC type:

```
echo | gcc -E -Wp,-v -
```
  
Sample of cakeconfig.h

```c

#ifdef __linux__
/*
   To find the include directories used my GCC type:   
   echo | gcc -E -Wp,-v -
*/
#pragma dir "/usr/lib/gcc/x86_64-linux-gnu/11/include"
#pragma dir "/usr/local/include"
#pragma dir "/usr/include/x86_64-linux-gnu"
#pragma dir "/usr/include"

#endif

#ifdef _WIN32
/*
   To find the include directories used my  MSVC,
   open Visual Studio Developer Commmand prompt and type:
   echo %INCLUDE%.
   Running Cake inside mscv command prompt uses %INCLUDE% automatically.
*/
#pragma dir "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.38.33130/include"
#pragma dir "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.38.33130/ATLMFC/include"
#pragma dir "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/VS/include"
#pragma dir "C:/Program Files (x86)/Windows Kits/10/include/10.0.22000.0/ucrt"
#pragma dir "C:/Program Files (x86)/Windows Kits/10/include/10.0.22000.0/um"
#pragma dir "C:/Program Files (x86)/Windows Kits/10/include/10.0.22000.0/shared"
#pragma dir "C:/Program Files (x86)/Windows Kits/10/include/10.0.22000.0/winrt"
#pragma dir "C:/Program Files (x86)/Windows Kits/10/include/10.0.22000.0/cppwinrt"
#pragma dir "C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/include/um"

#endif

```

Sample, project `cakeconfig.h`

```c

//system includes...etc
#include "C:\Program Files (x86)\cake\cakeconfig.h"

//project extra includes
#pragma dir ".\openssl\include"

```


## Command line

```
cake [options] source1.c source2.c ...

SAMPLES

    cake source.c
    Compiles source.c and outputs /out/source.c

    cake -target=C11 source.c
    Compiles source.c and outputs C11 code at /out/source.c

cake file.c -o file.cc && cl file.cc
    Compiles file.c and outputs file.cc then use cl to compile file.cc

cake file.c -direct-compilation -o file.cc && cl file.cc
    Compiles file.c and outputs file.cc for direct compilation then use cl to compile file.cc
  
```

### OPTIONS

#### -I  (same as GCC and MSVC)
Adds a directory to the list of directories searched for include files

####  -no-output
Cake will not generate output

#### -D (same as GCC and MSVC)
Defines a preprocessing symbol for a source file
#### -E (same as GCC and MSVC)
Copies preprocessor output to standard output

#### -o name.c (same as GCC and MSVC)
  Defines the output name. used when we compile one file
  
#### -remove-comments      
Remove all comments from the output file

#### -direct-compilation   
output code as compiler sees it without macros.

#### -target=standard      
Output target C standard (c89, c99, c11, c23, c2y, cxx)
C99 is the default and C89 (ANSI C) is the minimum target

#### -dump-tokens            
Output tokens before preprocessor

#### -fi
Format input (format before language conversion)

#### -fo
Format output (format after language conversion, result parsed again)

#### -ir
Generates C89 code

#### -Wname -Wno-name  (same as GCC)   
Enables or disable warnings.
See [warnings](warnings.html)

#### -disable-assert
disable cake extension where assert is an statement. See extensions

#### -showIncludes
Causes the compiler to output a list of the include files. The option also displays nested include files, that is, the files included by the files that you include.

#### -Wall
Enables all warnings

#### -sarif               
Generates sarif files.
Sarif Visual Studio plugin https://marketplace.visualstudio.com/items?itemName=WDGIS.MicrosoftSarifViewer

#### -sarif-path               
Specifies the Sarif output dir.

Inside "Visual Studio -> External Tools" this command can be used for static analysis.

`-Wstyle  -msvc-output  -no-output -sarif -sarif-path "$(SolutionDir).sarif" $(ItemPath)´


#### -msvc-output          
Output is compatible with visual studio IDE. We can click on the error message and IDE selects the line. 

### -fanalyzer
This option enables an static analysis of program flow. This is required for some
ownership checks

### -auto-config

Generates cakeconfig.h header. 

On Windows, it must be generated inside the Visual Studio Command Prompt to read the INCLUDE variable.
On Linux, it calls GCC with echo | gcc -v -E - 2>&1 and reads the output.

## Output

One directory called **out** is created keeping the same directory structure of the input files.

For instance:

```c
cake c:\project\file1.c
```

output:

```
  c:\project
  ├── file1.c
  ├── out
      ├── file1.c
```

More files..

```
cake c:\project\file1.c c:\project\other\file2.c
```

output

```
  c:\project
  ├── file1.c
  ├── other
  │   ├── file2.c
  ├── out
      ├── file1.c
      ├── other
          ├── file2.c
```


## Pre-defined macros

```c
 #define __CAKE__ 202311L
 #define __STDC_VERSION__ 202311L
 #define __STDC_OWNERSHIP__ 1
```

The define __STDC_OWNERSHIP__ indicates that the compiler suports owneship checks


### Pre-defined macros for MSVC compatibility
https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170#standard-predefined-macros

### Pre-defined macros for GCC compatibility
https://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html


## C99 Transformations

C89 
https://port70.net/~nsz/c/c89/c89-draft.html

C99
https://open-std.org/JTC1/SC22/WG14/www/docs/n1124.pdf

```c
 #define __STDC_VERSION__ 199901L  //C99
```

### C99 restrict pointers

```c
void f(const char* restrict s);
```

Becomes in C89

```c
void f(const char* /*restrict*/ s);
```

N448

###  C99 Variable-length array (VLA) 

The idea is not implement variable length arrays with automatic storage duration. (\_\_STDC\_NO\_VLA\_\_ 1). 

But there are other uses of VLA.


```c
#include <stdlib.h>
#include <stdio.h>

int main() {
	int n = 2;
	int m = 3;
	int (*p)[n][m] = malloc(sizeof * p);

	printf("%zu\n", sizeof(*p));

	free(p);
}

```

Becomes C89 (not implemented)

```c
#include <stdlib.h>
#include <stdio.h>

int main() {
	int n = 2;
	int m = 3;
    
    /*these variables are created to store the dynamic size*/
    const int vla_1_n = n;
	const int vla_1_m = m;
    
	int (*p)[n][m] = malloc((vla_1_n*vla_1_m)*sizeof(int));

	printf("%zu\n", (vla_1_n*vla_1_m)*sizeof(int));

	free(p);
}

```

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n683.htm

### C99 Flexible array members

```c
struct s {
    int n;
    double d[]; 
};
```

Becomes (not implemented)

```c
struct s {
    int n;
    double d[1];
};

//TODO sizeof
```


### C99 static and type qualifiers in parameter array declarators

```c
#include <stdlib.h>

void F(int a[static 5]) {
}

int main() 
{    
    F(0);
    F(NULL);
    F(nullptr);

    int a[] = {1, 2, 3};    
    F(a);//error
    
    int b[] = { 1, 2, 3 , 4, 5};
    F(b); 

    int c[] = { 1, 2, 3 , 4, 5, 6};
    F(c);
}

```
  
Cake verifies that the argument is an array with at 
least the specified number of elements. 
It extends this check to arrays without a static size as well.

### C99 Complex and imaginary support
Not implemented

### C99 Universal character names (\u and \U)
Not implemented

### C99 Hexadecimal floating constants

```c
double d = 0x1p+1;
```
  
Becomes in C89

```c
double d = 2.000000;
```

Cake converts hexadecimal floating-point values to decimal floating-point 
representation using strtod followed by snprintf. 
This conversion may introduce precision loss.

### C99 Compound literals

```c
struct s {
  int i;
};

int f(void) {
  struct s * p = 0, * q;
  int j = 0;
  again:
    q = p, p = & ((struct s) { j++ });
  if (j < 2) goto again;
  return p == q && q -> i == 1;
}
```

Becomes in C89

```c
struct s {
  int i;
};
int f(void) {
  struct s * p = 0, * q;
  int j = 0;
  again:
    struct s compound_literal_1 = { j++ };
    q = p, p = & compound_literal_1;
  if (j < 2) goto again;
  return p == q && q -> i == 1;
}
```

N716
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n716.htm

### C99 Designated initializers

```c
 int main()
 {
  int a[6] = {[4] = 29, [2] = 15 };

  struct point { int x, y; };

  struct point p = { .y = 2, .x = 3 }
 }

```

Becomes C89

```c
int main()
{
  int a[6] = { 0, 0, 15, 0, 29, 0 };
  struct point { int x, y; };
  struct point p = { 3, 2 }
}
```

N494
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n494.pdf

### C99 Line comments
When compiling to C89 line comments are converted to 
/*comments*/.

### C99 inline functions
TODO
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n741.htm

### C99 _Pragma preprocessing operator
TODO 

### C99 \_\_func\_\_ predefined identifier
Parsed. C89 conversion not implemented yet.

###  C99 Variadic macros

We need to expand the macro when comping to C89.
This is covered by # macro expand.

Sample:

```c

#include <stdio.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)
#pragma expand debug

int main()
{
  int x = 1;
  debug("X = %d\n", 1);
}
```
  
Becomes

```c
#include <stdio.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)
#pragma expand debug

int main()
{
  int x = 1;
  fprintf(stderr, "X = %d\n", 1);
}
```

I am considering to mark the debug macro to be expanded automatically
if \_\_VA\_ARGS\_\_ is used. Then pragma expand will not be necessary.

N707
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n707.htm


###  C99 \_Bool
When compiling to C89 _Bool is replaced by unsigned char.

```c
//line comments
int main(void)
{
    _Bool b = 1;
    return 0;
}
```

Becomes in C89

```c
/*line comments*/
int main(void)
{
    unsigned char b = 123;
    return 0;
}
```

Alternative design - typedef ?
Considering C23 has bool and the objective of C89 version is to have a version that compiles in C++ the best option would be use bool, true, false.

Obs:
 Currently cake is not converting 123 to 1 as required by C standard.


## C11 Transformations

```c
#define __STDC_VERSION__ 201112L //C11
```


https://open-std.org/JTC1/SC22/WG14/www/docs/n1570.pdf

https://files.lhmouse.com/standards/ISO%20C%20N2176.pdf

###  C11 \_Static\_assert

When compiling to versions < C11 \_Static\_Assert is removed.

### C11 Anonymous structures and unions

It is implemented, however the conversion to C99, C89 was not implemented yet.

```c
struct v {
  union { /* anonymous union*/
     struct { int i, j; }; /* anonymous structure*/
     struct { long k, l; } w;
  };
  int m;
} v1;

int main(){
  v1.i = 2; /* valid*/
  v1.w.k = 5; /* valid*/
}
```


### C11 \_Noreturn

```c
_Noreturn void f () {
  abort(); // ok
}
```

Becomes in < C11

```c
/*[[noreturn]]*/ void f () {
  abort(); // ok
}
```

C23 attribute [[noreturn]] provides similar semantics. The _Noreturn function specifier is an obsolescent feature

###  C11 Thread_local/Atomic
Parsed but not transformed.

###  C11 type-generic expressions (\_Generic)

When compiling to C99, C89 we keep the expression that matches the type.

For instance:

The expression that matches the argument 1.0 is **cbrtl**.

The result of \_Generic in C99 will be cbrtl. Because this is inside a macro we need to tell the transpiler to expand that macro using pragma expand.

N1441
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1441.htm

```c
#include <math.h>

#define cbrt(X) _Generic((X),    \
                  double: cbrtl, \
                  float: cbrtf , \
                  default: cbrtl \
              )(X)

#pragma expand cbrt

int main(void)
{
    cbrt(1.0);
}
```

Becomes in C99, C89

```c
#include <math.h>

#define cbrt(X) _Generic((X),    \
                  double: cbrtl, \
                  float: cbrtf , \
                  default: cbrtl \
              )(X)

#pragma expand cbrt

int main(void)
{
     cbrtl(1.0);
}
```

###  C11 u' ' U' ' character constants

```c
 int i = U'ç';
 int i2 = u'ç';
```

Becomes in < C11

```c
 int i = 231u;
 int i2 = ((unsigned short)231);
```

Important: Cake assume source is utf8 encoded.

###  C11 u8"literals"

u8 literals are converted to escape sequences.

```c
char * s1 = u8"maçã";
char * s2 = u8"maca";
```

Becomes in < C11

```c
char * s1 = "ma\xc3\xa7\xc3\xa3";
char * s2 = "maca";
```

N1488
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1488.htm

Important: Cake assume source is utf8 encoded.

### C11 _Alignof or C23 alignof

When compiling to C99 or C89 it is replaced by the equivalent constant.

```c
 int main()
 {
   int align = alignof(int);
 }
```

Becomes < C11

```c
 int main()
 {
   int align = 4;
 }
```

TODO considering a macro. For instance, ALIGNOF_INT

### C11 _Alignas or C23 alignas

Not implemented. 

## C23 Transformations

https://open-std.org/JTC1/SC22/WG14/www/docs/n3096.pdf

```c
#define __STDC_VERSION__ 201710L  //C17
#define __STDC_VERSION__ 202311L  //C23
```

###  C23 \_Decimal32, \_Decimal64, and \_Decimal128
Not implemented.
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1107.htm

### C23 static\_assert / single-argument static_assert
In C23 static\_assert is a keyword and the text message is optional.

Whe comping to C11, static\_assert is replaced by it C11 version \_Static\_assert. If the static\_assert has only one argument the text becomes "error".

N1330
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1330.pdf

```c
int main()
{    
    static_assert(1 == 1, "error message");
    static_assert(1 == 1);
}

```
  
Becomes in C11

```c
int main()
{    
    _Static_assert(1 == 1, "error message");
    _Static_assert(1 == 1, "error");
}
```
  
In < C11 it is replaced by one space;

###  C23 u8 character prefix

Implemented.
https://open-std.org/JTC1/SC22/WG14/www/docs/n2418.pdf

```c
int main(){
    unsigned char c = u8'~';
}
```

When compiling to < C23 becomes.

```c
int main(){
    unsigned char c = ((unsigned char)'~');
}
```


### C23 No function declarators without prototypes

Implemented.
https://www.open-std.org/JTC1/SC22/WG14/www/docs/n2841.htm

```c
int main(){
    func(); //this is an error in C23
}
```


See also Remove support for function definitions with identifier lists  

https://open-std.org/JTC1/SC22/WG14/www/docs/n2432.pdf


### C23 Improved Tag Compatibility

Not implemented yet.
  
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3037.pdf
  
```c
struct foo { int a; } p;
void bar(void)
{
  struct foo { int a; } q;
  q = p;
}
```

Becomes < C23

```c
struct foo { int a; } p;
void bar(void)
{
  struct foo  q;
  q = p;
}
```

### C23 Unnamed parameters in function definitions
  
```c
int f(int );

int f(int ) {
}
```

https://open-std.org/JTC1/SC22/WG14/www/docs/n2480.pdf

Cake should add a dummy name when generating C < 23. (Not implemented yet)

### C23 Digit separators

```c
int main()
{
    int a = 1000'00;
}
```
  
Becomes in < C23

```c
int main()
{
    int a = 100000;
}  
```

This transformation happens at token level, so even preprocessor and inactive blocks are transformed.

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2626.pdf

### C23 Binary literals 

```c
#define X  0b1010

int main()
{
    int a = X;
    int b = 0B1010;
}

```
Becomes in C11, C99, C89

```c
#define X  0xa

int main()
{
    int a = X;
    int b = 0xa;
}

```


This transformation happens at token level, so even preprocessor and inactive blocks are transformed.

### C23 Introduce the nullptr constant

```c

int main()
{
  void * p = nullptr;
  auto p2 = nullptr;
  typeof(nullptr) p3 = nullptr;
}

```

Becomes in < C23

```
int main()
{
  void * p = ((void*)0);
  void  * p2 = ((void*)0);
  void  * p3 = ((void*)0);
}

```

https://open-std.org/JTC1/SC22/WG14/www/docs/n3042.htm

### C23 Make false and true first-class language features

When compiling to C89 bool is replaced by unsigned char,  true by 1 and false by 0. 

When compiling to C99 and C11 bool is replaced with **_Bool**, true is replaced with `((_Bool)1)` 
and false with **(_Bool)0)**

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2935.pdf

###  C23 {} empty initializer

```c

int main()
{
    struct X {
        int i;
    } x = {};

    x = (struct X) {};

    struct Y
    {
        struct X x;
    } y = { {} };
}  

```

Becomes in < C23

```c

int main()
{
    struct X {
        int i;
    } x = {0};

    x = (struct X) {0};

    struct Y
    {
        struct X x;
    } y = { {0} };
}

```

> Note: Cake code is 100% equivalent because it does not make padding bit zero.

###  C23 auto

```c
static auto a = 3.5;
auto p = &a;

double A[3] = { 0 };
auto pA = A;
auto qA = &A;
```

Becomes < C23

```c
static double a = 3.5;
double  * p = &a;

double A[3] = { 0 };
double  * pA = A;
double  (* qA)[3] = &A;
```

https://open-std.org/JTC1/SC22/WG14/www/docs/n3007.htm


###  C23 typeof / typeof_unqual


```c

#define SWAP(a, b) \
  do {\
    typeof(a) temp = a; a = b; b = temp; \
  } while (0)

#pragma expand SWAP

int main()
{
    /*simple case*/
    int a = 1;
    typeof(a) b = 1;

    /*pay attention to the pointer*/
    typeof(int*) p1, p2;

    /*let's expand this macro and see inside*/
    SWAP(a, b);

    /*for anonymous structs we insert a tag*/
    struct { int i; } x;
    typeof(x) x2;
    typeof(x) x3;

   /*Things get a little more complicated*/
   int *array[2];
   typeof(array) a1, a2;
   
   typeof(array) a3[3];
   typeof(array) *a4[4];

   /*abstract declarator*/
   int k = sizeof(typeof(array));

   /*new way to declare pointer to functions?*/
   typeof(void (int)) * pf = NULL;
}

```
  
Becomes in < C23

```c


#define SWAP(a, b) \
  do {\
    typeof(a) temp = a; a = b; b = temp; \
  } while (0)

#pragma expand SWAP

int main()
{
    /*simple case*/
    int a = 1;
    int  b = 1;

    /*pay attention to the pointer*/
    int  *p1,  *p2;

    /*let's expand this macro and see inside*/
     do {int temp = a; a = b; b = temp; } while (0);

    /*for anonymous structs we insert a tag*/
    struct _anonymous_struct_0 { int i; } x;
    struct _anonymous_struct_0  x2;
    struct _anonymous_struct_0  x3;

   /*Things get a little more complicated*/
   int *array[2];
   int  *a1[2],  *a2[2];
   
   int  *(a3[3])[2];
   int  *(*a4[4])[2];

   /*abstract declarator*/
   int k = sizeof(int*[2]);
   
   /*new way to declare pointer to functions?*/
   void  (*pf)(int) = ((void*)0);
}
```

https://open-std.org/JTC1/SC22/WG14/www/docs/n2927.htm
https://open-std.org/JTC1/SC22/WG14/www/docs/n2930.pdf

### C23 Improved Normal Enumerations

//TODO

https://open-std.org/JTC1/SC22/WG14/www/docs/n3029.htm

```c
enum a {
	a0 = 0xFFFFFFFFFFFFFFFFULL
};

static_assert(_Generic(a0,
		unsigned long long: 0,
		int: 1,
		default: 2 == 0));
```

The type of the enum must be adjusted.


###  C23 constexpr 

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3018.htm

Cake convert constexpr declarator with a cast and the value.
addressof constexpr declarator is not implemented.

```c
#include <stdio.h>

constexpr int c = 123;

int a[c];

constexpr double PI = 3.14;

static_assert(PI + 1 == 3.14 + 1.0);

int main()
{
   printf("%f", PI);
}

```
  
Becomes < C23

```c
#include <stdio.h>

const int c = 123;

int a[((int)123)];

const double PI = 3.14;

int main()
{
   printf("%f", ((double)3.140000));
}
```
  
TODO: Maybe suffix like ULL etc makes the code easier to read.

###  C23 Enhancements to Enumerations

```c
enum X : short {
  A
};

int main() {
   enum X x = A;   
}
```

Becomes < C23

```c
enum X {
  A
};

int main() {
   short x = ((short)A);   
}
```

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3030.htm


###  C23 Attributes

Conversion to < C23  will just remove the attributes.

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2335.pdf
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2554.pdf

Related: Standard Attributes in C and C++ - Timur Doumler - ACCU 2023
https://youtu.be/EpAEFjbTh3I

### C23 fallthrough attribute
Not implemented

https://open-std.org/JTC1/SC22/WG14/www/docs/n2408.pdf

### C23 deprecated attribute

Partially implemented
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2334.pdf

### C23 maybe_unused attribute

Implemented
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2270.pdf

### C23 nodiscard attribute
Partially implemented

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2267.pdf

https://open-std.org/JTC1/SC22/WG14/www/docs/n2448.pdf

### C23 [[unsequenced]] and [[reproducible]]

//TODO

https://open-std.org/JTC1/SC22/WG14/www/docs/n2956.htm

###  C23 \_\_has\_attribute

Its is implemented in cake.
Conversion < C23 not defined. Maybe a define.

###  C23 \_\_has\_include

```c

#if __has_include(<stdio.h>)
#warning  YES
#endif

#if __has_include(<any.h>)
#warning  YES
#else
#warning  NO
#endif

```

Its is implemented in cake.
Conversion < C23 not defined. 


###  C23 \#warning
  
When compiling to versions < 23 the line is commented out.

```c
int main()
{
  #warning my warning message  
}
```

When target < C23 becomes

```c
int main()
{
  /* #warning my warning message */  
}
```

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2686.pdf

###  C23 \#embed

Partially implemented.

```c
#include <stdio.h>

int main()
{
  static const char file_txt[] = {
   #embed "stdio.h"
   ,0
  };

  printf("%s\n", file_txt);
}
```

Becomes in < C23

```c

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3017.htm

#include <stdio.h>

int main()
{
  static const char file_txt[] = {
    35,112,114,/*lot more here ...*/ 13,10
   ,0
  };

  printf("%s\n", file_txt);
}

```

I am considering add an option to generate a file with a suffix
like "embed_stdio.h" then the equivalent code will be:

Becomes in < C23

```c
#include <stdio.h>

int main()
{
  static const char file_txt[] = {
   #include "embed_stdio.h"
   ,0
  };

  printf("%s\n", file_txt);
}
```



###  C23 \#elifdef \#elifndef

```c
#define Y

#ifdef X
#define VERSION 1
#elifdef  Y
#define VERSION 2
#else
#define VERSION 3
#endif
```

Becomes < C23

```c
#define Y

#ifdef X
#define VERSION 1
#elif defined   Y
#define VERSION 2
#else
#define VERSION 3
#endif

```
  
###  C23 \_\_VA_OPT\_\_
Implemented.
Requires #pragma expand.

```c

#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })
#define EMP

/*maybe this could be automatic if <C23*/
#pragma expand F
#pragma expand G
#pragma expand SDEF
#pragma expand EMP

void f(int i, ...) {}


int main()
{
  int a = 1;
  int b = 2;
  int c = 3;
  
  F(a, b, c);
  F();
  F(EMP);
  G(a, b, c);
  G(a, );
  G(a);

}

```


Becomes in < C23

```c

#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })
#define EMP

/*maybe this could be automatic if <C23*/
#pragma expand F
#pragma expand G
#pragma expand SDEF
#pragma expand EMP

void f(int i, ...) {}


int main()
{
  int a = 1;
  int b = 2;
  int c = 3;
  
   f(0, a, b, c);
   f(0 );
   f(0);
   f(0, a, b, c);
   f(0, a );
   f(0, a );

}
```

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3033.htm

###  C23 BitInt(N))

Not implemented

### C23 Compound Literals with storage specifier
  
Not implemented yet.

```c
void F(int *p){}

int main()
{
   F((static int []){1, 2, 3, 0})
}
```

Becomes (not implemented yet)

```c
void F(int *p){}

int main()
{
    static int _compound_1[] = {1, 2, 3, 0};
    F(_compound_1);
x   }
```

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3038.htm

### C23 Variably-modified (VM) types

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2778.pdf

## C2Y Transformations

### Obsolete implicitly octal literals


```c

static_assert(0o52 == 052);
static_assert(0O52 == 052);
static_assert(0O52 == 42);

int main()
{
    int i = 0o52;
}

```

Becomes in < C2Y (prefix is removed)

```c

static_assert(052 == 052);
static_assert(052 == 052);
static_assert(052 == 42);

int main()
{
    int i = 052;
}

```


###  Extension - defer

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3199.htm

*defer* will call the defer statement before the block exit at inverse order of declaration.

```
     defer-statement:
        defer secondary-block
```

For instance:

```c
#include <stdio.h>

int main() {
  do {
     FILE* f = fopen("in.txt", "r");
     if (f == NULL) break;
     defer fclose(f);

     FILE* f2 = fopen("out.txt", "w");
     if (f2 == NULL) break;
     defer fclose(f2);
     //...    
  }
  while(0);
}
```

Becomes in < C2Y

```c
#include <stdio.h>

int main() {
  do {
     FILE* f = fopen("in.txt", "r");
     if (f == ((void*)0)) break;

     FILE* f2 = fopen("out.txt", "w");
     if (f2 == ((void*)0)) {  fclose(f); break;}
     
     fclose(f2); fclose(f);
   }
  while(0);
}
```

###  Extension - if with initializer

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3196.htm

```c
#include <stdio.h>
int main()
{
   int size = 10;
   if (FILE* f = fopen("file.txt", "r"); f)
   {
     /*...*/
     fclose(f);
   }
}
```
Becomes in < C2Y

```c
#include <stdio.h>

int main()
{
   int size = 10;
   {FILE* f = fopen("file.txt", "r");if ( f)
   {
     /*...*/
     fclose(f);
   }}
}
```

C++ proposal
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r0.html

### Extension typename on _Generic

This feature was created in Cake and now it is part of C2Y!

https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3260.pdf

```c
 int main()
{
    const int * const p;
    static_assert(_Generic(p, const int *: 1));

    /*extension*/
    static_assert(_Generic(int, int : 1));
    static_assert(_Generic(typeof(p), const int * const: 1));
}
```


## Cake Extensions (Not in C23, C2Y)

###  Extension - try catch throw

```
   try-statement:
      try secondary-block
      try secondary-block catch secondary-block   
```

```
jump-statement:
  throw;
```

try catch is a external block that we can jump off.

try catch is a **LOCAL jump** this is on purpose not a limitation.

catch block is optional.

```c
try
{
   for (int i = 0 ; i < 10; i++) {
      for (int j = 0 ; j < 10; j++) {
        ... 
        if (error) throw;
        ...
      }
   }
}
catch
{
}
```

###  Extension Literal function - lambdas

Lambdas without capture where implemented using a syntax similar of compound literal for function pointer.

Lambdas are the most complex code transformation so far because sometimes function scope types needs to be transformed to file scope. This is important because manual lambda capture
is something we want to use in function scope.

For instance:

```c
extern char* strdup(const char* s);
void create_app(const char* appname)
{
  struct capture {
     char * name;
  } capture = { .name = strdup(appname) };

  (void (void* p)) {
    struct capture* capture = p;    
  }(&capture); 
}
```
Because struct capture was in function scope and the lambda function will be created at file scope the type **struct capture** had to be moved from function scope to file scope.

```c
extern char* strdup(const char* s);

struct _capture0 {
     char * name;
  };
  
void _lit_func_0(void *p) {
    struct _capture0* capture = p;    
  }

void create_app(const char* appname)
{
  struct _capture0  capture = { .name = strdup(appname) };
  _lit_func_0(&capture);  
}
```

### Extension #pragma dir  

```c 
#pragma dir "C:/Program Files (x86)/Windows Kits/10//include/10.0.22000.0/cppwinrt"
```  
  
pragma dir makes the preprocessor include the directory when searching for includes.

### Extension #pragma expand

pragma expand tells the C back-end to not hide macro expansions. This is necessary when
the compiler makes changes inside macro expanded code.

For instance:


```c

#define SWAP(a, b) \
    do { \
      typeof(a) temp = a; a = b; b = temp; \
    } while(0)

#pragma expand SWAP

int main()
{
   int a = 1;
   typeof(a) b = 2;
   SWAP(a, b);
   return 1;
}
```

Becomes

```c
#define SWAP(a, b) \
    do { \
      typeof(a) temp = a; a = b; b = temp; \
    } while(0)

#pragma expand SWAP

int main()
{
   int a = 1;
   int b = 2;
    do {int temp = a; a = b; b = temp; } while(0);
   return 1;
}

```


### Type traits

We have some compile time functions to infer properties of types.

```c

_is_char()
The three types char, signed char, and unsigned char are collectively called the character types.

_is_pointer
Pointer to object or function

_is_array
Array type

_is_function
A function type describes a function with specified return type. 

_is_floating_point
float, double, and long double return true

_is_integral
The standard signed integer types and standard unsigned integer types are collectively called the
standard integer types;

_is_arithmetic
Integer and floating types are collectively called arithmetic types. 

_is_scalar
Arithmetic types, pointer types, and the nullptr_t type are collectively called scalar types

```
Note: Type traits that can be easily created with \_Generic will be removed.
_
### Extension - Object lifetime checks

See [ownership](ownership.html)


### Extension assert built-in

In cake assert is an built-in function.
The reason is because it works as tips for flow analysis.

For instance, in a linked list when `head` is null `tail` is also null,
and `tail->next` always points to null.

Assertion will check these properties in runtime and also make 
the static analysis assume that assert evaluates to true.

```c

void list_push_back(struct list* list,
                    struct item* _Owner p_item)
{
   if (list->head == NULL) {
      list->head = p_item;
   }
   else {
      assert(list->tail != nullptr);
      assert(list->tail->next == nullptr);
      list->tail->next = p_item;
   }
   list->tail = p_item;
}
```


However, `assert` is not a "blind override command." In situations like:

```c
    int i = 0;
    assert(i != 0);
```

In situations where static analysis can identify two or more possible states, 
assert works as a state selector, similar to what happens in if statements but without the scope.

```c
    void f(int * _Opt p)
    {
        if (p != NULL) {
           //p is not null here...
        }
    }
    
    void f2(int * _Opt p)
    {
        assert(p != NULL);
        //we assume p is not null here...        
    }
```







