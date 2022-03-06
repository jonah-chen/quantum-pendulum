#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr const char *const BASE =
    "manim render --renderer=opengl -r 7680,4320 --write_to_movie ";

static constexpr const char *const DIR = "animation";

static constexpr const char *const EXT = ".py";

void process_scene(const std::string &file, const std::string &scene)
{
    std::string cmd = BASE + file + " " + scene;
    std::cout << "Queued: " << cmd << std::endl;
    system(cmd.c_str());
}

int process_file(const std::filesystem::path &path) 
{
    std::ifstream file(path);
    std::string line;
    int count = 0;
    while (std::getline(file, line)) 
    {
        if (line.find("class ") != std::string::npos)
        {
            std::string scene = line.substr(line.find("class ") + 6);
            scene = scene.substr(0, scene.find("("));
            process_scene(path.string(), scene);
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    // get all the files that end with .py in the animation directory
    for (const auto &entry : 
        std::filesystem::directory_iterator(argc == 1 ? DIR : argv[1]))
    {
        if (entry.path().extension() == EXT)
        {
            int scenes = process_file(entry.path());
            std::cout << "Processed " << scenes << " scenes in " <<
                entry.path().string() << std::endl;
            std::cout << 
"--------------------------------------------------------------------------------" << std::endl;
        }
    }
    return 0;
}