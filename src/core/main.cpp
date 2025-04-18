//
//  main.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <src/core/singleExecution.hpp>
#include <src/core/network/networkExecution.hpp>
#include <vector>
#include <algorithm>
#include <src/config/config.h>
#include <src/config/unused/oldconfig.h>
#include <time.h>
#include <filesystem>
#include <string>

int main() {
    if (MODE == 0) { // single simulation
        clock_t tStart = clock();
        if (GRAPHICS_ON) {
            singleRunWithGraphics();
        } else {
            singleRunWithoutGraphics();
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        return 0;
    }
    if (MODE == 1 && !BULKMODE) { // simulation network
        clock_t tStart = clock();
        if (GRAPHICS_ON) {
            networkRunWithGraphics("config/network_config.yaml");
        } else {
            networkRunWithoutGraphics("config/bulkconfigs/network_config_0.yaml");
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        return 0;
    }
    if (MODE == 1 && BULKMODE) { // simulation network bulk run for mlp training data generation
        clock_t tStart = clock();
        std::string bulkConfigDir = "config/bulkconfigs/";
        std::vector<std::string> configFiles;
        
        // Check if directory exists
        if (!std::filesystem::exists(bulkConfigDir)) {
            std::cerr << "Error: Directory " << bulkConfigDir << " does not exist." << std::endl;
            return 1;
        }
        
        // Collect all YAML files in the directory
        for (const auto& entry : std::filesystem::directory_iterator(bulkConfigDir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.find("network_config_") != std::string::npos) {
                    // Check file extension
                    std::string extension = entry.path().extension().string();
                    if (extension == ".yaml" || extension == ".yml") {
                        configFiles.push_back(entry.path().string());
                    }
                }
            }
        }
        
        // Sort files to process them in order
        std::sort(configFiles.begin(), configFiles.end());
        
        if (configFiles.empty()) {
            std::cout << "No configuration files found in " << bulkConfigDir << std::endl;
            return 0;
        }
        
        // Process each configuration file
        std::cout << "Found " << configFiles.size() << " configuration files to process." << std::endl;
        for (size_t i = 0; i < configFiles.size(); ++i) {
            std::cout << "Processing [" << (i+1) << "/" << configFiles.size() << "]: " 
                      << configFiles[i] << std::endl;
            
            // Run simulation with this config
            std::cout << configFiles[i] << std::endl;
            networkRunWithoutGraphics(configFiles[i]);
        }
        
        printf("Total time taken for bulk processing: %.2fs\n", 
               (double)(clock() - tStart)/CLOCKS_PER_SEC);
        return 0;
    }
}
