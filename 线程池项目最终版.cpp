#include"threadPool.h"
#include<chrono>
/*
* 1、如何能让线程池提交任务更加方便
* pool.submitTask(sum1,10,20);
* pool.submitTask(sum2,10,20,30);
* submitTask:可变参模板编程
* 
* 2、为了接收任务的返回值，自己创建了一个result以及相关的类型，代码复杂
* C++11的线程库提供了 thread、 packaged_task(function函数对象) async(能够直接获取返回值)
* 使用future代替result 减少线程池代码
*/

int sum1(int a ,int b)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	//比较耗时
	return a + b;
}

int sum2(int a, int b,int c)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return a + b + c;
}
int main()
{

	ThreadPool pool;
	//pool.setMode(PoolMode::MODE_CACHED);
	pool.start(2);

	std::future<int> r1 = pool.submitTask(sum1, 1, 2);
	std::future<int> r2 = pool.submitTask(sum2, 3, 4,5);
	std::future<int> r3 = pool.submitTask([](int b, int e)->int {
		int sum = 0; 
		for (int i = b; i <= e; i++)
		{
			sum += i;
		}
		return sum;
		}, 1, 100);
	std::future<int> r4 = pool.submitTask([](int b, int e)->int {
		int sum = 0;
		for (int i = b; i <= e; i++)
		{
			sum += i;
		}
		return sum;
}, 1, 100);
	std::future<int> r5 = pool.submitTask([](int b, int e)->int {
		int sum = 0;
		for (int i = b; i <= e; i++)
		{
			sum += i;
		}
		return sum;
		}, 1, 100);
	std::cout << std::endl;
	std::cout << r1.get() << std::endl;
	std::cout << r2.get() << std::endl;
	std::cout << r3.get() << std::endl;
	std::cout << r4.get() << std::endl;
	std::cout << r5.get() << std::endl;
#if 0
	std::packaged_task<int(int, int)>task(sum1);//packaged_task可以通过get_future()函数得到返回值
	//future 可以看作我们自己写的result
	std::future<int> res = task.get_future();
	//task(10, 20);

	std::thread t1(std::move(task), 10, 20);
	t1.join();//等待该线程执行完成，main函数会在t1任务结束后再继续执行
	t1.detach();//将该线程分离，主程序不等待它，其执行不依赖于main函数的生命周期
	//若不进行上述两个操作，可能会导致t1还未执行完毕，程序就退出。
	
	std::cout << res.get() << std::endl;

#endif
}
