#include "../logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include <filesystem>


namespace tll {

namespace {
	auto importance_to_string(logger::importance imp) -> const char* {
		switch (imp) {
		case tll::logger::LOW:
			return "Low message importance";
		case tll::logger::MEDIUM:
			return "Medium message importance";
		case tll::logger::CRITICAL:
			return "Critical message importance";
		}
		throw std::invalid_argument("Bad importance");
	}
}

struct logger::impl {
	impl(const std::filesystem::path& path, importance imp = tll::logger::LOW) : file_(path, std::ios::out | std::ios::app), default_importance_(imp) 
	{}
	std::ofstream file_;
	importance default_importance_;
};

logger::logger(const std::filesystem::path& path, importance default_importance) : pimpl_(std::make_unique<impl>(impl{path, default_importance})) {
	if (!pimpl_->file_.is_open()) {
		throw std::ios_base::failure("Failed to open file: " + path.string());
	}
}

logger::~logger() = default;

auto logger::set_default_importance(importance imp) noexcept -> void {
	pimpl_->default_importance_ = imp;
}

auto logger::message_write(std::string_view text, importance imp) const -> void {
	if (imp < pimpl_->default_importance_) {
		return; // Filter out messages with lower importance
	}
	std::time_t tt = std::time(nullptr);
	pimpl_->file_ << text << "\t"
	<< importance_to_string(imp) << "\t" 
	<< std::put_time(std::localtime(&tt), "%H:%M:%S") << std::endl;
}

auto logger::message_write(std::string_view text) const -> void { 
	this->message_write(text, pimpl_ -> default_importance_);
}

auto logger::message_write(message msg) const -> void { 
	this->message_write(msg.text_, msg.importance_);
}

auto logger::operator<<(message msg) const -> const logger& {
	this->message_write(msg.text_, msg.importance_);
	return *this;
}

auto logger::operator<<(std::string_view text) const -> const logger& {
	this->message_write(text);
	return *this;
}



} // namespace tll
