# Large Integer Arithmetic — C++

Implementation of arbitrary-precision integer arithmetic algorithms in C++,
built as part of an Algorithms and Data Structures assignment.

## Algorithms implemented

- **Grade school multiplication** — digit by digit multiplication with carry propagation
- **Karatsuba multiplication** — divide and conquer algorithm for efficient large integer 
  multiplication, reducing the complexity from O(n²) to approximately O(n^1.58)
- **Addition and subtraction** — supporting operations with carry/borrow handling

## How to run

```bash
g++ main.cpp -o main
echo "1234 5678 10" | ./main
```

Input is two integers and a base, space separated. Output is their sum and product.

## Language
C++
