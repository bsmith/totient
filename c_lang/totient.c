#include <stdio.h>

static inline
void swap(long * restrict a_ptr, long * restrict b_ptr)
{
	long a, b;
	a = *a_ptr;
	b = *b_ptr;
	*a_ptr = b;
	*b_ptr = a;
}

long gcd(long a, long b)
{
	while (a != b && a > 0) {
		/* printf("gcd(%ld,%ld)\n", a, b); */
		if (b > a)
			swap(&a, &b);
		long rem = a % b;
		a = rem;
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

int main(int argc, char **argv)
{
	assert_eq_long(gcd(12,24), 12);
	assert_eq_long(gcd(5*7,7*7), 7);

	return 0;
}
