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
		if k > 2 {
			k_delta = 2
		}
	}

	/* n is a prime! */
	if n > 1 {
		multiplier *= n - 1
	}

	return multiplier
}

func assert_eq_int64(actual int64, expected int64) {
	if actual == expected {
	} else {
		fmt.Println("NOT OK: expected ", expected, "got", actual)
	}
}

type totient_func = func(int64) int64

func totient_check_first_10(totient totient_func) {
	expecteds := []int64{0, 1, 1, 2, 2, 4, 2, 6, 4, 6, 4}
	for i, expected := range expecteds {
		actual := totient(int64(i))
		fmt.Printf("totient(%d)=%d (expected=%d)\n", i, actual, expected)
		assert_eq_int64(actual, expected)
	}
}

func totient_sum_million(totient totient_func) {
	var sum int64 = 0
	for i := int64(1); i <= 1000000; i++ {
		sum += totient(i)
	}
	expected := int64(303963552392)
	assert_eq_int64(sum, expected)
	fmt.Println("sum of first million totients =", sum)
}

func totient_sum_goroutine(totient totient_func, limit int64) {
	responses := make(chan int64, 1)

	go func() {
		for i := int64(1); i <= limit; i++ {
			responses <- totient(i)
		}
		close(responses)
	}()

	sum := int64(0)
	for value := range responses {
		sum += value
	}
	fmt.Println("Sum of first million totients =", sum)
}

func main() {
	fmt.Println("Hello, world.")
	// fmt.Println("totient(7) =", totient_factors(7))
	totient_check_first_10(totient_factors)
	totient_sum_million(totient_factors)
	totient_sum_goroutine(totient_factors, int64(1000000))
}
