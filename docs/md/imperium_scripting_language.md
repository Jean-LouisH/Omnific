# Imperium Scripting Language


## Primitive Data Types

Imperium has strong typing with the following data types:

### Void placeholder
`void`

### Booleans
`boolean` / `bool`

### Natural numbers / Unsigned integers
`natural8`, `natural16`, `natural32`, `natural64`, `natural` / `nat`

### Signed Integers
`integer8`, `integer16`, `integer32`, `integer64`, `integer` / `int`

### Characters
`ascii`, `utf8`, `utf16`

### Floating Point
`float32`, `float64`, `float`

Data types with unspecified sizes such as `natural`, `integer`, and `float` are at the target precision of 32-bits.

## Procedures

```cpp
integer example():
	boolean test_boolean := false
	natural test_natural_number := 3
	integer test_integer := -40
	ascii test_ascii_letter := 'Q'
	utf8 test_utf8 := '€'
	utf16 test_utf16 := '😏'
	float32 test_real_value := 3.14159f
    return test_integer
```

## Control Structures

```
if a > maximum:
    maximum = a
elif a < minimum:
    minimum = a
else 
    other = a
```

```
for item in items:
    total += item
```

## Pointers and Arrays

A pointer is represented with `*` on the declared type only. The address of a variable with `&`.

```cpp
integer value := 2
integer* value_pointer := &value
```

Pointers can be dereferenced by:

```cpp
*value_pointer := 5
```

An array with a statically specified size is declared with the `[]` by the data type. Elements are accessed from 0 to the maximum number - 1 from the variable name. The separation between the declaration's brackets and the dereferencing bracket is meant to prevent confusion to beginners.

```cpp
void example():
	integer[15] numbers
	numbers[0] := 4
	integer a := numbers[0] + numbers[14]
```

## Memory Management

`allocate()`, `clear_allocate()`, `reallocate()`, `deallocate()`

```cpp
void example():

	natural* array1 := allocate(natural[5])
	
	natural* array2 := clear_allocate(natural[5])

	natural* array1 := reallocate(array1, natural[10])
	
	deallocate(array1)
	deallocate(array2)
```


Data Structure
---
```
data RigidBody
    natural64 id
	ascii[10] name
	float32 mass
	float32 xPosition
	float32 yPosition
```
	
```
void example()
	RigidBody ball
	ball.id := 32
	ball.name := "ball"
	ball.mass := 6.088
	ball.xPosition := 5.01
	ball.yPosition := 1.56
```