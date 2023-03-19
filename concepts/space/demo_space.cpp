#include <iostream>
#include <filesystem>
#include <cstdint>
#include <cstdlib>
#include <iomanip>

// macOS Ventura results
// │ Capacity        │ Free            │ Available       │ Dir
// │ 202,752         │ -1              │ -1              │ /dev/null
// │ 994,662,584,320 │ 801,581,285,376 │ 801,581,285,376 │ /tmp
// │ -1              │ -1              │ -1              │ /home
// │ -1              │ -1              │ -1              │ /null

void print_space_info(auto const& dirs, int width = 15)
{
    (std::cout << std::left).imbue(std::locale("en_US.UTF-8"));
    for (const auto s : {"Capacity", "Free", "Available", "Dir"})
        std::cout << "│ " << std::setw(width) << s << ' ';
    std::cout << '\n';
    std::error_code ec;
    for (auto const& dir : dirs) {
        const std::filesystem::space_info si = std::filesystem::space(dir, ec);
        if(ec){
            std::cerr << "ERROR:space_avail " << dir << " " << ec.message() << "\n";
            continue;
        }

        std::cout
            << "│ " << std::setw(width) << static_cast<std::intmax_t>(si.capacity) << ' '
            << "│ " << std::setw(width) << static_cast<std::intmax_t>(si.free) << ' '
            << "│ " << std::setw(width) << static_cast<std::intmax_t>(si.available) << ' '
            << "│ " << dir << '\n';
    }
}

int main()
{
    const auto dirs = { "/dev/null", "/tmp", "/home", "/null" };
    print_space_info(dirs);
    return EXIT_SUCCESS;
}
