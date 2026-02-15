#pragma once

#include "io.h"
#include "types.h"

#include "midi.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <arpa/inet.h>
#endif

void convert_note_on(u8 note, u8 *note_in_voice, FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 voice;
	u8 velocity;

	fread8(&voice,    1, bms_pointer);
	fread8(&velocity, 1, bms_pointer);

	note_in_voice[voice] = note;

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ %02X %02X %02X ] as ", note, voice, velocity);
	#endif

	fwrite_MIDI_track_event_note_on(0, note, velocity, midi_output_pointer);
}

void convert_note_off(u8 opCode, u8 *note_in_voice, FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 channel = opCode & 0x7F;
	u8 note    = note_in_voice[channel];

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ %02X ] as ", opCode);
	#endif
	
	fwrite_MIDI_track_event_note_off(0, note, 0x64, midi_output_pointer);
}

void convert_wait(u8 opCode, FILE *bms_pointer, FILE *midi_output_pointer) {

	u32 total_delta_time;

	do {

		if (opCode == 0x80) {
			u8 delta_time;
			fread8(&delta_time, 1, bms_pointer);
			total_delta_time += delta_time;

		} else if (opCode == 0x88) {
			u16 delta_time;
			fread16(&delta_time, 1, bms_pointer);
			total_delta_time += delta_time;

		}

		fread8(&opCode, 1, bms_pointer);

		if      (opCode == 0xC6 || opCode == 0xF4) { fseek(bms_pointer, 1l, SEEK_CUR); fread8(&opCode, 1, bms_pointer); }
		else if (opCode == 0x98 || opCode == 0xA4) { fseek(bms_pointer, 2l, SEEK_CUR); fread8(&opCode, 1, bms_pointer); }
		else if (opCode == 0x9A || opCode == 0x9C) { fseek(bms_pointer, 3l, SEEK_CUR); fread8(&opCode, 1, bms_pointer); }
		else if (opCode == 0xC4 || opCode == 0xC8) { fseek(bms_pointer, 4l, SEEK_CUR); fread8(&opCode, 1, bms_pointer); }

	} while (opCode == 0x80 || opCode == 0x88);

	// Move back so opCode is read again for the next operation
	// fprintf(stdout, "%lX ", ftell(bms_pointer));
	fseek(bms_pointer, -1l, SEEK_CUR);
	// fprintf(stdout, "%lX\n", ftell(bms_pointer));

	fwrite_MIDI_variable_length(total_delta_time + 1, midi_output_pointer);
}

void convert_tempo(FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 unknown;
	u8 bpm;

	fread8(&unknown, 1, bms_pointer);
	fread8(&bpm,     1, bms_pointer);

	u32 microseconds = (u32)roundf(60000000.0f / (float)bpm);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ FD %02X %02X ] as ", unknown, bpm);

		fprintf(stdout, "[[ 60,000,000 / %d ≈ %d]]", bpm, microseconds);
	#endif

	fwrite_MIDI_track_meta_event_tempo(microseconds, midi_output_pointer);
}

void convert_PPQN_set(MIDI_Header_Chunk *header, FILE *bms_pointer) {
	u16 ppqn;

	fread16(&ppqn, 1, bms_pointer);

	header->division = ppqn << 8 | ppqn >> 8;

	#ifdef WALK_THROUGH
		fprintf(stdout, "\n");
	#endif
}

void convert_track_define(MIDI_Header_Chunk *header, MIDI_Track_Chunk_Header *track_headers, u64 *track_pointers, FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 channel_number;
	u8 track_pointer_array[3];

	fread8(&channel_number,      1, bms_pointer);
	fread8( track_pointer_array, 3, bms_pointer);

	// Since track 0 is the global track in MIDI, and the global track is not defined with track define command in the BMS,
	// we assume track 0 until a track open, and since the first track in the BMS is track 0, we add one to that.
	track_headers[channel_number + 1].name   = 0x6B72544D; // 0x4D54726B
	track_headers[channel_number + 1].length = 0x00; // 0x4D54726B

	if (header->num_tracks > 1) {
		header->format = 0x0100;
	}
	
	#ifdef WALK_THROUGH
		fprintf(stdout, "\n");
	#endif
}

