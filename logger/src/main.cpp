#include <mutex>
#include <stdexcept>
#include <thread>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include "../../api/logger.h"

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
  try {
    if (argc < 3) {
      std::cerr << "Usage: " << argv[0] << " <log_file_path> <default_importance>" << std::endl;
      std::cerr << "Importance levels: low, medium, critical" << std::endl;
      return 1;
    }

    auto log = std::make_optional<tll::logger>(argv[1], parse_message_importance(argv[2]));
    
    std::string message;
    std::string importanceText;
    std::mutex mutex;
    auto write_message = [&log, &mutex](std::string message, std::string importanceText) {
      try {
        std::lock_guard<std::mutex> lg(mutex);
        
        if (importanceText.empty()) {
          log.value().message_write(message);
        } else {
          auto importance = parse_message_importance(importanceText);
          log.value().message_write(message, importance);
        }
      }
      catch (const std::exception& ex) {
        std::lock_guard<std::mutex> lg(mutex);
        std::cerr << "Error writing message: " << ex.what() << std::endl;
      }
    };

    std::vector<std::thread> threadVector;

    std::cout << "Please enter a blank message to end your entry." << std::endl;
    while (true) {
      std::cout << "Enter a message: ";
      std::getline(std::cin, message);

      if (message.empty()) {
        std::cout << "Empty message entered, input stopped" << std::endl;
        break;
      }

      std::cout << "Enter a message importance(low, medium, critical): ";
      std::getline(std::cin, importanceText);

      try {
        std::thread tr(write_message, std::move(message), std::move(importanceText));
        threadVector.push_back(std::move(tr));
      }
      catch (const std::exception& ex) {
        std::cerr << "Error creating thread: " << ex.what() << std::endl;
      }
    }

    for (auto& thread : threadVector) {
      if (thread.joinable()) {
        try { 
          thread.join(); 
        }
        catch (const std::exception& ex) { 
          std::cerr << "Error joining thread: " << ex.what() << std::endl; 
        }
      }
    }
    return 0;
  }
  catch (const std::exception& ex) {
    std::cerr << "Fatal error: " << ex.what() << std::endl;
    return 1;
  }
}