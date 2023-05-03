package main

import (
	"fmt"
	"time"
)

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

func totient_sum_serial(totient totient_func, limit int64) {
	var sum int64 = 0
	start := time.Now()
	for i := int64(1); i <= limit; i++ {
		sum += totient(i)
	}
	if limit == 1000000 {
		expected := int64(303963552392)
		assert_eq_int64(sum, expected)
	}
	fmt.Println("sum of first million totients =", sum)
	fmt.Println("  time elapsed:", time.Since(start))
}

func totient_sum_goroutine(totient totient_func, limit int64) {
	numWorkers := 4
	start := time.Now()
	responses := make(chan []int64, numWorkers)
	finishes := make(chan bool, numWorkers)

	worker := func(start int64, end int64) {
		values := make([]int64, 0, end-start+1)
		for i := int64(1); i <= limit; i++ {
			values = append(values, totient(i))
		}
		responses <- values
		finishes <- true
	}

	workersStarted := 0
	for start := int64(1); start <= limit; start += limit / int64(numWorkers) {
		end := start + limit/int64(numWorkers) - 1
		if end > limit {
			end = limit
		}
		go worker(start, end)
		workersStarted++
	}

	go func() {
		defer close(responses)
		count := workersStarted
		for count > 0 {
			<-finishes
			count--
		}
	}()

	sum := int64(0)
	for values := range responses {
		for _, value := range values {
			sum += value
		}
	}
	fmt.Println("Sum of first million totients =", sum)
	fmt.Println("  time elapsed:", time.Since(start))
}

func main() {
	fmt.Println("Hello, world.")
	// fmt.Println("totient(7) =", totient_factors(7))
	totient_check_first_10(totient_factors)
	fmt.Println("Serial")
	totient_sum_serial(totient_factors, int64(1000000))
	totient_sum_serial(totient_factors, int64(10000000))
	fmt.Println("Parallel")
	totient_sum_goroutine(totient_factors, int64(1000000))
	totient_sum_goroutine(totient_factors, int64(10000000))
}