void convert_track_open(MIDI_Header_Chunk *header, MIDI_Track_Chunk_Header *track_headers, u64 *track_pointers, u64 *current_track, FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 stuff[2];

	fread8(stuff, 2, bms_pointer);

	track_pointers[++(*current_track)] = ftell(midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "track_pointers[%llu]: 0x%llX\n", *current_track, track_pointers[*current_track]);
	#endif

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ E7 %02X %02X ] as ", stuff[0], stuff[1]);
	#endif

	fwrite_MIDI_track_chunk_header(&track_headers[*current_track], midi_output_pointer);
	// fwrite_MIDI_variable_length(0x00, midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Track Open: %08X\n", (u32)track_pointers[*current_track]);
	#endif

	if (header->num_tracks == 0xFF00) {
		header->num_tracks++;
	} else {
		header->num_tracks += 0x0100;
	}

	if (header->num_tracks > 0x0100) {
		header->format = 0x0100;
	}
	
}

void convert_track_close(MIDI_Track_Chunk_Header *track_headers, u64 *track_pointers, u64 *current_track, FILE *midi_output_pointer) {
	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ FF ] as ");
	#endif
	
	fwrite_MIDI_track_meta_event_end_of_track(midi_output_pointer);

	track_headers[*current_track].name   = 0x6B72544D;
	track_headers[*current_track].length = htonl((ftell(midi_output_pointer) - track_pointers[*current_track] - 8)); // Subtrack the 8 bytes that make up the header

	u64 current_pointer = ftell(midi_output_pointer);

	fseek(midi_output_pointer, track_pointers[*current_track], SEEK_SET);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Moving to 0x%llX\n", track_pointers[*current_track]);
	#endif

	fwrite_MIDI_track_chunk_header(&track_headers [*current_track], midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Track Close: %08X\n", (u32)track_pointers[current_pointer]);
		fprintf(stdout, "Moving to 0x%llX\n", current_pointer);
	#endif
	fseek(midi_output_pointer, current_pointer, SEEK_SET);

}

void convert_param(u8 opCode, FILE *bms_pointer, FILE *midi_output_pointer) {
	u8 target;
	u8 value0;
	u8 value1;

	fread8(&target, 1, bms_pointer);
	fread8(&value0, 1, bms_pointer);

	if (opCode == 0x98) {
		#ifdef WALK_THROUGH
			fprintf(stdout, "Write [ %02X %02X %02X ] as ", opCode, target, value0);
		#endif

		// Write to general contoller that should have no effect until we know what it does
		fwrite_MIDI_track_event_controller(0x00, 0x48, 0x00, midi_output_pointer);

		return;
	}

	if (opCode != 0xA4) {
		fread8(&value1, 1, bms_pointer);
	}

	#ifdef WALK_THROUGH
		if (opCode != 0xA4) { fprintf(stdout, "Write [ %02X %02X %02X %02X ] as ", opCode, target, value0, value1); }
		else                { fprintf(stdout, "Write [ %02X %02X %02X ] as ",      opCode, target, value0        ); }
	#endif

	if      (target == 0x00) { fwrite_MIDI_track_event_controller(0x00, 0x07,   value0, midi_output_pointer); }
	else if (target == 0x01) { fwrite_MIDI_track_event_pitch_bend(0x00, value0, value1, midi_output_pointer); }
	else if (target == 0x02) { fwrite_MIDI_track_event_controller(0x00, 0x91,   value0, midi_output_pointer); }
	else if (target == 0x03) { fwrite_MIDI_track_event_controller(0x00, 0x10,   value0, midi_output_pointer); }
}

void convert_bank_prg() {

}

