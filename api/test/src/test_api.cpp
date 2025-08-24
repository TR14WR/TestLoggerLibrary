#include <logger.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

class test_logger final {
private:
  std::string msg_string_;

  auto write_msg_foo(tll::logger::importance default_imp_) -> void {
    tll::logger log("logFile.txt", default_imp_);

    log.message_write("the first message", tll::logger::LOW);
    log.message_write("the second message", tll::logger::MEDIUM);
    log.message_write("the third message", tll::logger::CRITICAL);
    
    std::ifstream file("logFile.txt");
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        msg_string_ += line + "\n";
      }
      file.close();
    }

    std::filesystem::remove("logFile.txt");
  }

  auto set_default_foo(tll::logger::importance set_default_imp_) -> void {
    tll::logger log("logFile.txt", tll::logger::LOW); 

    log.set_default_importance(set_default_imp_);

    log.message_write("test message"); 

    std::ifstream file("logFile.txt");
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        msg_string_ += line + "\n";
      }
      file.close();
    }

    std::filesystem::remove("logFile.txt");
  }

public:
  auto writeMessage_lowDefault_123() -> bool {
    msg_string_ = "";
    write_msg_foo(tll::logger::LOW);

    if (!msg_string_.empty() &&
      msg_string_.find("the first message") != std::string::npos &&
      msg_string_.find("the second message") != std::string::npos &&
      msg_string_.find("the third message") != std::string::npos) {
      return true;
    }
    return false;
  }

  auto writeMessage_mediumDefault_23() -> bool {
    msg_string_ = "";
    write_msg_foo(tll::logger::MEDIUM);

    if (!msg_string_.empty() &&
      msg_string_.find("the second message") != std::string::npos &&
      msg_string_.find("the third message") != std::string::npos &&
      msg_string_.find("the first message") == std::string::npos) {
      return true;
    }
    return false;
  }

  auto writeMessage_criticalDefault_3() -> bool {
    msg_string_ = "";
    write_msg_foo(tll::logger::CRITICAL);

    if (!msg_string_.empty() &&
      msg_string_.find("the third message") != std::string::npos &&
      msg_string_.find("the first message") == std::string::npos &&
      msg_string_.find("the second message") == std::string::npos) {
      return true;
    }
    return false;
  }

  auto setDefault_lowDefault_low() -> bool {
    msg_string_ = "";
    set_default_foo(tll::logger::LOW);

    if (!msg_string_.empty() && msg_string_.find("test message") != std::string::npos) {
      return true;
    }
    return false;
  }

  auto setDefault_mediumDefault_medium() -> bool {
    msg_string_ = "";
    set_default_foo(tll::logger::MEDIUM);

    if (!msg_string_.empty() && msg_string_.find("test message") != std::string::npos) {
      return true;
    }
    return false;
  }

  auto setDefault_criticalDefault_critical() -> bool {
    msg_string_ = "";
    set_default_foo(tll::logger::CRITICAL);

    if (!msg_string_.empty() && msg_string_.find("test message") != std::string::npos) {
      return true;
    }
    return false;
  }

  auto writeMessage_leftShiftOperator_all() -> bool {
    msg_string_ = "";

    tll::logger log("logFile.txt", tll::logger::LOW);
    log << "Left shift operator";

    std::ifstream file("logFile.txt");
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        msg_string_ += line + "\n";
      }
      file.close();
    }

    std::filesystem::remove("logFile.txt");

    if (!msg_string_.empty() && msg_string_.find("Left shift operator") != std::string::npos) {
      return true;
    }
    return false;
  }
};

int main() {
  test_logger testlog;

  std::cout << "writeMessage_lowDefault_123\t\t" << testlog.writeMessage_lowDefault_123() << std::endl;
  std::cout << "writeMessage_mediumDefault_23\t\t" << testlog.writeMessage_mediumDefault_23() << std::endl;
  std::cout << "writeMessage_criticalDefault_3\t\t" << testlog.writeMessage_criticalDefault_3() << std::endl;
  std::cout << "writeMessage_leftShiftOperator_all\t" << testlog.writeMessage_leftShiftOperator_all() << std::endl;
  std::cout << "setDefault_lowDefault_low\t\t" << testlog.setDefault_lowDefault_low() << std::endl;
  std::cout << "setDefault_mediumDefault_medium\t\t" << testlog.setDefault_mediumDefault_medium() << std::endl;
  std::cout << "setDefault_criticalDefault_critical\t" << testlog.setDefault_criticalDefault_critical() << std::endl;

  return 0;
}