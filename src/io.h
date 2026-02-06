#pragma once

#include "rarc.h"

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <arpa/inet.h>
#endif

void fread8(u8 *buffer, size_t nitems, FILE *stream) {
	fread(buffer, sizeof(u8), nitems, stream);
}

void fread16(u16 *buffer, size_t nitems, FILE *stream) {
	fread(buffer, sizeof(u16), nitems, stream);

	*buffer = htons(*buffer);
}

void fread32(u32 *buffer, size_t nitems, FILE *stream) {
	fread(buffer, sizeof(u32), nitems, stream);

	*buffer = htonl(*buffer);
}

RARC_Header RARC_read_header(FILE *header_pointer) {
	RARC_Header header;

	fread8  ( header.header,             4, header_pointer);
	fread32 (&header.size,               1, header_pointer);
	fread32 (&header.data_header_offset, 1, header_pointer);
	fread32 (&header.file_data_offset,   1, header_pointer);
	fread32 (&header.file_data_length,   1, header_pointer);
	fread32 (&header.MRAM_size,          1, header_pointer);
	fread32 (&header.ARAM_size,          1, header_pointer);
	fread32 (&header.DVD_size,           1, header_pointer);

	return header;
}

RARC_Data_Header RARC_read_data_header(FILE *data_header_pointer) {
	RARC_Data_Header data_header;

	fread32 (&data_header.num_directory_nodes,       1, data_header_pointer);
	fread32 (&data_header.directory_offset,          1, data_header_pointer);
	fread32 (&data_header.num_file_nodes,            1, data_header_pointer);
	fread32 (&data_header.file_nodes_offset,         1, data_header_pointer);
	fread32 (&data_header.string_table_size,         1, data_header_pointer);
	fread32 (&data_header.string_table_offset,       1, data_header_pointer);
	fread16 (&data_header.next_available_file_index, 1, data_header_pointer);
	fread8  (&data_header.keep_file_ids_synced,      1, data_header_pointer);
	fread8  ( data_header.padding,                   5, data_header_pointer);

	return data_header;
}

RARC_Directory_Node_Section RARC_read_directory_node_section(FILE *directory_node_section_pointer) {
	RARC_Directory_Node_Section directory_node_section;

	fread8  ( directory_node_section.type,                   4, directory_node_section_pointer);
	fread32 (&directory_node_section.string_table_offset,    1, directory_node_section_pointer);
	fread16 (&directory_node_section.name_hash,              1, directory_node_section_pointer);
	fread16 (&directory_node_section.num_file_nodes,         1, directory_node_section_pointer);
	fread32 (&directory_node_section.first_file_node_offset, 1, directory_node_section_pointer);

	return directory_node_section;
}

RARC_File_Node RARC_read_file_node(FILE *file_node_pointer) {
	RARC_File_Node file_node;

	fread16 (&file_node.node_index,          1, file_node_pointer);
	fread16 (&file_node.name_hash,           1, file_node_pointer);
	fread8  (&file_node.node_attributes,     1, file_node_pointer);
	fread8  (&file_node.padding,             1, file_node_pointer);
	fread16 (&file_node.string_table_offset, 1, file_node_pointer);

	if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_FILE) {
		fread32(&file_node.file_data_offset, 1, file_node_pointer);
		fread32(&file_node.file_data_size,   1, file_node_pointer);

	} else if (file_node.node_attributes & RARC_NODE_ATTRIBUTE_DIRECTORY) {
		fread32(&file_node.directory_node_index, 1, file_node_pointer);
		fread32(&file_node.directory_node_size,  1, file_node_pointer);

	}

	fread32(&file_node.ending_padding, 1, file_node_pointer);

	return file_node;
}