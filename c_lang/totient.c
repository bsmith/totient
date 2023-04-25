#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef int64_t (*totient_func)(int64_t);

/* The Euclidean Algorithm */
int64_t gcd(int64_t a, int64_t b)
{
	int64_t larger = a > b ? a : b;
	int64_t smaller = a > b ? b : a;
	while (larger != smaller && smaller > 0) {
		/* printf("gcd(%ld,%ld)\n", larger, smaller); */
		int64_t rem = larger % smaller;
		larger = smaller;
		smaller = rem;
	}
	return larger;
}

/* Euler's totient (or phi) function counts the positive integers up to n that are relatively prime to n */
int64_t totient_gcd(int64_t n)
{
	if (n <= 0)
		return 0;

	int64_t count_twos = 0;
	while (n % 2 == 0) {
		n /= 2;
		count_twos++;
	}
	int64_t multiplier = count_twos >= 1 ? 1L << (count_twos - 1) : 1;
	
	/* start at 1 since 1 is relatively prime to all positive integers */
	/* start at 2 since 2 is relatively prime to n after the above */
	int64_t count = 1;
	for (int64_t k = 2; k < n; k++) {
		if (gcd(n, k) == 1)
			count++;
	}

	int64_t totient = multiplier * count;
	/* printf("totient(n=%ld): multiplier=%ld count=%ld totient=%ld\n", n, multiplier, count, totient); */
	return totient;
}

/* This version attempts to find prime factors */
int64_t totient_factors(int64_t n)
{
	// int64_t orig_n = n;
	if (n <= 0)
		return 0;
	// if (n == 1)
	// 	return 1;

	int64_t multiplier = 1;
	for (int64_t k = 2; k*k <= n; k += (k & 1 ? 2 : 1)) {
		if (n % k == 0) {
			int64_t k_pow = 1;
			n /= k;
			while (n % k == 0) {
				n /= k;
				k_pow *= k;
			}
			multiplier *= (k - 1) * k_pow;
		}
	}

	/* n is a prime! */
	if (n > 1)
		multiplier *= n - 1;

	// printf("totient(%ld)=%ld; n=%ld\n", orig_n, multiplier, n);

	return multiplier;
}

void assert_eq_int64(int64_t actual, int64_t expected)
{
	if (actual == expected)
		/**/;
	else
		printf("not okay; got: %jd; expected: %jd\n", (intmax_t)actual, (intmax_t)expected), abort();
}

void gcd_random_checks(void)
{
	for (int i = 0; i < 1000; i++) {
		/* arc4random returns a 32-bit random number, 0..UINT32_MAX */
		int64_t a = arc4random();
		int64_t b = arc4random();
		int64_t the_gcd = gcd(a, b);
		assert_eq_int64(a % the_gcd, 0);
		assert_eq_int64(b % the_gcd, 0);
		assert_eq_int64(gcd(a / the_gcd, b / the_gcd), 1);
	}
}

void totient_check_first_10(totient_func totient)
{
	int64_t expecteds[] = { 0, 1, 1, 2, 2, 4, 2, 6, 4, 6, 4 };
	for (int i = 0; i < sizeof(expecteds)/sizeof(*expecteds); i++) {
		int64_t actual = totient(i);
		assert_eq_int64(actual, expecteds[i]);
	}
}

void totient_sum_million(totient_func totient)
{
	/* expected[i] = sum(totient(x) for x = 1 .. 10^i) */
	int64_t expecteds[] = { 0, 32, 3044, 304192, 30397486, 3039650754, 303963552392, 30396356427242, 303963551173008414 };
	int64_t sum = 0;
	int64_t limit = 1;
	int64_t n = 1;
	clock_t start_clock = clock();
	for (int64_t i = 1; i <= 7; i++) {
		limit *= 10;
		for (; n <= limit; n++) {
			sum += totient(n);
		}
		printf("  totient sum: sum=%jd i=%jd limit=%jd n=%jd\n", (intmax_t)sum, (intmax_t)i, (intmax_t)limit, (intmax_t)n);
		printf("  time elapsed: %0.6f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);
		assert_eq_int64(sum, expecteds[i]);
	}
	/* now run until 10 seconds have elapsed */
	while ((clock() - start_clock) < 10*CLOCKS_PER_SEC) {
		/* We've done n totients in time t seconds */
		/* So we're doing n/t totients per second */
		/* In the remaining time Rt, we can therefore do Rt*n/t totients */
		/* BUT Rt = 10 - t, so Rt*n/t==(10-t)*n/t==(10/t-1)*n XXX */
		/* Let's do half of that */
		double time_elapsed = (double)(clock() - start_clock)/CLOCKS_PER_SEC;
		double remaining_totients = (10 / time_elapsed - 1) * n;
		limit += remaining_totients < 200 ? 100 : remaining_totients / 2;
		for (; n <= limit; n++)
			sum += totient(n);
		printf("  totient sum: sum=%jd remaining_totients=%.2f limit=%jd n=%jd\n", (intmax_t)sum, remaining_totients, (intmax_t)limit, (intmax_t)n);
		printf("  time elapsed: %0.6f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);
	}
}

int main(int argc, char **argv)
{
	totient_func totient = NULL;

	if (argc != 2) {
		fprintf(stderr, "%s: expected 1 argument\n", argv[0]);
		return 1;
	} else if (argc == 2) {
		if (strcmp("gcd", argv[1]) == 0)
			totient = totient_gcd;
		else if (strcmp("factors", argv[1]) == 0)
			totient = totient_factors;
		else {
			fprintf(stderr, "first argument should be one of: gcd, factors\n");
			return 1;
		}
	}

	printf("gcd: simple checks\n");
	assert_eq_int64(gcd(12,24), 12);
	assert_eq_int64(gcd(5*7,7*7), 7);
	assert_eq_int64(gcd(13,17),1);

	printf("gcd: random checks\n");
	gcd_random_checks();

	printf("totient: checking first 10\n");
	totient_check_first_10(totient);

	printf("totient: summing first million\n");
	totient_sum_million(totient);

	printf("completed\n");

	return 0;
}