void convert_modulation(FILE *bms_pointer, FILE *midi_output_pointer) {
	u16 modulation;

	fread16(&modulation, 1, bms_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write [ E6 %04X ] as ", modulation);
	#endif

	u8 modulation_msb = ((u8)(modulation >> 7)) & 0x7F;
	u8 modulation_lsb = ((u8)(modulation >> 0)) & 0x7F;

	// if (modulation_msb == 0x00) {
	// 	fwrite_MIDI_track_event_controller(0x00, 0x01, modulation_lsb, midi_output_pointer);
	// } else {
		fwrite_MIDI_track_event_controller(0x00, 0x01, modulation_msb, midi_output_pointer);
		fwrite_MIDI_variable_length       (0x00,                       midi_output_pointer);
		fwrite_MIDI_track_event_controller(0x00, 0x33, modulation_lsb, midi_output_pointer);
	// }
}

void BMS_to_MIDI(FILE *bms_pointer, FILE *midi_output_pointer) {

	if (!bms_pointer        ) { fprintf(stderr, "File pointer is NULL\n"       ); return; }
	if (!midi_output_pointer) { fprintf(stderr, "MIDI output pointer is NULL\n"); return; }

	u8 current_byte;

	// In the event the program gets stuck in an infinite loop, kill the while loop if the number of
	// iterations excedes the file size
	fseek(bms_pointer, 0l, SEEK_END);
	u64 safety_limit = ftell(bms_pointer);
	fseek(bms_pointer, 0l, SEEK_SET);

	u32 safety = 0;
	
	u8                      note_in_voice  [  8];
	u64                     track_pointers [128];
	MIDI_Track_Chunk_Header track_headers  [128];
	MIDI_Header_Chunk       header;
	MIDI_Track_Chunk_Header global_track_header = track_headers[0];

	u64                     current_track = 0ll;

	header.name       = 0x6468544D;	// 0x4D546864
	header.format     = 0x0000;		// Assume one track until more appear
	header.num_tracks = 0x0100;		// "
	header.size       = 0x06000000; // Always 6
	header.division   = 0x78;		// 120 as a base (tends to be what it is)

	fwrite_MIDI_header_chunk(&header, midi_output_pointer);

	global_track_header.name   = 0x6B72544D;
	global_track_header.length = 0x00;

	track_pointers[current_track] = ftell(midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "track_pointers[%llu]: 0x%llX\n", current_track, track_pointers[current_track]);
	#endif

	fwrite_MIDI_track_chunk_header(&global_track_header, midi_output_pointer);
	fwrite_MIDI_variable_length(0x00, midi_output_pointer);

	u8 was_delta_time_before = 1;

	while (
		fread(&current_byte, 1, 1, bms_pointer) == 1 &&
		safety++ < safety_limit
	) {
		// If the current byte is 0x00, it's probably the end of the file.
		if (current_byte == 0x00) { break; }

		#ifdef WALK_THROUGH
			fprintf(stdout, "[ 0x%02X ] ", current_byte);
		#endif

		if (current_byte == 0x80 || current_byte == 0x88) { convert_wait          (current_byte,                                                bms_pointer, midi_output_pointer); was_delta_time_before = 1; continue; }
		if (current_byte == 0xC1                        ) { convert_track_define  (&header,      track_headers, track_pointers,                 bms_pointer, midi_output_pointer);                            continue; }
		if (current_byte == 0xFE                        ) { convert_PPQN_set      (&header,                                                     bms_pointer                     );                            continue; }
		if (current_byte == 0xFF                        ) { convert_track_close   (              track_headers, track_pointers, &current_track,              midi_output_pointer);                            continue; }

		if (
			current_byte == 0xC6 ||
			current_byte == 0xF4
		) {
			fseek(bms_pointer, 1l, SEEK_CUR);
			goto CLEANUP;
		}

		if (
			current_byte == 0x9A ||
			current_byte == 0x9C
		) {
			fseek(bms_pointer, 3l, SEEK_CUR);
			goto CLEANUP;
		}

		if (
			current_byte == 0xC4 ||
			current_byte == 0xC8
		) {
			fseek(bms_pointer, 4l, SEEK_CUR);
			goto CLEANUP;
		}

		if (!was_delta_time_before) { fwrite_MIDI_variable_length(0x00, midi_output_pointer); was_delta_time_before = 1; }

		if (current_byte >= 0x01 && current_byte <= 0x7F) { convert_note_on       (current_byte, note_in_voice,                                 bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }
		if (current_byte >= 0x81 && current_byte <= 0x87) { convert_note_off      (current_byte, note_in_voice,                                 bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }
		if (current_byte == 0xE7                        ) { convert_track_open    (&header,      track_headers, track_pointers, &current_track, bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }
		if (current_byte == 0xFD                        ) { convert_tempo         (                                                             bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }
		if (current_byte == 0xE6                        ) { convert_modulation    (                                                             bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }
		if (current_byte == 0x98 || current_byte == 0xA4) { convert_param         (current_byte,                                                bms_pointer, midi_output_pointer); was_delta_time_before = 0; continue; }

		#ifdef WALK_THROUGH
			fprintf(stderr, "[UNHANLDED BYTE] 0x%02X\n", current_byte);
		#endif
		return;

	CLEANUP:
		#ifdef WALK_THROUGH
			fprintf(stdout, "\n");
		#endif
		continue;

	}

	fseek(midi_output_pointer, 0l, SEEK_SET);

	fwrite_MIDI_header_chunk(&header, midi_output_pointer);
}