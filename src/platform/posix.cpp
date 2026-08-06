#include "platform.hpp"                               
#include <dirent.h>                                   
#include <sys/stat.h>                                 
#include <unistd.h>                                   
#include <cstring>                                    
#include <cerrno>                                     

bool traverse_directory(const fs_string& path, std::vector<FileEntry>& out_entries) {  
    DIR* dir = opendir(path.c_str());                 
    if (!dir) {                                       
        return false;                                 
    }                                                 

    struct dirent* entry;                             
    while ((entry = readdir(dir)) != nullptr) {       
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {  
            continue;                                 
        }                                             

        fs_string full_path = path;                   
        if (full_path.back() != '/') {                
            full_path += '/';                         
        }                                             
        full_path += entry->d_name;                   

        FileEntry file_entry;                         
        file_entry.path = full_path;                  
        file_entry.name = entry->d_name;              

        struct stat st;                               
        if (stat(full_path.c_str(), &st) == 0) {      
            file_entry.is_directory = S_ISDIR(st.st_mode);  
            file_entry.size = st.st_size;             
            file_entry.modified_time = st.st_mtime;   
        } else {                                      
            continue;                                 
        }                                             

        out_entries.push_back(file_entry);            
    }                                                 

    closedir(dir);                                    
    return true;                                      
}                                                     