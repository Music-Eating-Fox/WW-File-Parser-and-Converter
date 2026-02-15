#pragma once

#include "io.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MIDI_Header_Chunk {	// MIDI Files are big endian, so byte order is reversed.

	u32 name;
	u32 size;
	u16 format;
	u16 num_tracks;
	i16 division;

} MIDI_Header_Chunk;

typedef struct MIDI_Track_Chunk_Header {

	u32 name;
	u32 length;

} MIDI_Track_Chunk_Header;

typedef enum MIDI_META_EVENT_TYPE {

	SEQUENCE_NUMBER                = 0x00,
	TEXT_EVENT                     = 0x01,
	COPYRIGHT_NOTICE               = 0x02,
	SEQUENCE_OR_TRACK_NAME         = 0x03,
	INSTRUMENT_NAME                = 0x04,
	LYRIC_TEXT                     = 0x05,
	MARKER_TEXT                    = 0x06,
	CUE_POINT                      = 0x07,
	PROGRAM_NAME                   = 0x08,
	DEVICE_NAME                    = 0x09,
	MIDI_CHANNEL_PREFIX_ASSIGNMENT = 0x20,
	END_OF_TRACK                   = 0x2F,
	TEMPO_SETTING                  = 0x51,
	SMPTE_OFFSET                   = 0x54,
	TIME_SIGNATURE                 = 0x58,
	KEY_SIGNATURE                  = 0x59,
	SEQUENCER_SPECIFIC_EVENT       = 0x7F,

} MIDI_META_EVENT_TYPE;

void fwrite_MIDI_variable_length(u32 value, FILE *midi_output_pointer) {

	u8 variable_length[5] = {
		((u8)(value >> 28) & 0x7F) | 0x80,
		((u8)(value >> 21) & 0x7F) | 0x80,
		((u8)(value >> 14) & 0x7F) | 0x80,
		((u8)(value >>  7) & 0x7F) | 0x80,
		((u8)(value >>  0) & 0x7F),
	};

	for (int i = 0; i < 5; i++) {
		if (variable_length[i] != 0x80) {
			#ifdef WALK_THROUGH
				fprintf(stdout, "[ ");
			#endif

			#ifdef WALK_THROUGH
				for (int j = i; j < 5; j++) {
					fprintf(stdout, "%02X ", variable_length[j]);
				}
				
				fprintf(stdout, "] at 0x%lX?", ftell(midi_output_pointer));	
				
				fflush(stdin);
				getchar();
			#endif

			fwrite(&variable_length[i], sizeof(u8), 5 - i, midi_output_pointer);
			break;
		}
	}
}

void fwrite_MIDI_header_chunk(MIDI_Header_Chunk *midi_header_chunk, FILE *midi_output_pointer) {
	#ifdef WALK_THROUGH
		fprintf(
			stdout, "[ %08X %08X %04X %04X %04X ] at 0x%lX?",
			midi_header_chunk->name,
			midi_header_chunk->size,
			midi_header_chunk->format,
			midi_header_chunk->num_tracks,
			midi_header_chunk->division,
			ftell(midi_output_pointer)
		);

		fflush(stdin);
		getchar();
	#endif

	// fwrite(midi_header_chunk, sizeof(MIDI_Header_Chunk), 1, midi_output_pointer); // Isn't packed, so write two bogus bytes
	fwrite(&midi_header_chunk->name,       sizeof(u32), 1, midi_output_pointer);
	fwrite(&midi_header_chunk->size,       sizeof(u32), 1, midi_output_pointer);
	fwrite(&midi_header_chunk->format,     sizeof(u16), 1, midi_output_pointer);
	fwrite(&midi_header_chunk->num_tracks, sizeof(u16), 1, midi_output_pointer);
	fwrite(&midi_header_chunk->division,   sizeof(u16), 1, midi_output_pointer);

	// u32 name   = 0x6468544D;
	// u32 length = 0x0600; 
}

void fwrite_MIDI_track_chunk_header(MIDI_Track_Chunk_Header *midi_track_chunk_header, FILE *midi_output_pointer) {
	#ifdef WALK_THROUGH
		fprintf(
			stdout, "[ %08X %08X ] at 0x%lX?",
			htonl(midi_track_chunk_header->name),
			htonl(midi_track_chunk_header->length),
			ftell(midi_output_pointer)
		);

		fflush(stdin);
		getchar();
	#endif

	fwrite(midi_track_chunk_header, sizeof(MIDI_Track_Chunk_Header), 1, midi_output_pointer);
}

