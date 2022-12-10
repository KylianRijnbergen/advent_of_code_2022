#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Defines */
#define MAX_FILENAME_LENGTH 30
#define MAX_DIRNAME_LENGTH 30
#define MAX_SUBDIRECTORIES 50
#define MAX_FILES_IN_DIRECTORY 50


/* Struct declarations */
typedef struct Directory Directory;
typedef struct File File;

/* Function declarations */
File* create_file(char* filename, long long filesize);
Directory* create_directory(char* dirname);
void add_subdirectory_to_directory(Directory* parent_directory, Directory* subdirectory);
void add_file_to_directory(Directory* directory, File* file);
Directory* create_root(void);
void print_directory(Directory* root);
void print_file(File* file);
Directory* cmd_to_root(void);
Directory* build_tree_from_file(FILE* file);
bool string_equals(char* string1, char* string2, int char_count);
bool subdir_exists(Directory* directory, char* subdir_name);
bool file_exists(Directory* directory, char* file_name);
char* get_dirname(char* line, int offset);
Directory* change_directory(Directory* current, char* target_dirname);
File* new_file_from_line(char* line);
void eval_tree(Directory* tree);
void traverse_tree(Directory* dir, long long* records);

/* Struct definitions */
typedef struct Directory {
	char *dirname;
	Directory* parent_directory;
	Directory* directories[MAX_SUBDIRECTORIES];
	int num_subdirectories;
	File* files[MAX_FILES_IN_DIRECTORY];
	int num_files;
	long long total_size;
} Directory;

typedef struct File {
	char* filename;
	Directory* parent_directory;
	long long filesize;
} File;

long long counter = 0;

/* Function definitions */
int main(int argc, char* argv) {
	Directory* root = NULL;

	FILE* file = fopen("input.txt", "r");
	root = build_tree_from_file(file);
	fclose(file);
	eval_tree(root);

	long long* records = malloc(sizeof(long long) * 2000);
	for (unsigned int i = 0; i < 2000; i++) {
		records[i] = 0;
	}
	long long hdd_size = 70000000;
	long long needed = 30000000;
	long long occupied = root->total_size;
	long long to_delete = needed - (hdd_size - occupied);
	traverse_tree(root, records);

	long long current_best = 99999999999;
	for (unsigned int i = 0; i < 2000; i++) {
		if (records[i] >= to_delete && records[i] <= current_best) {
			current_best = records[i];
		}
	}
	printf("The best directory to delete has size %lld\n", current_best);

	return 0;
}

File* create_file(char* filename, long long filesize) {
	File* file = malloc(sizeof(File));
	file->filename = filename;
	file->parent_directory = NULL;
	file->filesize = filesize;
	return file;
}

Directory* create_directory(char* dirname) {
	Directory* dir = malloc(sizeof(Directory));
	dir->dirname = dirname;
	dir->parent_directory = NULL;
	for (unsigned int i = 0; i < MAX_SUBDIRECTORIES; i++) {
		dir->directories[i] = NULL;
	}
	dir->num_subdirectories = 0;
	for (unsigned int i = 0; i < MAX_FILES_IN_DIRECTORY; i++) {
		dir->files[i] = NULL;
	}
	dir->num_files = 0;
	dir->total_size = 0;
	return dir;
}

void add_subdirectory_to_directory(Directory* parent_directory, Directory* subdirectory) {
	parent_directory->directories[parent_directory->num_subdirectories] = subdirectory;
	parent_directory->num_subdirectories++;
	subdirectory->parent_directory = parent_directory;
}

void add_file_to_directory(Directory* directory, File* file) {
	directory->files[directory->num_files] = file;
	directory->num_files++;
	file->parent_directory = directory;
}

Directory* create_root(void) {
	Directory* root = create_directory("/");
	return root;
}

void print_directory(Directory* root) {
	static int indentation_level = 0; 
	printf("- %s (dir, size=%lld)\n", root->dirname, root->total_size);
	int subdir_count = root->num_subdirectories;
	if (subdir_count == 0) {
	} else {
		indentation_level++;
		for (unsigned int i = 0; i < subdir_count; i++) {
			for (unsigned int j = 0; j < indentation_level; j++) {
				printf("  ");
			}
			print_directory(root->directories[i]);
		}
		indentation_level--;
	}
	int file_count = root->num_files;
	if (file_count == 0) {
	}
	else {
		for (unsigned int i = 0; i < file_count; i++) {
			for (unsigned int j = 0; j < indentation_level + 1; j++) {
				printf("  ");
			}
			print_file(root->files[i]);
		}
	}
}

void print_file(File* file) {
	printf("- %s (file, size=%lld)\n", file->filename, file->filesize);
}

