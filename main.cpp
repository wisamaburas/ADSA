#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned int digit;
typedef std::vector<digit> integer;

// takes inputs of two integers and base, returns sum and carry
void fullAdder(digit a, digit b, digit c, digit& s, digit& carry,
               unsigned int base) {
  if (base == 0) {
    cerr << "Error: Base cannot be zero." << endl;
    exit(1);
  }

  unsigned int sum = a + b + c;
  carry = sum / base;
  s = sum % base;
}

void digitMult(digit a, digit b, digit& s, digit& carry, unsigned int base) {
  unsigned int prod = a * b;
  carry = prod / base;
  s = prod - carry * base;
}

digit getDigit(const integer& a, int i) { return (i < a.size() ? a[i] : 0); }

void mult(const integer& a, const digit& b, integer& atimesb,
          unsigned int base) {
  int n = a.size();
  atimesb.resize(n + 1, 0);
  digit carry = 0;
  for (int i = 0; i < n; i++) {
    digit d, c;
    digitMult(a[i], b, d, c, base);
    d += carry;
    carry = d / base;
    atimesb[i] = d % base;
  }
  atimesb[n] = carry;
}

void addAt(integer& p, const integer& atimesbj, int j, unsigned int base) {
  digit carry = 0;
  int L = p.size();
  for (int i = j; i < L; i++) {
    digit sum = p[i] + getDigit(atimesbj, i - j) + carry;
    p[i] = sum % base;
    carry = sum / base;
  }
  while (carry > 0) {
    p.push_back(carry % base);
    carry /= base;
  }
}

integer mult(const integer& a, const integer& b, unsigned int base) {
  int n = a.size();
  int m = b.size();
  integer result(n + m, 0);
  for (int j = 0; j < m; j++) {
    integer temp(n + 1, 0);
    mult(a, b[j], temp, base);
    for (int i = 0; i < n + 1; i++) {
      result[i + j] += temp[i];
      result[i + j + 1] += result[i + j] / base;
      result[i + j] %= base;
    }
  }
  return result;
}

integer add(const integer& a, const integer& b, unsigned int base) {
  int n = a.size();
  int m = b.size();
  integer result(max(n, m) + 1, 0);
  digit carry = 0;
  for (int i = 0; i < n || i < m; i++) {
    digit sum = getDigit(a, i) + getDigit(b, i) + carry;
    result[i] = sum % base;
    carry = sum / base;
  }
  result.back() = carry;
  return result;
}

void sub(integer& a, const integer& b, unsigned int base) {
  digit carry = 0;
  for (int i = 0; i < a.size(); i++)
    if (a[i] >= (getDigit(b, i) + carry)) {
      a[i] = a[i] - getDigit(b, i) - carry;
      carry = 0;
    } else {
      a[i] = a[i] + base - getDigit(b, i) - carry;
      carry = 1;
    }
  assert(carry == 0);
}

void split(const integer& a, integer& a1, integer& a0, int k) {
  int n = a.size();
  a0.clear();
  a1.clear();
  for (int i = 0; i < k; i++) {
    if (i < n - k) {
      a0.push_back(a[i]);
    } else {
      a1.push_back(a[i]);
    }
  }
}

integer Karatsuba(const integer& a, const integer& b, unsigned int base) {
  int n = a.size();
  int m = b.size();

  if (n == 0 || m == 0) {
    return integer();
  }
  if (n == 1 && m == 1) {
    digit s, carry;
    digitMult(a[0], b[0], s, carry, base);
    integer result;
    result.push_back(s);
    if (carry > 0) {
      result.push_back(carry);
    }
    return result;
  }

  int k = (std::max(n, m) + 1) / 2;

  integer a0, a1, b0, b1;
  split(a, a1, a0, k);
  split(b, b1, b0, k);

  integer p2 = Karatsuba(a1, b1, base);
  integer p0 = Karatsuba(a0, b0, base);

  integer temp1 = add(a1, a0, base);
  integer temp2 = add(b1, b0, base);
  integer p1 = Karatsuba(temp1, temp2, base);
  sub(p1, p0, base);
  sub(p1, p2, base);

  integer result(2 * k + std::max(p2.size(), p1.size()), 0);
  addAt(result, p0, 0, base);
  addAt(result, p2, 2 * k, base);
  addAt(result, p1, k, base);

  return result;
}

int main() {
  int num1, num2, base;
  std::cout << "Enter two integers and a base separated by spaces: ";
  std::cin >> num1 >> num2 >> base;

  integer a, b;
  while (num1 > 0) {
    a.push_back(num1 % base);
    num1 /= base;
  }
  while (num2 > 0) {
    b.push_back(num2 % base);
    num2 /= base;
  }

  // school addition
  integer sum = add(a, b, base);

  // Karatsuba multiplication
  integer product = Karatsuba(a, b, base);

  std::cout << "School Addition: ";
  for (int i = sum.size() - 1; i >= 0; i--) {
    std::cout << sum[i];
  }
  std::cout << std::endl;

  std::cout << "Karatsuba Multiplication: ";
  for (int i = product.size() - 1; i >= 0; i--) {
    std::cout << product[i];
  }
  std::cout << std::endl;

  return 0;
}