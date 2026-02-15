#pragma once

#include "io.h"
#include "types.h"

#include "midi.h"

#include <stdio.h>
#include <stdlib.h>

void BMS_parse_note_on(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {

	u8 key = current_byte;
	u8 voice;
	u8 velocity;

	const char *names[12] = {
		"C\u266E", "C\u266F", "D\u266E", "D\u266F", "E\u266E", "F\u266E", "F\u266F", "G\u266E", "G\u266F", "A\u266E", "A\u266F", "B\u266E"
	};

	fread8(&voice,    1, bms_pointer);
	fread8(&velocity, 1, bms_pointer);

	fprintf(txt_output_pointer, "Note On:");

	/* if (key      > 0x7F) { fprintf(txt_output_pointer, " \033[41m[note %d (%s%d)]\033[0m", key, names[key % 12], (int)(key / 12) - 1); } else { */fprintf(txt_output_pointer, " [note %d (%s%d)]", key, names[key % 12], (int)(key / 12) - 1); //}
	/* if (voice    >    7) { fprintf(txt_output_pointer, " \033[41m[voice %d]\033[0m",       voice                                    ); } else { */fprintf(txt_output_pointer, " [voice %d]",       voice                                    ); //}
	/* if (velocity > 0x7F) { fprintf(txt_output_pointer, " \033[41m[vel %d]\033[0m",         velocity                                 ); } else { */fprintf(txt_output_pointer, " [vel %d]",         velocity                                 ); //}

	if (voice > 7) {
		// fprintf(stderr, "Voice is greater than 7, meaning this is a gated note [unimplemented]\nVoice: %d\n", voice);
		u8 next;
		fread8(&next, 1, bms_pointer);
		fprintf(txt_output_pointer, " [? 0x%02X]", next);
	
		if (next >= 0x80) { 
			// fprintf(stderr, "Fourth argument is 0x80; this does something unknown.\n");

			u8 final;
			fread8(&final, 1, bms_pointer);
			fprintf(txt_output_pointer, " [?? 0x%02X]", final);
		}
	}

	fprintf(txt_output_pointer, "\n");


}

void BMS_parse_cmd_wait_byte(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 time;

	fread8(&time, 1, bms_pointer);
	fprintf(txt_output_pointer, "cmdWaitByte: [WaitTime %d ticks]\n", time);
}

void BMS_parse_note_off(u8 current_byte, FILE *txt_output_pointer) {
	u8 voice = current_byte - 0x80;
	fprintf(txt_output_pointer, "Note Off: [voice %d]\n", voice);
}

void BMS_parse_unknown(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	if (current_byte == 0x98) {
		u8 param0, param1;

		fread8(&param0, 1, bms_pointer);
		fread8(&param1, 1, bms_pointer);

		fprintf(txt_output_pointer, "Unknown: [param 0x%02X] [param 0x%02X]\n", param0, param1);
	} else if (current_byte == 0x9A || current_byte == 0x9C) {
		u8 target, param0, param1;

		fread8(&target, 1, bms_pointer);
		fread8(&param0, 1, bms_pointer);
		fread8(&param1, 1, bms_pointer);

		if (target == 0x00) {
			u16   uvolume = (param0 << 8 | param1 << 0);
			float fvolume = ((float)uvolume) / 0x7FFF * 100.0f;

			fprintf(txt_output_pointer, "Param Set: [target: Volume] [value: %f%% (0x%02X%02X)]\n", fvolume, param0, param1);
		} else if (target == 0x01) {
			fprintf(
				txt_output_pointer,
				"Param Set: [target: Pitch] [value: %s%d]\n",
				(param0 == 0x01 ? "+" : param0 == 0xFF ? "-" : param0 == 0x00 ? "" : "?"),
				param1
			);
		} else if (target == 0x02) {
			fprintf(txt_output_pointer, "Param Set: [target: Reverb] [value: 0x%02X%02X]\n", param0, param1);
		} else if (target == 0x03) {
			fprintf(txt_output_pointer, "Param Set: [target: Pan] [value: 0x%02X%02X]\n", param0, param1);
		} else {
			fprintf(txt_output_pointer, "Param Set: [target: unknown (0x%02X)] [values: (0x%02X)(0x%02X)]\n", target, param0, param1);
		}
	} else {
		fprintf(txt_output_pointer, "Unknown\n");
	}
}

void BMS_parse_NULL(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	if (current_byte == 0xA4) {
		u8 param0, param1;

		fread8(&param0, 1, bms_pointer);
		fread8(&param1, 1, bms_pointer);

		if (param0 == 0x20) {
			fprintf(txt_output_pointer, "Set Bank: [Bank 0x%02X]\n", param1);
		} else if (param0 == 0x21) {
			fprintf(txt_output_pointer, "Set Program: [Bank 0x%02X]\n", param1);
		} else {
			fprintf(txt_output_pointer, "Unknown: [param 0x%02X] [param 0x%02X]\n", param0, param1);
		}

	} else if (current_byte == 0xAC || current_byte == 0xDD || current_byte == 0xEF) {
		u8 param0, param1, param2;

		fread8(&param0, 1, bms_pointer);
		fread8(&param1, 1, bms_pointer);
		fread8(&param2, 1, bms_pointer);

		fprintf(txt_output_pointer, "Unknown: [param 0x%02X] [param 0x%02X] [param 0x%02X]\n", param0, param1, param2);
	} else if (current_byte == 0xE6) {
		u16 value;

		fread16(&value, 1, bms_pointer);

		fprintf(txt_output_pointer, "Modulation: [value %04X]\n", value);
	} else if (current_byte == 0xE7) {
		u16 value;

		fread16(&value, 1, bms_pointer);

		fprintf(txt_output_pointer, "Modulation: [value %04X]\n", value);
	} else {
		fprintf(txt_output_pointer, "NULL\n");
	}
}

void BMS_parse_extended_opcode(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 next;
	fread8(&next, 1, bms_pointer);

	if      (next == 0x00) { fprintf(txt_output_pointer, "NULL: 0x%02X%02X\n",                    current_byte, next); }
	else if (next == 0x01) { fprintf(txt_output_pointer, "cmdDump\n"                                                ); }
	else                   { fprintf(txt_output_pointer, "Unknown extended opcode: 0x%02X%02X\n", current_byte, next); }
}

void BMS_parse_cmd_note_on       (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdNoteOn\n"     ); }
void BMS_parse_cmd_note_off      (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdNoteOff\n"    ); }
void BMS_parse_cmd_note          (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdNote\n"       ); }
void BMS_parse_cmd_set_last_note (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdSetLastNote\n"); }

void BMS_parse_cmd_param_e(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 target;
	u8 parameter;

	fread8(&target,    1, bms_pointer);
	fread8(&parameter, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdParamE: ");

	switch (target) {
		case 0x00: ;	// Dumb, hacky fix for a non C23 thing.
			f32 volume = (parameter > 0x7F) ? 0x00 : (float)parameter / (float)0x7F;
			fprintf(txt_output_pointer, "[Volume %f]\n", volume);
			break;

		case 0x01: ;
			const char *pitch = (parameter == 0x01) ? "Increase" : (parameter == 0xFF) ? "Decrease" : "Unknown";
			fprintf(txt_output_pointer, "[Pitch %s]\n", pitch);
			break;
		
		case 0x02: ;
			f32 reverb = (parameter > 0x7F) ? 0x00 : (float)parameter / (float)0x7F;
			fprintf(txt_output_pointer, "[Reverb %f]\n", reverb);
			break;
		
		case 0x03: ;
			const char *pan = (parameter == 0x07F) ? "Right" : (parameter == 0x00) ? "Center" : (parameter == 0x80) ? "Left" : "Unknown";
			fprintf(txt_output_pointer, "[Pan %s]\n", pan);
			break;
		
		default: ;
			fprintf(txt_output_pointer, "[Unknown target=0x%02X param=0x%02X]\n", target, parameter);
			break;
	}
}

void BMS_parse_cmd_param_i(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8  target;
	u16 parameter;

	fread8 (&target,    1, bms_pointer);
	fread16(&parameter, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdParamI: ");

	switch (target) {
		case 0x00: ;
			f32 volume = (parameter > 0x7FFF) ? 0x0000 : (float)parameter / (float)0x7FFF;
			fprintf(txt_output_pointer, "[Volume %f]\n", volume);
			break;

		case 0x01: ;
			const char *pitch = (parameter == 0x0100) ? "Increase" : (parameter == 0xFF00) ? "Decrease" : "Unknown";
			fprintf(txt_output_pointer, "[Pitch %s]\n", pitch);
			break;
		
		case 0x02: ;
			f32 reverb = (parameter > 0x7FFF) ? 0x0000 : (float)parameter / (float)0x7FFF;
			fprintf(txt_output_pointer, "[Reverb %f]\n", reverb);
			break;
		
		case 0x03: ;
			const char *pan = (parameter == 0x07FFF) ? "Right" : (parameter == 0x0000) ? "Center" : (parameter == 0x8000) ? "Left" : "Unknown";
			fprintf(txt_output_pointer, "[Pan %s]\n", pan);
			break;
		
		default: ;
			fprintf(txt_output_pointer, "[Unknown target=0x%02X param=0x%04X]\n", target, parameter);
			break;
	}
}

void BMS_parse_cmd_param_ei(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8  target;
	u8  parameter;
	u16 fade_time;

	fread8 (&target,    1, bms_pointer);
	fread8 (&parameter, 1, bms_pointer);
	fread16(&fade_time, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdParamI: ");

	switch (target) {
		case 0x00: ;
			f32 volume = (parameter > 0x7F) ? 0x00 : (float)parameter / (float)0x7F;
			fprintf(txt_output_pointer, "[Volume %f 0x%04Xticks]\n", volume, fade_time);
			break;

		case 0x01: ;
			const char *pitch = (parameter == 0x01) ? "Increase" : (parameter == 0xFF) ? "Decrease" : "Unknown";
			fprintf(txt_output_pointer, "[Pitch %s 0x%04Xticks]\n", pitch, fade_time);
			break;
		
		case 0x02: ;
			f32 reverb = (parameter > 0x7F) ? 0x00 : (float)parameter / (float)0x7F;
			fprintf(txt_output_pointer, "[Reverb %f 0x%04Xticks]\n", reverb, fade_time);
			break;
		
		case 0x03: ;
			const char *pan = (parameter == 0x07F) ? "Right" : (parameter == 0x00) ? "Center" : (parameter == 0x80) ? "Left" : "Unknown";
			fprintf(txt_output_pointer, "[Pan %s 0x%04Xticks]\n", pan, fade_time);
			break;
		
		default: ;
			fprintf(txt_output_pointer, "[Unknown target=0x%02X param=0x%02X fade_time=0x%04X]\n", target, parameter, fade_time);
			break;
	}
}

void BMS_parse_cmd_param_ii(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	u8  target;
	u16 parameter;
	u16 fade_time;

	fread8 (&target,    1, bms_pointer);
	fread16(&parameter, 1, bms_pointer);
	fread16(&fade_time, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdParamI: ");

	switch (target) {
		case 0x00: ;
			f32 volume = (parameter > 0x7FFF) ? 0x0000 : (float)parameter / (float)0x7FFF;
			fprintf(txt_output_pointer, "[Volume %f %dticks]\n", volume, fade_time);
			break;

		case 0x01: ;
			const char *pitch = (parameter == 0x0100) ? "Increase" : (parameter == 0xFF00) ? "Decrease" : "Unknown";
			fprintf(txt_output_pointer, "[Pitch %s %dticks]\n", pitch, fade_time);
			break;
		
		case 0x02: ;
			f32 reverb = (parameter > 0x7FFF) ? 0x0000 : (float)parameter / (float)0x7FFF;
			fprintf(txt_output_pointer, "[Reverb %f %dticks]\n", reverb, fade_time);
			break;
		
		case 0x03: ;
			const char *pan = (parameter == 0x07FFF) ? "Right" : (parameter == 0x0000) ? "Center" : (parameter == 0x8000) ? "Left" : "Unknown";
			fprintf(txt_output_pointer, "[Pan %s %dticks]\n", pan, fade_time);
			break;
		
		default: ;
			fprintf(txt_output_pointer, "[Unknown target=0x%02X param=0x%04X fade_time=%dticks]\n", target, parameter, fade_time);
			break;
	}
}

void BMS_parse_cmd_open_track(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 channel_number;
	u8 track_pointer_array[3];

	fread8(&channel_number,      1, bms_pointer);
	fread8( track_pointer_array, 3, bms_pointer);

	u32 track_pointer =
		track_pointer_array[0] << 16 |
		track_pointer_array[1] <<  8 |
		track_pointer_array[2];
	
	fprintf(txt_output_pointer, "cmdOpenTrack: [channel %d] [offset 0x%03X]\n", channel_number, track_pointer);
}

void BMS_parse_cmd_close_track(FILE *bms_pointer, FILE *txt_output_pointer) {
	fprintf(txt_output_pointer, "cmdCloseTrack\n\n");
}

void BMS_parse_cmd_call(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 track_pointer_array[3];

	fread8(track_pointer_array, 3, bms_pointer);

	u32 track_pointer =
		track_pointer_array[0] << 16 |
		track_pointer_array[1] <<  8 |
		track_pointer_array[2];
	
	fprintf(txt_output_pointer, "cmdCall: [offset 0x%06X]\n", track_pointer);
}

void BMS_parse_cmd_call_f(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 track_pointer_array[4];

	fread8(track_pointer_array, 4, bms_pointer);

	u32 track_pointer =
		track_pointer_array[0] << 24 |
		track_pointer_array[1] << 16 |
		track_pointer_array[2] <<  8 |
		track_pointer_array[3];
	
	fprintf(txt_output_pointer, "cmdCallF: [offset 0x%08X]\n", track_pointer);
}
void BMS_parse_cmd_ret    (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdRet\n"  ); }
void BMS_parse_cmd_ret_f  (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdRetF\n" ); }

void BMS_parse_cmd_jmp(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 track_pointer_array[3];

	fread8(track_pointer_array, 3, bms_pointer);

	u32 track_pointer =
		((u32)track_pointer_array[0]) << 16 |
		((u32)track_pointer_array[1]) <<  8 |
		((u32)track_pointer_array[2]);
	
	fprintf(txt_output_pointer, "cmdJmp: [offset 0x%06X]\n", track_pointer);
}

void BMS_parse_cmd_jmp_f(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 track_pointer_array[4];

	fread8(track_pointer_array, 4, bms_pointer);

	u32 track_pointer =
		((u32)track_pointer_array[0]) << 24 |
		((u32)track_pointer_array[1]) << 16 |
		((u32)track_pointer_array[2]) <<  8 |
		((u32)track_pointer_array[3]);
	
	fprintf(txt_output_pointer, "cmdJmpF: [offset 0x%08X]\n", track_pointer);
}

void BMS_parse_cmd_jmp_table  (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdJmpTable\n" ); }
void BMS_parse_cmd_call_table (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdCallTable\n"); }
void BMS_parse_cmd_loop_s     (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdLoopS\n"    ); }
void BMS_parse_cmd_loop_e     (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdLoopE\n"    ); }

void BMS_parse_cmd_read_port(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 source;
	u8 destination;

	fread8(&source,      1, bms_pointer);
	fread8(&destination, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdReadPort: [src 0x%02X] [dst 0x%02X]\n", source, destination);
}

void BMS_parse_cmd_write_port(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 source;
	u8 destination;

	fread8(&source,      1, bms_pointer);
	fread8(&destination, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdWritePort: [src 0x%02X] [dst 0x%02X]\n", source, destination);
}

void BMS_parse_cmd_check_port_import (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdCheckPortImport\n"); }
void BMS_parse_cmd_check_port_export (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdCheckPortExport\n"); }
void BMS_parse_cmd_parent_write_port (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdParentWritePort\n"); }
void BMS_parse_cmd_child_write_port  (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdChildWritePort\n" ); }
void BMS_parse_cmd_parent_read_port  (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdParentReadPort\n" ); }
void BMS_parse_cmd_child_read_port   (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdChildReadPort\n"  ); }

void BMS_parse_cmd_reg_load(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 setting;
	u8 unknown;
	u8 variable;

	fread8(&setting,  1, bms_pointer);
	fread8(&unknown,  1, bms_pointer);
	fread8(&variable, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdRegLoad: ");
	if (setting == 0x62) { fprintf(txt_output_pointer, "[BMS set PPQN (unknown 0x%02X) 0x%02X]\n", unknown, variable); }
	if (setting == 0x6B) { fprintf(txt_output_pointer, "[SC set PPQN (unknown 0x%02X) 0x%02X]\n",  unknown, variable); }
}

void BMS_parse_cmd_reg     (u8 current_byte, FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdReg: 0x%02X\n", current_byte); }
void BMS_parse_cmd_reg_uni (                 FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdRegUni\n"                 ); }

void BMS_parse_cmd_reg_tbl_load(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 access_mode;
	u8 target_pointer_array[3];
	u8 index;


	fread8(&access_mode,          1, bms_pointer);
	fread8( target_pointer_array, 3, bms_pointer);
	fread8(&index,                1, bms_pointer);

	u32 target =
		((u32)target_pointer_array[0]) << 16 |
		((u32)target_pointer_array[1]) <<  8 |
		((u32)target_pointer_array[2]);
	
	
	fprintf(txt_output_pointer, "cmdRegTblLoad: [AccessMode 0x%02X] [TargetDst 0x%03X] [Index 0x%02X]\n", access_mode, target, index);
}

void BMS_parse_cmd_tempo(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 unknown;
	u8 bpm;

	fread8(&unknown, 1, bms_pointer);
	fread8(&bpm,     1, bms_pointer);

	fprintf(txt_output_pointer, "cmdTempo: [Unknown 0x%02X] [BPM %d]\n", unknown, bpm);
}

void BMS_parse_cmd_bank_prg(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 bank;
	u8 program;

	fread8(&bank,    1, bms_pointer);
	fread8(&program, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdBankPrg: [Bank (WSYS ID) 0x%02X] [Program (IBNK LIST Entry) %d]\n", bank, program);
}

void BMS_parse_cmd_bank(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 bank;

	fread8(&bank, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdBank: [Bank (WSYS ID) 0x%02X]\n", bank);
}

void BMS_parse_cmd_prg(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 program;
	
	fread8(&program, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdPrg: [Program (IBNK LIST Entry) %d]\n", program);
}

void BMS_parse_cmd_env_scale_set (FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 param0, param1;

	fread8(&param0, 1, bms_pointer);
	fread8(&param1, 1, bms_pointer);
	
	fprintf(txt_output_pointer, "cmdEnvScaleSet: [unknown (0x%02X)(0x%02X)]\n", param0, param1);
}

void BMS_parse_cmd_env_set (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdEnvSet\n"     ); }

void BMS_parse_cmd_simple_adsr(FILE *bms_pointer, FILE *txt_output_pointer) {
	u16 attack;
	u16 sustain;
	u16 decay;
	u16 amplitude;
	u16 release;

	fread16(&attack,    1, bms_pointer);
	fread16(&sustain,   1, bms_pointer);
	fread16(&decay,     1, bms_pointer);
	fread16(&amplitude, 1, bms_pointer);
	fread16(&release,   1, bms_pointer);

	fprintf(
		txt_output_pointer,
		"cmdSimpleADSR: [A %dticks] [D %dticks] [S %dticks @ %f%%] [R %dticks]\n",
		       attack,
		       decay,
		       sustain,
		(float)amplitude / 0x7FFF,
		       release
	);
}

void BMS_parse_cmd_bus_connect(FILE *bms_pointer, FILE *txt_output_pointer) {
	u8 unknowns[3];

	fread8(unknowns, 3, bms_pointer);

	fprintf(txt_output_pointer, "cmdBusConnect: [Unknown 0x%02X] [Unknown 0x%02X] [Unknown 0x%02X]\n", unknowns[0], unknowns[1], unknowns[2]);
}

void BMS_parse_cmd_iir_cut_off (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdIIRCutOff\n"); }
void BMS_parse_cmd_iir_set     (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdIIRSet\n"   ); }
void BMS_parse_cmd_fir_set     (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdFIRSet\n"   ); }

void BMS_parse_cmd_wait(u8 current_byte, FILE *bms_pointer, FILE *txt_output_pointer) {
	if (current_byte == 0x88) {
		u16 wait_time;
		
		fread16(&wait_time, 1, bms_pointer);

		fprintf(txt_output_pointer, "cmdWait: [WaitTime %d ticks]\n", wait_time);
		return;
	}

	u64 total_wait_time;
	u8  _current_byte;

	do {

		fread8(&current_byte, 1, bms_pointer);
		total_wait_time += (u64)_current_byte;

	} while (_current_byte >= 0x80);

	fprintf(txt_output_pointer, "cmdWait: [WaitTime %llu ticks] [FIXME]\n", total_wait_time);
}

void BMS_parse_cmd_set_int_table (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdSetIntTable\n" ); }

void BMS_parse_cmd_set_interrupt (FILE*bms_pointer, FILE *txt_output_pointer) {
	u8 param;

	fread8(&param, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdSetInterrupt: [unknown 0x%02X]\n", param);
}

void BMS_parse_cmd_dis_interrupt (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdDisInterrupt\n"); }
void BMS_parse_cmd_ret_i         (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdRetI\n"        ); }
void BMS_parse_cmd_clr_i         (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdClrI\n"        ); }
void BMS_parse_cmd_int_timer     (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdIntTimer\n"    ); }
void BMS_parse_cmd_sync_cpu      (FILE *txt_output_pointer) { fprintf(txt_output_pointer, "cmdSyncCPU\n"     ); }

void BMS_parse_cmd_printf(FILE *bms_pointer, FILE *txt_output_pointer) {
	fprintf(txt_output_pointer, "cmdPrintf: \"\"\"\n\t");

	u8 next_char;

	fread8(&next_char, 1, bms_pointer);

	while (next_char != 0x00) {

		fprintf(txt_output_pointer, "%c", (char)next_char);
		fread8(&next_char, 1, bms_pointer);

	}

	fprintf(txt_output_pointer, "\"\"\"\n");
}

void BMS_parse_cmd_nop(FILE *bms_pointer, FILE *txt_output_pointer) {
	// u8 letssee;
	// fread8(&letssee, 1, bms_pointer);

	fprintf(txt_output_pointer, "cmdNop\n");
}

void BMS_parse_cmd_PPQN_set(FILE *bms_pointer, FILE *txt_output_pointer) {
	u16 ppqn;

	fread16(&ppqn,   1, bms_pointer);

	fprintf(txt_output_pointer, "cmdPPQNSet: [PPQN: %d]\n", ppqn);
}

void BMS_parse_cmd_finish(u32 **stack_pointer, FILE *bms_pointer, FILE *txt_output_pointer) {
	fseek(bms_pointer, **stack_pointer, SEEK_SET);
	(*stack_pointer)--;

	fprintf(txt_output_pointer, "cmdFinish\n\n");
}

//
//
//
//
//

void BMS_parse(FILE *bms_pointer, FILE *txt_output_pointer) {

	if (!bms_pointer       ) { fprintf(stderr, "File pointer is NULL\n"       ); return; }
	if (!txt_output_pointer) { fprintf(stderr, "Text output pointer is NULL\n"); return; }

	u8 current_byte;

	// In the event the program gets stuck in an infinite loop, kill the while loop if the number of
	// iterations excedes the file size
	fseek(bms_pointer, 0l, SEEK_END);
	u64 safety_limit = ftell(bms_pointer);
	fseek(bms_pointer, 0l, SEEK_SET);

	u32 safety = 0;

	while (
		fread(&current_byte, 1, 1, bms_pointer) == 1 &&
		safety++ < safety_limit
	) {
		// If the current byte is 0x00, it's probably the end of the file.
		if (current_byte == 0x00) { break; }

		fprintf(txt_output_pointer, "[0x%04lX] 0x%02X: ", ftell(bms_pointer) - 1l, current_byte);

		if (current_byte >= 0x01 && current_byte <= 0x7F) { BMS_parse_note_on               (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0x80                        ) { BMS_parse_cmd_wait_byte         (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0x81 && current_byte <= 0x87) { BMS_parse_note_off              (current_byte,              txt_output_pointer); continue; }
		if (current_byte == 0x88                        ) { BMS_parse_cmd_wait              (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0x89 && current_byte <= 0x9F) { BMS_parse_unknown               (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0xA0 && current_byte <= 0xAF) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xB0                        ) { BMS_parse_extended_opcode       (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xB1                        ) { BMS_parse_cmd_note_on           (                           txt_output_pointer); continue; }
		if (current_byte == 0xB2                        ) { BMS_parse_cmd_note_off          (                           txt_output_pointer); continue; }
		if (current_byte == 0xB3                        ) { BMS_parse_cmd_note              (                           txt_output_pointer); continue; }
		if (current_byte == 0xB4                        ) { BMS_parse_cmd_set_last_note     (                           txt_output_pointer); continue; }
		if (current_byte >= 0xB5 && current_byte <= 0xB7) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xB8                        ) { BMS_parse_cmd_param_e           (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xB9                        ) { BMS_parse_cmd_param_i           (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xBA                        ) { BMS_parse_cmd_param_ei          (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xBB                        ) { BMS_parse_cmd_param_ii          (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0xBC && current_byte <= 0xC0) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC1                        ) { BMS_parse_cmd_open_track        (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC2                        ) { BMS_parse_cmd_close_track       (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC3                        ) { BMS_parse_cmd_call              (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC4                        ) { BMS_parse_cmd_call_f            (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC5                        ) { BMS_parse_cmd_ret               (                           txt_output_pointer); continue; }
		if (current_byte == 0xC6                        ) { BMS_parse_cmd_ret_f             (                           txt_output_pointer); continue; }
		if (current_byte == 0xC7                        ) { BMS_parse_cmd_jmp               (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC8                        ) { BMS_parse_cmd_jmp_f             (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xC9                        ) { BMS_parse_cmd_jmp_table         (                           txt_output_pointer); continue; }
		if (current_byte == 0xCA                        ) { BMS_parse_cmd_call_table        (                           txt_output_pointer); continue; }
		if (current_byte == 0xCB                        ) { BMS_parse_cmd_loop_s            (                           txt_output_pointer); continue; }
		if (current_byte == 0xCC                        ) { BMS_parse_cmd_loop_e            (                           txt_output_pointer); continue; }
		if (current_byte >= 0xCD && current_byte <= 0xCF) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xD0                        ) { BMS_parse_cmd_read_port         (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xD1                        ) { BMS_parse_cmd_write_port        (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xD2                        ) { BMS_parse_cmd_check_port_import (                           txt_output_pointer); continue; }
		if (current_byte == 0xD3                        ) { BMS_parse_cmd_check_port_export (                           txt_output_pointer); continue; }
		if (current_byte == 0xD4                        ) { BMS_parse_cmd_parent_write_port (                           txt_output_pointer); continue; }
		if (current_byte == 0xD5                        ) { BMS_parse_cmd_child_write_port  (                           txt_output_pointer); continue; }
		if (current_byte == 0xD6                        ) { BMS_parse_cmd_parent_read_port  (                           txt_output_pointer); continue; }
		if (current_byte == 0xD7                        ) { BMS_parse_cmd_child_read_port   (                           txt_output_pointer); continue; }
		if (current_byte == 0xD8                        ) { BMS_parse_cmd_reg_load          (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xD9 || current_byte == 0xDA) { BMS_parse_cmd_reg               (current_byte,              txt_output_pointer); continue; }
		if (current_byte == 0xDB                        ) { BMS_parse_cmd_reg_uni           (                           txt_output_pointer); continue; }
		if (current_byte == 0xDC                        ) { BMS_parse_cmd_reg_tbl_load      (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0xDD && current_byte <= 0xDF) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE0                        ) { BMS_parse_cmd_tempo             (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE1                        ) { BMS_parse_cmd_bank_prg          (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE2                        ) { BMS_parse_cmd_bank              (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE3                        ) { BMS_parse_cmd_prg               (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte >= 0xE4 && current_byte <= 0xE6) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE7                        ) { BMS_parse_cmd_env_scale_set     (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xE8                        ) { BMS_parse_cmd_env_set           (                           txt_output_pointer); continue; }
		if (current_byte == 0xE9                        ) { BMS_parse_cmd_simple_adsr       (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xEA                        ) { BMS_parse_cmd_bus_connect       (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xEB                        ) { BMS_parse_cmd_iir_cut_off       (                           txt_output_pointer); continue; }
		if (current_byte == 0xEC                        ) { BMS_parse_cmd_iir_set           (                           txt_output_pointer); continue; }
		if (current_byte == 0xED                        ) { BMS_parse_cmd_fir_set           (                           txt_output_pointer); continue; }
		if (current_byte == 0xEE || current_byte == 0xEF) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xF0                        ) { BMS_parse_cmd_wait              (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xF1                        ) { BMS_parse_cmd_wait_byte         (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xF2                        ) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xF3                        ) { BMS_parse_cmd_set_int_table     (                           txt_output_pointer); continue; }
		if (current_byte == 0xF4                        ) { BMS_parse_cmd_set_interrupt     (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xF5                        ) { BMS_parse_cmd_dis_interrupt     (                           txt_output_pointer); continue; }
		if (current_byte == 0xF6                        ) { BMS_parse_cmd_int_timer         (                           txt_output_pointer); continue; }
		if (current_byte == 0xF7                        ) { BMS_parse_cmd_sync_cpu          (                           txt_output_pointer); continue; }
		if (current_byte >= 0xF8 && current_byte <= 0xFC) { BMS_parse_NULL                  (current_byte, bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xFD                        ) { BMS_parse_cmd_tempo             (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xFE                        ) { BMS_parse_cmd_PPQN_set          (              bms_pointer, txt_output_pointer); continue; }
		if (current_byte == 0xFF                        ) { BMS_parse_cmd_close_track       (              bms_pointer, txt_output_pointer); continue; }
 
	}
}

/// SMG2 COMMANDS
// if (current_byte >= 0x00 && current_byte <= 0x7F) { BMS_parse_note_on               (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0x80                        ) { BMS_parse_cmd_wait_byte         (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte >= 0x81 && current_byte <= 0x87) { BMS_parse_note_off              (current_byte,                txt_output_pointer); continue; }
// if (current_byte >= 0x88 && current_byte <= 0x9F) { BMS_parse_unknown               (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte >= 0xA0 && current_byte <= 0xAF) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xB0                        ) { BMS_parse_extended_opcode       (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xB1                        ) { BMS_parse_cmd_note_on           (                             txt_output_pointer); continue; }
// if (current_byte == 0xB2                        ) { BMS_parse_cmd_note_off          (                             txt_output_pointer); continue; }
// if (current_byte == 0xB3                        ) { BMS_parse_cmd_note              (                             txt_output_pointer); continue; }
// if (current_byte == 0xB4                        ) { BMS_parse_cmd_set_last_note     (                             txt_output_pointer); continue; }
// if (current_byte >= 0xB5 && current_byte <= 0xB7) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xB8                        ) { BMS_parse_cmd_param_e           (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xB9                        ) { BMS_parse_cmd_param_i           (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xBA                        ) { BMS_parse_cmd_param_ei          (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xBB                        ) { BMS_parse_cmd_param_ii          (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte >= 0xBC && current_byte <= 0xC0) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC1                        ) { BMS_parse_cmd_open_track        (&stack_pointer, bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC2                        ) { BMS_parse_cmd_close_track       (                             txt_output_pointer); continue; }
// if (current_byte == 0xC3                        ) { BMS_parse_cmd_call              (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC4                        ) { BMS_parse_cmd_call_f            (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC5                        ) { BMS_parse_cmd_ret               (                             txt_output_pointer); continue; }
// if (current_byte == 0xC6                        ) { BMS_parse_cmd_ret_f             (                             txt_output_pointer); continue; }
// if (current_byte == 0xC7                        ) { BMS_parse_cmd_jmp               (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC8                        ) { BMS_parse_cmd_jmp_f             (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xC9                        ) { BMS_parse_cmd_jmp_table         (                             txt_output_pointer); continue; }
// if (current_byte == 0xCA                        ) { BMS_parse_cmd_call_table        (                             txt_output_pointer); continue; }
// if (current_byte == 0xCB                        ) { BMS_parse_cmd_loop_s            (                             txt_output_pointer); continue; }
// if (current_byte == 0xCC                        ) { BMS_parse_cmd_loop_e            (                             txt_output_pointer); continue; }
// if (current_byte >= 0xCD && current_byte <= 0xCF) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xD0                        ) { BMS_parse_cmd_read_port         (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xD1                        ) { BMS_parse_cmd_write_port        (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xD2                        ) { BMS_parse_cmd_check_port_import (                             txt_output_pointer); continue; }
// if (current_byte == 0xD3                        ) { BMS_parse_cmd_check_port_export (                             txt_output_pointer); continue; }
// if (current_byte == 0xD4                        ) { BMS_parse_cmd_parent_write_port (                             txt_output_pointer); continue; }
// if (current_byte == 0xD5                        ) { BMS_parse_cmd_child_write_port  (                             txt_output_pointer); continue; }
// if (current_byte == 0xD6                        ) { BMS_parse_cmd_parent_read_port  (                             txt_output_pointer); continue; }
// if (current_byte == 0xD7                        ) { BMS_parse_cmd_child_read_port   (                             txt_output_pointer); continue; }
// if (current_byte == 0xD8                        ) { BMS_parse_cmd_reg_load          (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xD9 || current_byte == 0xDA) { BMS_parse_cmd_reg               (current_byte,                txt_output_pointer); continue; }
// if (current_byte == 0xDB                        ) { BMS_parse_cmd_reg_uni           (                             txt_output_pointer); continue; }
// if (current_byte == 0xDC                        ) { BMS_parse_cmd_reg_tbl_load      (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte >= 0xDD && current_byte <= 0xDF) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE0                        ) { BMS_parse_cmd_tempo             (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE1                        ) { BMS_parse_cmd_bank_prg          (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE2                        ) { BMS_parse_cmd_bank              (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE3                        ) { BMS_parse_cmd_prg               (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte >= 0xE4 && current_byte <= 0xE6) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE7                        ) { BMS_parse_cmd_env_scale_set     (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xE8                        ) { BMS_parse_cmd_env_set           (                             txt_output_pointer); continue; }
// if (current_byte == 0xE9                        ) { BMS_parse_cmd_simple_adsr       (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xEA                        ) { BMS_parse_cmd_bus_connect       (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xEB                        ) { BMS_parse_cmd_iir_cut_off       (                             txt_output_pointer); continue; }
// if (current_byte == 0xEC                        ) { BMS_parse_cmd_iir_set           (                             txt_output_pointer); continue; }
// if (current_byte == 0xED                        ) { BMS_parse_cmd_fir_set           (                             txt_output_pointer); continue; }
// if (current_byte == 0xEE || current_byte == 0xEF) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xF0                        ) { BMS_parse_cmd_wait              (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xF1                        ) { BMS_parse_cmd_wait_byte         (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xF2                        ) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xF3                        ) { BMS_parse_cmd_set_int_table     (                             txt_output_pointer); continue; }
// if (current_byte == 0xF4                        ) { BMS_parse_cmd_set_interrupt     (                             txt_output_pointer); continue; }
// if (current_byte == 0xF5                        ) { BMS_parse_cmd_dis_interrupt     (                             txt_output_pointer); continue; }
// if (current_byte == 0xF6                        ) { BMS_parse_cmd_int_timer         (                             txt_output_pointer); continue; }
// if (current_byte == 0xF7                        ) { BMS_parse_cmd_sync_cpu          (                             txt_output_pointer); continue; }
// if (current_byte >= 0xF8 && current_byte <= 0xFC) { BMS_parse_NULL                  (current_byte,   bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xFD                        ) { BMS_parse_cmd_printf            (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xFE                        ) { BMS_parse_cmd_nop               (                bms_pointer, txt_output_pointer); continue; }
// if (current_byte == 0xFF                        ) { BMS_parse_cmd_finish            (&stack_pointer, bms_pointer, txt_output_pointer); continue; }
