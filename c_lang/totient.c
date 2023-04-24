#include <stdio.h>
#include <stdlib.h>

/* The Euclidean Algorithm */
long gcd(long a, long b)
{
	while (a != b && a > 0) {
		/* printf("gcd(%ld,%ld)\n", a, b); */
		long larger = a > b ? a : b;
		long smaller = a > b ? b : a;
		long rem = larger % smaller;
		a = rem;
		b = smaller;
	}
	return b;
}

/* Euler's totient (or phi) function counts the positive integers up to n that are relatively prime to n */
long totient(long n)
{
	long count = 0;
	/* 1 is relatively prime for n>=1 */
	if (n >= 1)
		count++;
	for (long k = 2; k < n; k++) {
		if (gcd(n, k) == 1)
			count++;
	}
	return count;
}

void assert_eq_long(long actual, long expected)
{
	if (actual == expected)
		/**/;
	else
		printf("not okay; got: %ld; expected: %ld\n", actual, expected);
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
		assert_eq_long(totient(i), expecteds[i]);
	}
}

void totient_sum_million(void)
{
	/* expected[i] = sum(totient(x) for x = 1 .. 10^i) */
	long expecteds[] = { 0, 32, 3044, 304192, 30397486, 3039650754, 303963552392 };
	long sum = 0;
	long limit = 1;
	long n = 1;
	for (long i = 1; i <= 6; i++) {
		limit *= 10;
		for (; n <= limit; n++) {
			sum += totient(n);
		}
		printf("  totient sum: sum=%ld i=%ld limit=%ld n=%ld\n", sum, i, limit, n);
		assert_eq_long(sum, expecteds[i]);
	}
	assert_eq_long(sum, 303963552392);
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
