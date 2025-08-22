#include "../../logger.h"

void test1() {
	tll::logger log("logfile.txt", tll::logger::LOW);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

void test2() {
	tll::logger log("logfile.txt", tll::logger::MEDIUM);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

void test3() {
	tll::logger log("logfile.txt", tll::logger::CRITICAL);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

void test4() {
	tll::logger log("logfile.txt", tll::logger::LOW);
	log.set_default_importance(tll::logger::MEDIUM);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

void test5() {
	tll::logger log("logfile.txt", tll::logger::LOW);
	log.set_default_importance(tll::logger::CRITICAL);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

void test6() {
	tll::logger log("logfile.txt", tll::logger::CRITICAL);
	log.set_default_importance(tll::logger::LOW);
	log.message_write("the first message", tll::logger::LOW);
	log.message_write("the second message", tll::logger::MEDIUM);
	log.message_write("the third message", tll::logger::CRITICAL);
}

int main() {
	test1(); //123
	test2(); //23 
	test3(); //3
	test4(); //23
	test5(); //3
	test6(); //123
}
