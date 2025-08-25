#include <mutex>
#include <stdexcept>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <logger.h>

auto parse_message_importance(std::string_view sv) -> tll::logger::importance {
  if (sv == "low")
    return tll::logger::LOW;
  if (sv == "medium")
    return tll::logger::MEDIUM;
  if (sv == "critical")
    return tll::logger::CRITICAL;
  throw std::invalid_argument("Bad importance");
}

int main(int argc, char* argv[]) {

    auto err_log = tll::logger("errorLog.log", tll::logger::CRITICAL);
  
  try {
    if (argc < 3) {
      std::cerr << "Usage: " << argv[0] << " <log_file_path> <default_importance>" << std::endl;
      std::cerr << "Importance levels: low, medium, critical" << std::endl;
      return 1;
    }

    auto log = tll::logger(argv[1], parse_message_importance(argv[2]));
    std::string message_;
    std::string importanceText_;
    std::mutex mutex_;
    auto write_message = [&log, &mutex_, &err_log](std::string message_, std::string importanceText_) {
      try {
        std::lock_guard<std::mutex> lg(mutex_);
        
        if (importanceText_.empty()) {
          log.message_write(message_);
        } else {
          auto importance_ = parse_message_importance(importanceText_);
          log.message_write(message_, importance_);
        }
      }
      catch (const std::exception& ex) {
        std::lock_guard<std::mutex> lg(mutex_);
        err_log << std::string("Error writing message: ") + ex.what();
      }
    };
    
    std::vector<std::thread> threadVector;

    std::cout << "Please enter a blank message to end your entry." << std::endl;
    while (true) {
      std::cout << "Enter a message: ";
      std::getline(std::cin, message_);

      if (message_.empty()) {
        std::cout << "Empty message entered, input stopped" << std::endl;
        break;
      }

      std::cout << "Enter a message importance(low, medium, critical): ";
      std::getline(std::cin, importanceText_);

      try {
        std::thread tr_(write_message, std::move(message_), std::move(importanceText_));
        threadVector.push_back(std::move(tr_));
      }
      catch (const std::exception& ex) {
        err_log << std::string("Error creating thread: ") + ex.what();
      }
    }

    for (auto& thread : threadVector) {
      if (thread.joinable()) {
        try { 
          thread.join(); 
        }
        catch (const std::exception& ex) { 
          err_log << std::string("Error joining thread: ") + ex.what();
        }
      }
    }
    return 0;
  }
  catch (const std::exception& ex) {
    err_log << std::string("Fatal error: ") + ex.what();
    return 1;
  }
}