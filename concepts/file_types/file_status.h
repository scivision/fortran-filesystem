#include <filesystem>

namespace fs = std::filesystem;

void print_file_status(fs::path);
void print_file_status(const char*);
void print_file_status(const fs::directory_entry&);
