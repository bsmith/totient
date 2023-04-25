long isqrt(long n)
{
	double x_d = sqrt(n);
	long x = x_d;
	if (!(x*x <= n && (x+1)*(x+1) > n))
		abort();
	return x;
}