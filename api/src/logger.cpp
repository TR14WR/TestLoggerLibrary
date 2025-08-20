#include "../logger.h"
#include <string>
#include <fstream>

namespace tll {

struct logger::Impl {
	Impl(const std::filesystem::path& path, importance imp) : file_(path, std::ios::out | std::ios::app), default_importance_(imp) {}
	std::fstream file_;
	importance default_importance_;
};

logger::logger(const std::filesystem::path& path, importance default_importance) : pimpl_(std::make_unique<Impl>(Impl{path, default_importance})) {}
  
}//11

auto logger::set_default_importance(importance imp) -> void {
	this->default_importance_ = imp;
}

auto logger::message_write(std::string_view text, importance imp) const -> void {
	if (imp < pimpl_->default_importance_) {
		throw std::invalid_argument("Message importance is less than default importance");
	}
	pimpl_->file_ << text << "\t"
				  << importance_to_string(imp) << "\t" 
				  << std::put_time(std::localtime(&std::time(nullptr)), "%H:%M:%S") << std::endl;
}

auto logger::message_write(std::string_view text) const -> void {
	pimpl_->file_ << text << "\t"
				  << importance_to_string(pimpl_->default_importance_) << "\t" 
				  << std::put_time(std::localtime(&std::time(nullptr)), "%H:%M:%S") << std::endl;
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

auto logger::importance_to_string(importance imp) const -> std::string {
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


} // namespace tll