Directory* cmd_to_root(void) {
	Directory* root = create_root();
	root->dirname = malloc(sizeof(char) * 2);
	root->dirname = "/";
	return root;
}

Directory* build_tree_from_file(FILE* file) {
	Directory* root = NULL;
	Directory* current_directory = NULL;
	const int MAX_LINE_LENGTH = 50;
	char line[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		/* If we change to /, we create root. This is only done once. */
		if (string_equals(line, "$ cd /\n", 6)) {
			root = cmd_to_root();
			current_directory = root;
			continue;
		}

		/* If the command is cd .., we go up a level. */
		if (string_equals(line, "$ cd ..", 6)) {
			current_directory = current_directory->parent_directory;
			continue;
		}

		/* If the command is dir <dirname>, we should create a new directory. */
		if (string_equals(line, "dir ", 4)) {
			Directory* new_directory;
			char* dirname = get_dirname(line, 4);
			new_directory = create_directory(dirname);
			add_subdirectory_to_directory(current_directory, new_directory);
			continue;
		}

		/* Change directies */
		if (string_equals(line, "$ cd ", 5)) {
			char* dirname = get_dirname(line, 5);
			current_directory = change_directory(current_directory, dirname);
			continue;
		} 

		if (string_equals(line, "$ ls", 4)) {
			//ls doesnt do anything relevant to us
			continue;
		}

		/* If the first char is a number, we are to create a new file */
		if (line[0] >= 49 && line[0] <= 57) {
			File* file = new_file_from_line(line);
			add_file_to_directory(current_directory, file);
			continue;
		}
	}
	return root;
}

bool string_equals(char* string1, char* string2, int char_count) {
	return !strncmp(string1, string2, char_count);
}

bool subdir_exists(Directory* directory, char* subdir_name) {
	char* current_subdirectory_name = malloc(sizeof(char) * MAX_DIRNAME_LENGTH);
	for (unsigned int i = 0; i < directory->num_subdirectories; i++) {
		current_subdirectory_name = directory->directories[i]->dirname;
		if (string_equals(subdir_name, current_subdirectory_name, MAX_DIRNAME_LENGTH)) {
			//free(current_subdirectory_name);
			return true;
		}
	}
	//free(current_subdirectory_name);
	return false;
}

bool file_exists(Directory* directory, char* file_name) {
	char* current_file_name;
	for (unsigned int i = 0; i < directory->num_files; i++) {
		current_file_name = directory->files[i]->filename;
		if (string_equals(file_name, current_file_name, MAX_FILENAME_LENGTH)) {
			return true;
		}
	}
	return false;
}

char* get_dirname(char* line, int offset) {
	char* name_start = line + offset;
	int string_length = strlen(name_start);
	char* dirname = malloc(sizeof(char) * (string_length));
	dirname = memcpy(dirname, name_start, string_length - 1);
	dirname[string_length - 1] = '\0';
	return dirname;
}

Directory* change_directory(Directory* current, char* target_dirname) {
	for (unsigned int i = 0; i < current->num_subdirectories; i++) {
		char* current_dir_name = current->directories[i]->dirname;
		if (string_equals(target_dirname, current_dir_name, strlen(target_dirname))) {
			return current->directories[i];
		}
	}
}

File* new_file_from_line(char* line) {
	char buff[20]; 
	int space_pos = 0;
	for (unsigned int i = 0; i < strlen(line); i++) {
		if (line[i] == ' ') {
			buff[i] = '\0';
			File* file;
			long long filesize = atoll(buff);

			char* filename = malloc(sizeof(char) * MAX_FILENAME_LENGTH);
			for (unsigned int j = space_pos + 1; j < MAX_FILENAME_LENGTH + space_pos; j++) {
				if (line[j] == '\n') {
					filename[j - space_pos - 1] = '\0';
					file = create_file(filename, filesize);
					return file;
				}
				filename[j - space_pos - 1] = line[j];
			}
		}
		else {
			space_pos++;
			buff[i] = line[i];
		}
	}
}

void eval_tree(Directory* tree) {
	/* Evaluate files */
	for (unsigned int i = 0; i < tree->num_files; i++) {
		tree->total_size += tree->files[i]->filesize;
	}
	/* Evaluate subdirectories */
	for (unsigned int i = 0; i < tree->num_subdirectories; i++) {
		eval_tree(tree->directories[i]);
		tree->total_size += tree->directories[i]->total_size;
	}
}

void traverse_tree(Directory* dir, long long* records) {
	records[counter] = dir->total_size;
	counter++;
	for (unsigned int i = 0; i < dir->num_subdirectories; i++) {
		traverse_tree(dir->directories[i], records);
	}
}