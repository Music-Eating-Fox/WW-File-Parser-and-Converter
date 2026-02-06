#include "io.h"
#include "bms.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char **argv) {
	if (argv[1]) {
		if (strcmp(argv[1], "convert") == 0) {
			if (!argv[2]) {
				fprintf(stderr, "\033[31mERROR: No file to convert!\n\033[0m");
				return 0;
			}

			char input_file_name  [128];
			char output_file_name [128];

			snprintf(input_file_name,  sizeof(input_file_name),  "./files/bms/%s.bms",  argv[2]);
			snprintf(output_file_name, sizeof(output_file_name), "./files/txt/%s.ansi", argv[2]);

			FILE *bms_pointer        = fopen(input_file_name,  "rb");
			FILE *txt_output_pointer = fopen(output_file_name, "w" );
			// FILE *txt_output_pointer = stderr;

			if (!bms_pointer) {
				fprintf(stderr, "\033[1;31mFATAL: Failed to open file \"%s\"!\n\033[0m", input_file_name);
			}

			if (!txt_output_pointer) {
				fprintf(stderr, "\033[1;31mFATAL: Failed to create output file \"%s\"!\n\033[0m", output_file_name);
			}

			BMS_parse(bms_pointer, txt_output_pointer);

			return 0;
		}
	}

	FILE *file_pointer_binary = fopen("../data/JaiSeqs.arc", "rb");
	FILE *file_pointer_string = fopen("../data/JaiSeqs.arc", "r" );

	RARC rarc = { 0 };
	rarc.header = RARC_read_header(file_pointer_binary);

	fseek(file_pointer_binary, (long)rarc.header.data_header_offset, SEEK_SET);

	rarc.data_header = RARC_read_data_header(file_pointer_binary);

	fseek(file_pointer_binary, (long)(rarc.data_header.directory_offset + 0x20), SEEK_SET);

	rarc.directory_node = RARC_read_directory_node_section(file_pointer_binary);

	printf("----- RARC FILE -----\n"                                                                                                                                        );
	printf("  - Header:\n"                                                                                                                                                  );
	printf("      - File Type              : %c%c%c%c\n", rarc.header.header[0], rarc.header.header[1], rarc.header.header[2], rarc.header.header[3]                        );
	printf("      - Size                   : 0x%08X\n",   rarc.header.size                                                                                                  );
	printf("      - Data Header Offset     : 0x%08X\n",   rarc.header.data_header_offset                                                                                    );
	printf("      - File Data Offset       : 0x%08X\n",   rarc.header.file_data_offset                                                                                      );
	printf("      - File Data Length       : 0x%08X\n",   rarc.header.file_data_length                                                                                      );
	printf("      - MRAM Size              : 0x%08X\n",   rarc.header.MRAM_size                                                                                             );
	printf("      - ARAM Size              : 0x%08X\n",   rarc.header.ARAM_size                                                                                             );
	printf("      - DVD Size               : 0x%08X\n",   rarc.header.DVD_size                                                                                              );
	printf("  -  Data Header:\n"                                                                                                                                            );
	printf("      - Directory Nodes        : %d\n",       rarc.data_header.num_directory_nodes                                                                              );
	printf("      - Directory Offset       : 0x%08X\n",   rarc.data_header.directory_offset                                                                                 );
	printf("      - File Nodes             : %d\n",       rarc.data_header.num_file_nodes                                                                                   );
	printf("      - File Offset            : 0x%08X\n",   rarc.data_header.file_nodes_offset                                                                                );
	printf("      - String Table Size      : 0x%08X\n",   rarc.data_header.string_table_size                                                                                );
	printf("      - String Table Offset    : 0x%04X\n",   rarc.data_header.string_table_offset                                                                              );
	printf("      - Keep File IDs Synced   : %s\n",       rarc.data_header.keep_file_ids_synced ? "True" : "False"                                                          );
	printf("  -  Directory Node Section:\n"                                                                                                                                 );
	printf("      - Type                   : %c%c%c%c\n", rarc.directory_node.type[0], rarc.directory_node.type[1], rarc.directory_node.type[2], rarc.directory_node.type[3]);
	printf("      - String Table Offset    : 0x%08X\n",   rarc.directory_node.string_table_offset                                                                           );
	printf("      - Directory Name Hash    : %d\n",       rarc.directory_node.name_hash                                                                                     );
	printf("      - File Nodes             : %d\n",       rarc.directory_node.num_file_nodes                                                                                );
	printf("      - First File Node Offset : 0x%08X\n",   rarc.directory_node.first_file_node_offset                                                                        );

	fseek(file_pointer_string, rarc.data_header.string_table_offset + 0x20 + rarc.directory_node.string_table_offset, SEEK_SET);

	char directory_name[256];
	fread(directory_name, sizeof(u8), sizeof(directory_name), file_pointer_string);
	printf("      - Name                   : %s\n",       directory_name);

	// Stuff to be used in loop
	char            file_name_string[256];
	RARC_File_Node  file_node;
	void           *buffer0   = malloc(2048);
	void           *buffer1   = malloc(2048);
	int             size      = 0;

	for (int i = 0; i < rarc.data_header.num_directory_nodes + rarc.data_header.num_file_nodes; i++) {
		// Ignore the magic numbers; I'm trying to figure out how to get these values in code, but those are
		// the addresses we need
		fseek(file_pointer_binary, (long)(0x60 + (i * sizeof(RARC_File_Node))), SEEK_SET);

		file_node = RARC_read_file_node(file_pointer_binary);

		printf("  - File Node:\n");

		if (file_node.node_index == 0xFFFF) { printf("      - Node index             : Subdirectory\n"            ); }
		else                                { printf("      - Node index             : %d\n", file_node.node_index); }

		printf("      - File Name Hash         : %d\n", file_node.name_hash);
		printf("      - Node Attributes        : "                         );

		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_FILE           ) { printf("FILE, "           ); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_DIRECTORY      ) { printf("DIRECTORY, "      ); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_COMPRESSED     ) { printf("COMPRESSED, "     ); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_PRELOAD_TO_MRAM) { printf("PRELOAD_TO_MRAM, "); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_PRELOAD_TO_ARAM) { printf("PRELOAD_TO_ARAM, "); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_LOAD_FROM_DVD  ) { printf("LOAD_FROM_DVD, "  ); }
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_YAZ0_COMPRESSED) { printf("YAZO_COMPRESSED, "); }
		printf("\n");

		printf("      - String Table Offset    : 0x%04X\n", file_node.string_table_offset);
		
		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_FILE) {
			printf("      - File Data Offset       : 0x%08X\n", file_node.file_data_offset);
			printf("      - File Data Size         : 0x%08X\n", file_node.file_data_size  );

		} else if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_DIRECTORY) {
			printf("      - Directory Node Index   : 0x%08X\n", file_node.file_data_offset);
			printf("      - Directory Node Size    : 0x%08X\n", file_node.file_data_size  );
		}

		fseek(file_pointer_string, (long)(rarc.data_header.string_table_offset + 0x20 + file_node.string_table_offset), SEEK_SET);

		fread(file_name_string, sizeof(u8), sizeof(file_name_string), file_pointer_string);

		size = 0;
		for (char *i = file_name_string; i < file_name_string + 256; i++, size++) { if (*i == 0x00) { break; } }
		printf("      - Name                   : %s\n", file_name_string);

		char file_location[512];
		snprintf(file_location, sizeof(directory_name) + 9, "./files/%s/%s%s", directory_name, file_name_string, (file_node.node_attributes & RARC_NODE_ATTRIBUTE_YAZ0_COMPRESSED) ? ".szs" : "");
		printf("      - Location               : %s\n", file_location);

		if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_DIRECTORY || strcmp(file_name_string, ".") == 0) { continue; }

		FILE *target_file_pointer = fopen(file_location, "wb");
		if (!target_file_pointer) { fprintf(stderr, "Failed to open file %s; you may need to add missing directories [%d]\n", file_location, i); continue; }

		fseek(file_pointer_binary, rarc.header.file_data_offset + 0x20 + file_node.file_data_offset, SEEK_SET);

		if (file_node.file_data_size > 2048) {
			size_t read_size = 0;

			while (file_node.file_data_size - read_size > 2048) {

				read_size += fread(buffer1, sizeof(u8), 2048, file_pointer_binary);
				
				fwrite(buffer1, sizeof(u8), 2048, target_file_pointer);
			}

			fread(buffer1, sizeof(u8), file_node.file_data_size - read_size, file_pointer_binary);
			fread(buffer1, sizeof(u8), file_node.file_data_size - read_size, target_file_pointer);
		}

		fclose(target_file_pointer);
	}

	free(buffer0);
	free(buffer1);

	char string_table[rarc.data_header.string_table_size];

	fseek(file_pointer_string, (long)(rarc.data_header.string_table_offset + 0x20), SEEK_SET);

	fread(string_table, sizeof(u8), rarc.data_header.string_table_size, file_pointer_string);

	printf("  - String Table:\n      - ");

	for (char *i = string_table; i < string_table + rarc.data_header.string_table_size; i++) {
		if (*i == 0x00) { printf("\n      - "); }
		else            { printf("%c", *i    ); }
	}

	fclose(file_pointer_binary);
	fclose(file_pointer_string);

	return 0;
}