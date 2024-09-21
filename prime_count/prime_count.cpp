#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

using namespace std;;

//소수 구하기 
// 1과 자기 자신으로만 나뉘면 그것이 소수

bool isPrime(int number) {
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;
	//어떤 소수도 N의 제곱근보다 큰수로 나눠지지않는다.
	//ex) 17의 경우 4까지만 확인해보면된다.
	for (int i = 2; i * i <= number; i++)
	{
		if ((number % i) == 0)
			return false;
	}
	return true;
}


int Prime_Number(int start, int end)
{
	int check_number = 0;
	for (int number = start; number <= end; number++)
	{
		if (isPrime(number))
			check_number++;
	}

	return check_number;
}

int main()
{
	//스레드 별로 각각 일을하고 마지막에 totalcount에만 += 하면되서 멀티스레드의 득을 보는 케이스
	//잘못된 멀티스레드와 단일스레드로 작성 해보았는데. 
	// 각 숫자마다 스레드 생성시 숫자가 커질수록 처리해야할 일이많아지고 스레드 숫자도 그만큼 많아저서 어느순간 너무많은 스레드로인해 진행을 못하는 상황이되어버린다.
	// 스레드가 너무많으니 단일 스레드만도 못 하였다. 
	atomic<int32_t> totalcount = 0;
	//1~MAX_NUMBER까지의 소수 개수
	const int MAX_NUMBER = 1'000'000;
	vector<thread> threads;
	int coreCount = thread::hardware_concurrency(); //현재 pc의 코어 갯수
	int jobCount = (MAX_NUMBER / coreCount) + 1;//한 스레드당 해야할 갯수

	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &totalcount]()
		{
			totalcount += Prime_Number(start, end);
		}));
	}

	

	for (thread& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}


	cout << totalcount << endl;

}
