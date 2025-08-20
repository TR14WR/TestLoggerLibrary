#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string_view>
#include <filesystem>
#include <memory>

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

	auto set_default_importance(importance) noexcept -> void;

	auto message_write(std::string_view, importance) const -> void;
	auto message_write(message) const -> void;
	auto operator<<(message) const -> const logger&;

	auto message_write(std::string_view) const -> void;
	auto operator<<(std::string_view) const -> const logger&;

private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};



}//test Logger library
#endif // !_LOGGER_H_