void fwrite_MIDI_track_event_note_off(u8 channel, u8 note, u8 velocity, FILE *midi_output_pointer) {
	u8 data[3] = {
		0x80 | channel,
		note,
		velocity,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_event_note_on(u8 channel, u8 note, u8 velocity, FILE *midi_output_pointer) {
	u8 data[3] = {
		0x90 | channel,
		note,
		velocity,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_event_polyphonic_pressure(u8 channel, u8 note, u8 velocity, FILE *midi_output_pointer) {
	u8 data[3] = {
		0xA0 | channel,
		note,
		velocity,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_event_controller(u8 channel, u8 controller, u8 value, FILE *midi_output_pointer) {
	u8 data[3] = {
		0xB0 | channel,
		controller,
		value,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_event_program_change(u8 channel, u8 program, FILE *midi_output_pointer) {
	u8 data[2] = {
		0xC0 | channel,
		program,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X ] at 0x%lX?", data[0], data[1], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 2, midi_output_pointer);
}

void fwrite_MIDI_track_event_channel_pressure(u8 channel, u8 pressure, FILE *midi_output_pointer) {
	u8 data[2] = {
		0xD0 | channel,
		pressure,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X ] at 0x%lX?", data[0], data[1], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 2, midi_output_pointer);
}

void fwrite_MIDI_track_event_pitch_bend(u8 channel, u8 lsb, u8 msb, FILE *midi_output_pointer) {
	u8 data[3] = {
		0xE0 | channel,
		lsb,
		msb,
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_sequence_number(u16 sequence_number, FILE *midi_output_pointer) {
	u8 data[3] = {
		0xFF,	// OpCode
		0x00,	// Type
		0x02,	// Size
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite( data,            sizeof(u8 ), 3, midi_output_pointer);
	fwrite(&sequence_number, sizeof(u16), 1, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_text(MIDI_META_EVENT_TYPE event_type, const char *text, FILE *midi_output_pointer) {
	u8  opCode = 0xFF;
	u32 length = strlen(text);

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X ] at 0x%lX?", opCode, (u8)event_type, ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(&opCode,     sizeof(u8), 1, midi_output_pointer);
	fwrite(&event_type, sizeof(u8), 1, midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write variable length %08X as ", length);
	#endif

	fwrite_MIDI_variable_length(length, midi_output_pointer);

	#ifdef WALK_THROUGH
		fprintf(stdout, "Write text \"%s\" at 0x%lX?", text, ftell(midi_output_pointer));
	#endif

	fwrite(text, sizeof(char), length, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_channel_prefix(u8 channel, FILE *midi_output_pointer) {
	u8 data[4] = {
		0xFF,		// OpCode
		0x20,		// Type
		0x01,		// Size
		channel,	// Channel
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 4, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_midi_port(u8 midi_port, FILE *midi_output_pointer) {
	u8 data[4] = {
		0xFF,		// OpCode
		0x21,		// Type
		0x01,		// Size
		midi_port,	// Midi Port
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif
	
	fwrite(data, sizeof(u8), 4, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_end_of_track(FILE *midi_output_pointer) {
	u8 data[3] = {
		0xFF,	// OpCode
		0x2F,	// Type
		0x00,	// Size
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 3, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_tempo(u32 tempo, FILE *midi_output_pointer) {
	u8 data[6] = {
		0xFF,				// OpCode
		0x51,				// Type
		0x03,				// Size
		(u8)(tempo >> 16),	// XX 00 00
		(u8)(tempo >>  8),	// 00 XX 00
		(u8)(tempo >>  0),	// 00 00 XX
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], data[4], data[5], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 6, midi_output_pointer);
}

void fwrite_MIDI_track_meta_event_SMPTE_offset(u8 hours, u8 minutes, u8 seconds, u8 frames, u8 hundreth_frames, FILE *midi_output_pointer) {
	u8 data[8] = {
		0xFF,				// OpCode
		0x54,				// Type
		0x05,				// Size
		hours,				// hours
		minutes,			// minutes
		seconds,			// seconds
		frames,				// frames
		hundreth_frames,	// 100ths of a frame
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 8, midi_output_pointer);
}

/// @brief 
/// @param numerator Notated Numerator
/// @param denominator Notated Denominator as a negative power of two (e.g., 2 -> 1, 4 -> 2, 8 -> 3)
/// @param clocks_per_met Number of MIDI clocks between metronome clicks
/// @param thirty_seconds_per_24_clocks Number of notated 32nd notes per MIDI quarter note (24 MIDI clocks) [normally 8]
/// @param midi_output_pointer Pointer to MIDI file
void fwrite_MIDI_track_meta_event_time_signature(u8 numerator, u8 denominator, u8 clocks_per_met, u8 thirty_seconds_per_24_clocks, FILE *midi_output_pointer) {
	u8 data[7] = {
		0xFF,							// OpCode
		0x58,							// Type
		0x04,							// Size
		numerator,						// Numerator
		denominator,					// Denominator
		clocks_per_met,					// MIDI Clocks between metronome clicks
		thirty_seconds_per_24_clocks,	// Number of notated 32nd notes per 24 MIDI clocks
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], data[4], data[5], data[6], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 7, midi_output_pointer);
}
/// @brief 
/// @param number_of_sharps Specifies the number of sharps (if positive) or flats (if negative)
/// @param major_minor 0 -> major, 1 -> minor
/// @param midi_output_pointer Pointer to MIDI file
void fwrite_MIDI_track_meta_event_key_signature(i8 number_of_sharps, u8 major_minor, FILE *midi_output_pointer) {
	u8 data[5] = {
		0xFF,				// OpCode
		0x59,				// Type
		0x02,				// Size
		number_of_sharps,	// Number of sharps/flats
		major_minor,		// Major/Minor
	};

	#ifdef WALK_THROUGH
		fprintf(stdout, "[ %02X %02X %02X %02X %02X ] at 0x%lX?", data[0], data[1], data[2], data[3], data[4], ftell(midi_output_pointer));
		
		fflush(stdin);
		getchar();
	#endif

	fwrite(data, sizeof(u8), 5, midi_output_pointer);
}