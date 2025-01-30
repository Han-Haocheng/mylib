//
// Created by HanHaocheng on 2025/1/29.
//
#include "../src/convert/convert.h"
#include "include.h"


TEST(TestConvert, GeneralTest) {
	const std::string str = "123.01";
	try {
		const int a	   = mylib::Convert<int>(str);
		const double b = mylib::Convert<double>(str);
		std::cout << a << "\t" << b << std::endl;
	} catch (const mylib::Exception &e) {
		std::cout << e.what_string() << std::endl;
	}
}