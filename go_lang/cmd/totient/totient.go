package main

import "fmt"

func totient_factors(n int64) int64 {
	if n <= 0 {
		return 0
	}

	var multiplier int64 = 1
	var k int64
	var k_delta int64 = 1
	for k = 2; k*k <= n; k += k_delta {
		if n%k == 0 {
			var k_pow int64 = 1
			n /= k
			for n%k == 0 {
				n /= k
				k_pow *= k
			}
			multiplier *= (k - 1) * k_pow
		}
		k_delta = 2
	}

	/* n is a prime! */
	if n > 1 {
		multiplier *= n - 1
	}

	return multiplier
}

func main() {
	fmt.Println("Hello, world.")
	fmt.Println("totient(7) =", totient_factors(7))
}
