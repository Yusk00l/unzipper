#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <sys/inotify.h>
#include <unistd.h>
#include <string>

int main(int argc, char* argv[]) {

    std::string command = argv[1];
    bool applyChmod = false;
    if (command == "--chmod")
    {
        applyChmod = true;
    }

    std::string watchPath = "/home/" + std::string(getenv("USER")) + "/Downloads";

    int fd = inotify_init();
    if (fd < 0) {
        std::cerr << "Failed to init inotify" << std::endl;
        return 1;
    }
    int wd = inotify_add_watch(fd, watchPath.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);
    if (wd < 0) {
        std::cerr << "Failed to watch folder" << std::endl;
        return 1;
    }

    std::cout << "Watching: " << watchPath << std::endl;

    char buffer[4096];

    while (true) {
        int length = read(fd, buffer, sizeof(buffer));
        if (length < 0) {
            std::cerr << "Read error" << std::endl;
            break;
        }

        struct inotify_event* event = (struct inotify_event*)buffer;

        if (event->len > 0) {
            std::string filename = event->name;

            if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".zip") {
                std::string fullPath = watchPath + "/" + filename;
                std::string extractPath = watchPath + "/" + filename.substr(0, filename.size() - 4);

                std::cout << "Zip detected: " << filename << std::endl;

                std::filesystem::create_directory(extractPath);
                
                std::string unzipCmd = "unzip -o \"" + fullPath + "\" -d \"" + extractPath + "\"";
                int unzipResult = system(unzipCmd.c_str());

                if (unzipResult != 0) {
                    std::cerr << "Failed to unzip: " << filename << std::endl;
                    continue;
                }

                std::cout << "Extracted to: " << extractPath << std::endl;

                if (applyChmod == true) {
                std::string chmodCmd = "chmod -R 755 \"" + extractPath + "\"";
                system(chmodCmd.c_str());
                }

                std::cout << "Permissions set" << std::endl;

                std::filesystem::remove(fullPath);

                std::cout << "Deleted zip: " << filename << std::endl;
                std::cout << "Done!" << std::endl;
            } 
            else {std::cout << "Ignoring: " << filename << std::endl;}
      } //end of if statement
    } //end of while loop

    close(fd);
    return 0;
} //end of main

//the decription that is unfinished

/*
a program for linux mint that can run in the backround. 
Its 3 features:
1.Auto extracts any zip file that goes into the download folder
Disclaimer: does not work on zip folders with passwords on them, it will just skip it.
2.automatically sets up the permisions so the file can be able to read and write and be able to run as a program
3.

*/
//also would like to add more features