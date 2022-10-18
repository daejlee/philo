
int	main(int argc, char **argv)
{
	pthread	thread_id[철학자 수]; //철학자
	mutex	mutex_id[철학자 수]; //포크

	while (철학자의 수--)
		thread_id[철학자의 수--] = 쓰레드 생성; //스레드 생성하며 id 저장
	while (철학자의 수--)
		mutex_id[철학자의 수--] = 뮤텍스 생성; //뮤텍스 생성하며 id 저장
	if (스레드 or 뮤텍스 실패 시)
		return (err);
	// 철학자는 자신의 오른쪽에 있는 포크를 집는다.
	while (철학자의 수--)
	{
		LOCK mutex_id[철학자의 수 + 1]; //자신의 번호 + 1에 해당하는 포크를 집음.
		print_log(); //timestamp_in_ms X has taken a fork
	}
	while (no_dead_philosopers()
			|| number_of_times_each_philosopher_must_eat == 식사 횟수)
	{
		// 임계 구역
		LOCK mutex_id[NOT LOCKED]; //1번 철학자 식사 시작
		print_log(); //timestamp_in_ms X is eating
		wait(eating time);
		UNLOCK mutex_id[철학자 번호, 철학자 번호 + 1];
		// 임계 구역 끝
		print_log(); //timestamp_in_ms X is sleepling
	}
	purge_everything();
	return (0);
}