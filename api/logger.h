#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <string_view>
#include <filesystem>

namespace tll { //test Logger library

class logger final {
public:

	enum importance : char {
		LOW = 0,	
		MEDIUM,
		CRITICAL
	};

	struct message {
		importance importance_;
		std::string text_;
	};

	explicit logger(const std::filesystem::path&, importance);
	~logger() = default;

	logger(const logger&) = delete;
	auto operator=(const logger&) -> logger& = delete;

	logger(logger&&) = default;
	auto operator=(logger&&) -> logger& = default;

	auto set_default_importance(importance) -> void;
	auto message_write(std::string_view, importance) const -> void;
	auto message_write(message) const -> void;
	auto message_write(std::string_view) const -> void;

	auto operator<<(std::string_view) const -> const logger&;
	auto operator<<(message) const -> const logger&;

private:
	auto importance_to_string(importance) const -> std::string;

	struct Impl;
	std::unique_ptr<Impl> pimpl_;
};



}//test Logger library
#endif // !_LOGGER_H_