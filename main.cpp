#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    using namespace std::filesystem;

    if (argc < 3 or std::string(argv[1]) == "--help")
    {
        std::clog << "USAGE: " << argv[0] << " <DIR-TO-BE-REMOVE> <SOURCE-DIR>" << std::endl;
        return EXIT_FAILURE;
    }

    const path rm_dir{argv[1]};
    const path source_dir{argv[2]};

    using vec_path = std::vector<path>;

    vec_path source_file;
    vec_path dir_files;

    std::thread t1{[&]() {
        for (const auto& i : recursive_directory_iterator(source_dir))
        {
            source_file.push_back(i);
        }
    }};
    std::thread t2{[&]() {
        for (const auto& i : recursive_directory_iterator(rm_dir))
        {
            dir_files.push_back(i);
        }
    }};

    t1.join();
    t2.join();

    for (size_t i{}; i < source_file.size(); ++i)
    {
        if (is_directory(source_file.at(i)))
        {
            continue;
        }

        for (size_t j{}; j < dir_files.size(); ++j)
        {
            if (is_directory(dir_files.at(j)))
            {
                continue;
            }

            if (source_file.at(i).filename() == dir_files.at(j).filename())
            {
                remove(dir_files.at(j));
                std::clog << "remove: " << dir_files.at(j) << std::endl;
            }
        }
    }
}
