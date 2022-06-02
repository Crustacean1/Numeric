# Simple arithmetic library
## Usage
`cmake -S ./ -B ./build`/
`cmake --build ./build`
## Test file format description
Quite self-explanatory - look at examples in tests/
## Buffer behavior
- Buffers are wrappers around underlying number representation, containing its size and pointer to data
- Copy operator does not transfer ownership, both buffers will refer to the same buffer
- In order to copy buffer content, initialize empty buffer with appropriate size and call copy with target buffer as parameter 
## Math operations
All math operations return integer with same size as left operand.
Overflow management is left to the user.
## Addition
## Subtraction
## Multiplication
Karatsuba
## Division
Newton-Raphson
Returns floor of a/b
## Exponentiation
Binary algorithm
## Modular exponentation
## GCD
Extended Euclidean Algorithm
# Operations in external modules
## Miller Rabin primality test
## RSA Key Generation
