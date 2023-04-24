#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv)
{
	printf("gcd: simple checks\n");
	assert_eq_long(gcd(12,24), 12);
	assert_eq_long(gcd(5*7,7*7), 7);
	assert_eq_long(gcd(13,17),1);

	printf("gcd: random checks\n");
	gcd_random_checks();

	return 0;
}
