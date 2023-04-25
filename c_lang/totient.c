#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* The Euclidean Algorithm */
long gcd(long a, long b)
{
	long larger = a > b ? a : b;
	long smaller = a > b ? b : a;
	while (larger != smaller && smaller > 0) {
		/* printf("gcd(%ld,%ld)\n", larger, smaller); */
		long rem = larger % smaller;
		larger = smaller;
		smaller = rem;
	}
	return larger;
}

/* Euler's totient (or phi) function counts the positive integers up to n that are relatively prime to n */
long totient_gcd(long n)
{
	if (n <= 0)
		return 0;

	long count_twos = 0;
	while (n % 2 == 0) {
		n /= 2;
		count_twos++;
	}
	long multiplier = count_twos >= 1 ? 1L << (count_twos - 1) : 1;
	
	/* start at 1 since 1 is relatively prime to all positive integers */
	/* start at 2 since 2 is relatively prime to n after the above */
	long count = 1;
	for (long k = 2; k < n; k++) {
		if (gcd(n, k) == 1)
			count++;
	}

	long totient = multiplier * count;
	/* printf("totient(n=%ld): multiplier=%ld count=%ld totient=%ld\n", n, multiplier, count, totient); */
	return totient;
}

long isqrt(long n)
{
	double x_d = sqrt(n);
	long x = x_d;
	if (!(x*x <= n && (x+1)*(x+1) > n))
		abort();
	return x;
}

/* This version attempts to find prime factors */
long totient(long n)
{
	// long orig_n = n;
	if (n <= 0)
		return 0;
	// if (n == 1)
	// 	return 1;

	long multiplier = 1;
	for (long k = 2; k*k <= n; k += (k & 1 ? 2 : 1)) {
		if (n % k == 0) {
			long k_pow = 1;
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

void assert_eq_long(long actual, long expected)
{
	if (actual == expected)
		/**/;
	else
		printf("not okay; got: %ld; expected: %ld\n", actual, expected), abort();
}

void gcd_random_checks(void)
{
	for (int i = 0; i < 1000; i++) {
		/* arc4random returns a 32-bit random number, 0..UINT32_MAX */
		long a = arc4random();
		long b = arc4random();
		long the_gcd = gcd(a, b);
		assert_eq_long(a % the_gcd, 0);
		assert_eq_long(b % the_gcd, 0);
		assert_eq_long(gcd(a / the_gcd, b / the_gcd), 1);
	}
}

void totient_check_first_10(void)
{
	long expecteds[] = { 0, 1, 1, 2, 2, 4, 2, 6, 4, 6, 4 };
	for (int i = 0; i < sizeof(expecteds)/sizeof(*expecteds); i++) {
		long actual = totient(i);
		assert_eq_long(actual, expecteds[i]);
	}
}

void totient_sum_million(void)
{
	/* expected[i] = sum(totient(x) for x = 1 .. 10^i) */
	long expecteds[] = { 0, 32, 3044, 304192, 30397486, 3039650754, 303963552392, 30396356427242, 303963551173008414 };
	long sum = 0;
	long limit = 1;
	long n = 1;
	clock_t start_clock = clock();
	for (long i = 1; i <= 7; i++) {
		limit *= 10;
		for (; n <= limit; n++) {
			sum += totient(n);
		}
		printf("  totient sum: sum=%ld i=%ld limit=%ld n=%ld\n", sum, i, limit, n);
		printf("  time elapsed: %0.6f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);
		assert_eq_long(sum, expecteds[i]);
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
		printf("  totient sum: sum=%ld remaining_totients=%.2f limit=%ld n=%ld\n", sum, remaining_totients, limit, n);
		printf("  time elapsed: %0.6f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);
	}
}

int main(int argc, char **argv)
{
	printf("gcd: simple checks\n");
	assert_eq_long(gcd(12,24), 12);
	assert_eq_long(gcd(5*7,7*7), 7);
	assert_eq_long(gcd(13,17),1);

	printf("gcd: random checks\n");
	gcd_random_checks();

	printf("totient: checking first 10\n");
	totient_check_first_10();

	printf("totient: summing first million\n");
	totient_sum_million();

	printf("completed\n");

	return 0;
}
