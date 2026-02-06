#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

typedef struct RARC_Header {

	u8  header[4];					// Should always be "RARC"
	u32 size;
	u32 data_header_offset;			// Should always be 0x20
	u32 file_data_offset;			// Minus 0x20
	u32 file_data_length;
	u32 MRAM_size;
	u32 ARAM_size;
	u32 DVD_size;

} RARC_Header;

typedef struct RARC_Data_Header {

	// Data header
	u32 num_directory_nodes;
	u32 directory_offset;			// Minus 0x20 (should always be 0x20)
	u32 num_file_nodes;
	u32 file_nodes_offset;			// Minus 0x20
	u32 string_table_size;
	u32 string_table_offset;
	u16 next_available_file_index;
	u8  keep_file_ids_synced;		// bool; I don't know what it does
	u8  padding[5];					// All zeros (0x00)

} RARC_Data_Header;

typedef enum RARC_NODE_ATTRIBUTES {

	RARC_NODE_ATTRIBUTE_FILE            = 0x01,
	RARC_NODE_ATTRIBUTE_DIRECTORY       = 0x02,
	RARC_NODE_ATTRIBUTE_COMPRESSED      = 0x04,
	RARC_NODE_ATTRIBUTE_PRELOAD_TO_MRAM = 0x10,
	RARC_NODE_ATTRIBUTE_PRELOAD_TO_ARAM = 0x20,
	RARC_NODE_ATTRIBUTE_LOAD_FROM_DVD   = 0x40,
	RARC_NODE_ATTRIBUTE_YAZ0_COMPRESSED = 0x80,	// Requires that `COMPRESSED` is set as well

} RARC_NODE_ATTRIBUTES;

typedef struct RARC_Directory_Node_Section {

	u8  type[4];					// First four characters of directory name in all caps
	u32 string_table_offset;		// Offset to directory's name in the string table
	u16 name_hash;					// Hash of directory's name
	u16 num_file_nodes;
	u32 first_file_node_offset;		// Offset to the first file node in file nodes section

} RARC_Directory_Node_Section;

typedef struct RARC_File_Node {

	u16 node_index;					// = 0xFFFF if this is a subdirectory
	u16 name_hash;					// Hash of node's name
	u8  node_attributes;
	u8  padding;					// = 0x00
	u16 string_table_offset;		// Offset to node's name in the string table

	/**
	 * @if this node is a file, use the first `u32`
	 * @elseif this node is a directory, use the second `u32`
	 */

	union {
		u32 file_data_offset;
		u32 directory_node_index;
	};
	union {
		u32 file_data_size;
		u32 directory_node_size;	// Always 0x10
	};

	u32 ending_padding;				// Always 0x00000000
	
} RARC_File_Node;

typedef struct RARC {

	RARC_Header                 header;
	RARC_Data_Header            data_header;
	RARC_Directory_Node_Section directory_node;

} RARC;