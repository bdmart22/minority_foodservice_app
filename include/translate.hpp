#pragma once
#include <string>

// Simple translate helper that returns raw JSON response from Google Translate API.
std::string translate_text(const std::string& text, const std::string& target);
