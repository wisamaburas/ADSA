#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned int digit;
typedef std::vector<digit> integer;
unsigned int B = 10;

void setBase(unsigned int base) {
  B = base;  // This assigns the value to the global variable B
}
int getBase() { return B; }

// adds I1, I2, and c which is carryIn. Outputs carry for the next
// digit (carryOut) and s (sum)
void fullAdder(digit I1, digit I2, digit c, digit& s, digit& carry) {
  unsigned int sum = I1 + I2 + c;
  carry = sum / B;
  s = sum - carry * B;
}

// multiplies I1 and I2, B is base, s is sum ouutput, carry is carry out
void digitMult(digit I1, digit I2, digit& s, digit& carry) {
  unsigned int prod = I1 * I2;
  carry = prod / B;
  s = prod - carry * B;
}

digit getDigit(const integer& a, int i) { return (i < a.size() ? a[i] : 0); }

// The school method (function mult) multiplies a with each
//  digit of b and then adds it at the appropriate position to the result
//  (function addAt).
void mult(const integer& a, const digit& b, integer& atimesb) {
  int n = a.size();
  assert(atimesb.size() == n + 1);
  digit carry = 0, c, d, cprev = 0;
  for (int i = 0; i < n; i++) {
    digitMult(a[i], b, d, c);
    fullAdder(d, cprev, carry, atimesb[i], carry);
    cprev = c;
  }
  d = 0;
  fullAdder(d, cprev, carry, atimesb[n], carry);
  assert(carry == 0);
}

void addAt(integer& p, const integer& atimesbj, int j) {  // p has length n+m,
  digit carry = 0;
  int L = p.size();
  for (int i = j; i < L; i++)
    fullAdder(p[i], getDigit(atimesbj, i - j), carry, p[i], carry);
  assert(carry == 0);
}

integer mult(const integer& a, const integer& b) {
  int n = a.size();
  int m = b.size();
  integer p(n + m, 0);
  integer atimesbj(n + 1);
  for (int j = 0; j < m; j++) {
    mult(a, b[j], atimesbj);
    addAt(p, atimesbj, j);
  }
  return p;
}

integer add(const integer& a, const integer& b) {
  int n = max(a.size(), b.size());
  integer s(n + 1);
  digit carry = 0;
  for (int i = 0; i < n; i++) {
    fullAdder(getDigit(a, i), getDigit(b, i), carry, s[i], carry);
  }
  s[n] = carry;
  return s;
}

void sub(integer& a, const integer& b)  // requires a >= b
{
  digit carry = 0;
  for (int i = 0; i < a.size(); i++)
    if (a[i] >= (getDigit(b, i) + carry)) {
      a[i] = a[i] - getDigit(b, i) - carry;
      carry = 0;
    } else {
      a[i] = a[i] + B - getDigit(b, i) - carry;
      carry = 1;
    }
  assert(carry == 0);
}

void split(const integer& a, integer& a1, integer& a0) {
  int n = a.size();
  int k = n / 2;
  for (int i = 0; i < k; i++) a0[i] = a[i];
  for (int i = 0; i < n - k; i++) a1[i] = a[k + i];
}

integer Karatsuba(const integer& a, const integer& b, int n0) {
  int n = a.size();
  int m = b.size();
  integer p(2 * n);
  if (n < n0) return mult(a, b);
  int k = n / 2;
  integer a0(k), a1(n - k), b0(k), b1(n - k);
  split(a, a1, a0);
  split(b, b1, b0);
  integer p2 = Karatsuba(a1, b1, n0),
          p1 = Karatsuba(add(a1, a0), add(b1, b0), n0),
          p0 = Karatsuba(a0, b0, n0);
  for (int i = 0; i < 2 * k; i++) p[i] = p0[i];
  for (int i = 2 * k; i < n + m; i++) p[i] = p2[i - 2 * k];
  sub(p1, p0);
  sub(p1, p2);
  addAt(p, p1, k);
  return p;
}

int main() {
  string inputI1_str, inputI2_str;
  digit inputbase;

  cin >> inputI1_str >> inputI2_str >> inputbase;

  setBase(inputbase);

  string reversestringI1(inputI1_str);
  reverse(reversestringI1.begin(), reversestringI1.end());

  string reversestringI2(inputI2_str);
  reverse(reversestringI2.begin(), reversestringI2.end());

  // Convert inputI1_str and inputI2_str to integer objects
  integer inputI1(inputI1_str.size());
  for (int i = inputI1_str.size() - 1; i >= 0; i--) {
    inputI1[i] = reversestringI1[i] - '0';
  }

  integer inputI2(inputI2_str.size(), 0);
  for (int i = inputI2_str.size() - 1; i >= 0; i--) {
    inputI2[i] = reversestringI2[i] - '0';
  }
  integer result_add = add(inputI1, inputI2);

  // Use Karatsuba method if the size of inputs is greater than 1
  integer result_mult = Karatsuba(inputI1, inputI2, 4);

  for (int i = result_add.size() - 1; i >= 0; i--) {
    cout << result_add[i];
  }
  cout << " ";
  for (int i = 0; i <= result_mult.size() - 1; i++) {
    cout << result_mult[i];
  }
  cout << " ";
  // For division, output 0
  cout << "0" << endl;

  return 0;
}
