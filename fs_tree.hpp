

#ifndef FS_TREE_HPP
#define FS_TREE_HPP
  

#include <cstring> // string
#include <vector> // vector
#include <iostream>// cout

#include <dirent.h> // open dir


namespace ilrd {



//***************************************************************************

class Entry { // Component
public:
    inline Entry(const std::string& path_to_entry, size_t m_space); 
    virtual ~Entry();
    Entry(const Entry& other) = delete;
    Entry& operator=(const Entry& other) = delete;
    
    virtual void print() = 0;
    inline static void PrintsSpace(size_t space) noexcept;

    inline std::string& get_entry_name() noexcept;
    inline size_t get_space() noexcept {return m_space;}

private:
    std::string entry_name; 
   
    // for iddentation 
    size_t m_space;

}; 

//***************************************************************************

class Dir : public Entry { // Composite
public:
    
    Dir(const std::string& path_to_entry, size_t spaces);
   ~Dir();
    Dir(const Dir& other) = delete;
    Dir& operator=(const Dir& other) = delete;

    inline void add(Entry* entry);
    inline void print() override;
	
	//build tree for print 
	inline void BuildTree(const std::string& path_to_entry, size_t space = 0);
	
private:
    std::vector<Entry*> content;   

}; 

//***************************************************************************

class File : public Entry { // Leaf
public:
    File(const std::string& path_to_entry, size_t spaces); 
   ~File() override{};
    File(const File& other) = delete;
    File& operator=(const File& other) = delete;
    void print() override;
}; 

Entry::Entry(const std::string& path_to_entry, size_t space = 0): entry_name(path_to_entry), m_space(space) 
{}

Entry::~Entry()
{}

std::string& Entry::get_entry_name() noexcept
{
	return entry_name;
}
    

void BuildTree(const std::string& path_to_entry, size_t space = 0);
File::File(const std::string& path_to_entry, size_t spaces = 0): Entry(path_to_entry, spaces)
{}

Dir::Dir(const std::string& path_to_entry, size_t spaces = 0): Entry(path_to_entry, spaces)
{} 

Dir::~Dir()
{
	for (std::vector<Entry*>::iterator it = content.begin();it != content.end(); ++it){	
  		
  		delete *it;
	}
}

void Entry::PrintsSpace(size_t space) noexcept
{
	while(space != 0){
		std::cout << "──";
		--space;
	}
}


inline void Dir::BuildTree(const std::string& path_to_entry, size_t space)
{
	DIR *dir;
	
	if (!(dir = opendir(path_to_entry.c_str())))
    {
       std::string  eror =  "path not found: %s: %s\n";
       throw eror;  
    }
	
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;

        if (entry->d_type == DT_DIR)
        {
            if (!std::strcmp(name, ".") || !std::strcmp(name, ".."))
            {
                continue;
            }     
            else
            {
                std::string next_entry_path(path_to_entry);
                next_entry_path += '/';
                next_entry_path += name;
                content.push_back(new Dir(entry->d_name, space));
				
				try { 
					
					BuildTree(next_entry_path ,space + 2);
				}
				catch (const std::string&) {
					throw;
				}
            }
        } 
        else 
        {
			content.push_back(new File(name, space));
		}
     
    }
    
    closedir(dir);
}


void Dir::print()
{
	PrintsSpace(get_space());

	std:: cout << "\033[34;1m" << get_entry_name() << "\033[0m" <<"\n";
	
	for (std::vector<Entry*>::iterator it = content.begin();it != content.end(); ++it){
  		(*it)->print();
	}
}


void File::print() 
{
		
	PrintsSpace(get_space());
	
	std::cout << get_entry_name() << "\n";
}











}; // end-of-namespace ilrd
#endif //FS_TREE_HPP
