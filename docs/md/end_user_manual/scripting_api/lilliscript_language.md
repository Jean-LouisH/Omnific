# LilliScript Language

## Data Types

### Void placeholder
`void`

### Booleans
`bool`, `boolarr8`, `boolarr16`, `boolarr32`, `boolarr64`, `boolarr`

### Natural numbers / Unsigned Integers
`nat8`, `nat16`, `nat32`, `nat64`, `nat`

### Integers, Signed
`int8`, `int16`, `int32`, `int64`, `int`

### Text
`ascii`, `utf8`, `utf16`

### Floating Point
`float32`, `float64`, `float`

LilliScript is a strongly statically typed language. From declaration, primitive and object variables have fixed types assigned to them until they are deleted. They are declared ascii

```cpp

bool example := true
```

## Built-in Classes

`String`

## Built-in API Namespaces

`File`
`Input`
`Log`
`Render`
`Scene`
`Thread`
`Time`
`Window`

## Literals



## Operators

### Arithmetic

`+`
`-`
`*`
`/`
`%`
`**`

### Assignment

`:=`
`+=`
`-=`
`*=`
`/=`
`%=`
`**=`
`>>=`
`&=`
`^=`
`|=`

### Increment and Decrement

`++`
`--`

### Relational

`=`
`!=`
`>`
`<`
`>=`
`<=`

### Logical

`not` or `!`
`and` or `&&`
`or` or `||`

## Conditional

`?`

## Comma

`,`

### Bitwise

`&`
`|`
`^`
`>>`
`<<`

## Data Structures

```cpp
data Apple
	ascii[10] name
	float32 mass
	float32 xPosition
	float32 yPosition
```

## Classes

```cpp
class Banana
```

## Control Structures

### If, Else, Else-If Statement

```cpp
if (condition)
	statement1()
elseif
	statement2()
else
	statement3()
```

To place the statement inline 

`if (condition) then execute()`

### For Loops

### While Loops

### Do While Loops

### Switch-Case

### Jump statements

`break`
`continue`

## Template


## Memory Management

* Garbage collector

`alloc`, `realloc`

```cpp
void example()

	nat[] array1 := alloc(nat[5])
	nat[] array1 := realloc(array1, nat[10])
	nat[] array2 := alloc(uninit nat[5])
```

## Procedures

```cpp
float32 example(int a, int b)
	return a + b
```

## Design by Contract

`precond`, `postcond`, `assert`
