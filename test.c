#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define	PTHREAD_NUM	5

// we are gonna test gettimeofday.

int	main()
{
	struct timeval	time;

	int	i = 0;
	while (i++ < 10)
	{
	gettimeofday(&time, NULL);
	printf("%lu, %lu\n", time.tv_sec , time.tv_usec);
	printf("%lu, %lu\n", (time.tv_sec % 10) * 1000000 , time.tv_usec);
	printf("%lu\n", (time.tv_sec % 10) * 1000 + time.tv_usec) / 1000;
	usleep(200000);
	}
	// gettimeofday(&time, NULL);
	// temp = time.tv_sec / 100000 + time.tv_usec / 1000;
	// printf("%lu\n", temp);
	// usleep(200000);
	// gettimeofday(&time, NULL);
	// temp = time.tv_sec / 100000 + time.tv_usec / 1000;
	// printf("%lu\n", temp);
	// usleep(200000);
	// gettimeofday(&time, NULL);
	// temp = time.tv_sec / 100000 + time.tv_usec / 1000;
	// printf("%lu\n", temp);
	// usleep(200000);
	// gettimeofday(&time, NULL);
	// temp = time.tv_sec / 100000 + time.tv_usec / 1000;
	// printf("%lu\n", temp);
	// usleep(200000);
	// gettimeofday(&time, NULL);
	// temp = time.tv_sec / 100000 + time.tv_usec / 1000;
	// printf("%lu\n", temp);
	return (0);
}