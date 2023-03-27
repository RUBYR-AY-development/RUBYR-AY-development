// custom c++ interface

#include <iostream>

class interface
{
	public:
		virtual void a_func();
};

class base_class : interface
{
	public:
		void a_func() override
		{
			std::cout << "virtual" << '\n';
		}
};

int main(void)
{
	base_class base;
	
	base.a_func();

	return 0;
}
