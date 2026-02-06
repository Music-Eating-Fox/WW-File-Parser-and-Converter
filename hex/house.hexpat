// 00 - 7F  JASSeqParser::parseNoteOn
// 80       JASSeqParser::cmdWaitByte
// 81 - 87  JASSeqParser::parseNoteOff
// 88 - 9F  Unknown
// A0 - AF  NULL
// B0       Extended Opcode Indicator
// B1       JASSeqParser::cmdNoteOn             
// B2       JASSeqParser::cmdNoteOff            
// B3       JASSeqParser::cmdNote           
// B4       JASSeqParser::cmdSetLastNote            
// B5 - B7  NULL
// B8       JASSeqParser::cmdParamE
// B9       JASSeqParser::cmdParamI
// BA       JASSeqParser::cmdParamEI
// BB       JASSeqParser::cmdParamII
// BC - C0  NULL
// C1       JASSeqParser::cmdOpenTrack
// C2       JASSeqParser::cmdCloseTrack             
// C3       JASSeqParser::cmdCall
// C4       JASSeqParser::cmdCallF          
// C5       JASSeqParser::cmdRet
// C6       JASSeqParser::cmdRetF           
// C7       JASSeqParser::cmdJmp
// C8       JASSeqParser::cmdJmpF           
// C9       JASSeqParser::cmdJmpTable           
// CA       JASSeqParser::cmdCallTable          
// CB       JASSeqParser::cmdLoopS          
// CC       JASSeqParser::cmdLoopE          
// CD - CF  NULL
// D0       JASSeqParser::cmdReadPort
// D1       JASSeqParser::cmdWritePort
// D2       JASSeqParser::cmdCheckPortImport            
// D3       JASSeqParser::cmdCheckPortExport            
// D4       JASSeqParser::cmdParentWritePort            
// D5       JASSeqParser::cmdChildWritePort             
// D6       JASSeqParser::cmdParentReadPort             
// D7       JASSeqParser::cmdChildReadPort          
// D8       JASSeqParser::cmdRegLoad
// D9       JASSeqParser::cmdReg            
// DA       JASSeqParser::cmdReg            
// DB       JASSeqParser::cmdRegUni             
// DC       JASSeqParser::cmdRegTblLoad 
// DD - DF  NULL
// E0       JASSeqParser::cmdTempo
// E1       JASSeqParser::cmdBankPrg
// E2       JASSeqParser::cmdBank
// E3       JASSeqParser::cmdPrg
// E4 - E6  NULL
// E7       JASSeqParser::cmdEnvScaleSet            
// E8       JASSeqParser::cmdEnvSet             
// E9       JASSeqParser::cmdSimpleADSR
// EA       JASSeqParser::cmdBusConnect
// EB       JASSeqParser::cmdIIRCutOff          
// EC       JASSeqParser::cmdIIRSet             
// ED       JASSeqParser::cmdFIRSet             
// EE - EF  NULL
// F0       JASSeqParser::cmdWait
// F1       JASSeqParser::cmdWaitByte
// F2       NULL
// F3       JASSeqParser::cmdSetIntTable            
// F4       JASSeqParser::cmdSetInterrupt           
// F5       JASSeqParser::cmdDisInterrupt           
// F6       JASSeqParser::cmdRetI           
// F7       JASSeqParser::cmdClrI           
// F8       JASSeqParser::cmdIntTimer           
// F9       JASSeqParser::cmdSyncCPU            
// FA - FC  NULL
// FD       JASSeqParser::cmdPrintf
// FE       JASSeqParser::cmdNop            
// FF       JASSeqParser::cmdFinish
// B000     NULL            
// B001     JASSeqParser::cmdDump 

using f32 = float;
using f64 = double;

namespace JaiSSeqParser {

    struct parseNoteOn                { u8  opCode; u8 voice; u8 velocity; };

    struct cmdWaitByte                { u8  opCode; u8 waitTime; };
    struct parseNoteOff               { u8  opCode; };
    struct unknown                    { u8  opCode; };
    struct NULL8                      { u8  opCode; };
    struct NULL16                     { u16 opCode; };
    
    struct cmdNoteOn          { u8 opCode; };
    struct cmdNoteOff         { u8 opCode; };
    struct cmdNote            { u8 opCode; };
    struct cmdSetLastNote     { u8 opCode; };
    
    struct cmdParamE          { u8 opCode; u8 target; u8  parameter; };
    struct cmdParamI          { u8 opCode; u8 target; u16 parameter; };
    
    struct cmdParamEI         { u8 opCode; u8 target; u8  parameter; u16 fadeTime; };
    struct cmdParamII         { u8 opCode; u8 target; u16 parameter; u16 fadeTime; };
    
    struct cmdOpenTrack       { u8 opCode; u8 channelNumber; u8 trackPointer[3]; };
    struct cmdCloseTrack      { u8 opCode;                                       };
    
    struct cmdCall            { u8 opCode; u8  pointer[3]; };
    struct cmdCallF           { u8 opCode; f32 pointer;    };
    
    struct cmdRet             { u8  opCode; };
    struct cmdRetF            { f32 opCode; };
    
    struct cmdJmp             { u8 opCode; u8  pointer[3]; };
    struct cmdJmpF            { u8 opCode; f32 pointer;    };
    
    struct cmdJmpTable        { u8 opCode; };
    struct cmdCallTable       { u8 opCode; };
    struct cmdLoopS           { u8 opCode; };
    struct cmdLoopE           { u8 opCode; };
    
    struct cmdReadPort        { u8 opCode; u8 source; u8 destination; };
    struct cmdWritePort       { u8 opCode; u8 source; u8 destination; };
    
    struct cmdCheckPortImport { u8 opCode; };
    struct cmdCheckPortExport { u8 opCode; };
    struct cmdParentWritePort { u8 opCode; };
    struct cmdChildWritePort  { u8 opCode; };
    struct cmdParentReadPort  { u8 opCode; };
    struct cmdChildReadPort   { u8 opCode; };
    
    struct cmdRegLoad         { u8 opCode; u8 setting; u8 unknown; u8 variable; };
    
    struct cmdReg             { u8 opCode; };
    struct cmdRegUni          { u8 opCode; };
    
    struct cmdRegTblLoad      { u8 opCode; u8 accessMode; u8 destination; u8 offset[3]; u8 index; };
    
    struct cmdTempo           { u8 opCode; u8 unknown; u8 BPM;     };
    struct cmdBankPrg         { u8 opCode; u8 bank;    u8 program; };
    
    struct cmdBank            { u8 opCode; u8 bank;    };
    struct cmdPrg             { u8 opCode; u8 program; };
    
    struct cmdEnvScaleSet     { u8 opCode; };
    struct cmdEnvSet          { u8 opCode; };
    
    struct cmdSimpleADSR      { u8 opCode; u16 attack; u16 decay; u16 amplitude; u16 release; };
    
    struct cmdBusConnect      { u8 opcode; u8 unknown0; u8 unknown1; u8 unknown2; };
    
    struct cmdIIRCutOff       { u8 opCode; };
    struct cmdIIRSet          { u8 opCode; };
    struct cmdFIRSet          { u8 opCode; };
    
    struct cmdWait            { u8 opCode; u8 waitTime[]; };
    
    struct cmdSetIntTable     { u8 opCode; };
    struct cmdSetInterrupt    { u8 opCode; };
    struct cmdDisInterrupt    { u8 opCode; };
    struct cmdRetI            { u8 opCode; };
    struct cmdClrI            { u8 opCode; };
    struct cmdIntTimer        { u8 opCode; };
    struct cmdSyncCPU         { u8 opCode; };
    
    struct cmdPrintf          { u8 opCode; u8 message[]; u8 nullTerminator; };
    
    struct cmdNop             { u8 opCode; };
    struct cmdFinish          { u8 opCode; };
    
    struct cmdDump            { u16 opCode; };
    
    enum cmdType : u8 {
        parseNoteOn        = 0x00 ... 0x7F,
        cmdWaitByte        = 0x80,
        parseNoteOff       = 0x81 ... 0x87,
        Unknown            = 0x88 ... 0x9F,
        extOpcodeIndicator = 0xB0,
        cmdNoteOn          = 0xB1,
        cmdNoteOff         = 0xB2,
        cmdNote            = 0xB3,
        cmdSetLastNote     = 0xB4,
        cmdParamE          = 0xB8,
        cmdParamI          = 0xB9,
        cmdParamEI         = 0xBA,
        cmdParamII         = 0xBB,
        cmdOpenTrack       = 0xC1,
        cmdCloseTrack      = 0xC2,
        cmdCall            = 0xC3,
        cmdCallF           = 0xC4,
        cmdRet             = 0xC5,
        cmdRetF            = 0xC6,
        cmdJmp             = 0xC7,
        cmdJmpF            = 0xC8,
        cmdJmpTable        = 0xC9,
        cmdCallTable       = 0xCA,
        cmdLoopS           = 0xCB,
        cmdLoopE           = 0xCC,
        cmdReadPort        = 0xD0,
        cmdWritePort       = 0xD1,
        cmdCheckPortImport = 0xD2,
        cmdCheckPortExport = 0xD3,
        cmdParentWritePort = 0xD4,
        cmdChildWritePort  = 0xD5,
        cmdParentReadPort  = 0xD6,
        cmdChildReadPort   = 0xD7,
        cmdRegLoad         = 0xD8,
        cmdReg             = 0xD9,
        cmdReg             = 0xDA,
        cmdRegUni          = 0xDB,
        cmdRegTblLoad      = 0xDC,
        cmdTempo           = 0xE0,
        cmdBankPrg         = 0xE1,
        cmdBank            = 0xE2,
        cmdPrg             = 0xE3,
        cmdEnvScaleSet     = 0xE7,
        cmdEnvSet          = 0xE8,
        cmdSimpleADSR      = 0xE9,
        cmdBusConnect      = 0xEA,
        cmdIIRCutOff       = 0xEB,
        cmdIIRSet          = 0xEC,
        cmdFIRSet          = 0xED,
        cmdWait            = 0xF0,
        cmdWaitByte        = 0xF1,
        cmdSetIntTable     = 0xF3,
        cmdSetInterrupt    = 0xF4,
        cmdDisInterrupt    = 0xF5,
        cmdRetI            = 0xF6,
        cmdClrI            = 0xF7,
        cmdIntTimer        = 0xF8,
        cmdSyncCPU         = 0xF9,
        cmdPrintf          = 0xFD,
        cmdNop             = 0xFE,
        cmdFinish          = 0xFF,
        
        NULL,               //= 0xA0 ... 0xAF | 0xB5 ... 0xB7 |  0xBC ... 0xC0 | 0xCD ... 0xCF | 0xDD ... 0xDF | 0xE4 ... 0xE6 | 0xEE ... 0xEF | 0xF2 | 0xFA ... 0xFC | 0xB0,
    }; 
    
}

// fn parse() {
//     JaiSSeqParser::cmdType cmds[1024];
// 
//     u32 cmdsToSkip = 0;
// 
//     for (u32 i = 0, i < 1024, i += 1) {
//         match (currentCmd + i) {
//             ( JaiSSeqParser::cmdType::parseNoteOn        ): cmds[i] = 4095JaiSSeqParser::parseNoteOn;
//             ( JaiSSeqParser::cmdType::cmdWaitByte        ): cmds[i] = JaiSSeqParser::cmdWaitByte;
//             ( JaiSSeqParser::cmdType::parseNoteOff       ): cmds[i] = JaiSSeqParser::parseNoteOff;
//             ( JaiSSeqParser::cmdType::Unknown            ): cmds[i] = JaiSSeqParser::Unknown;
//             ( JaiSSeqParser::cmdType::cmdNoteOn          ): cmds[i] = JaiSSeqParser::cmdNoteOn;
//             ( JaiSSeqParser::cmdType::cmdNoteOff         ): cmds[i] = JaiSSeqParser::cmdNoteOff;
//             ( JaiSSeqParser::cmdType::cmdNote            ): cmds[i] = JaiSSeqParser::cmdNote;
//             ( JaiSSeqParser::cmdType::cmdSetLastNote     ): cmds[i] = JaiSSeqParser::cmdSetLastNote;
//             ( JaiSSeqParser::cmdType::cmdParamE          ): cmds[i] = JaiSSeqParser::cmdParamE;
//             ( JaiSSeqParser::cmdType::cmdParamI          ): cmds[i] = JaiSSeqParser::cmdParamI;
//             ( JaiSSeqParser::cmdType::cmdParamEI         ): cmds[i] = JaiSSeqParser::cmdParamEI;
//             ( JaiSSeqParser::cmdType::cmdParamII         ): cmds[i] = JaiSSeqParser::cmdParamII;
//             ( JaiSSeqParser::cmdType::cmdOpenTrack       ): cmds[i] = JaiSSeqParser::cmdOpenTrack;
//             ( JaiSSeqParser::cmdType::cmdCloseTrack      ): cmds[i] = JaiSSeqParser::cmdCloseTrack;
//             ( JaiSSeqParser::cmdType::cmdCall            ): cmds[i] = JaiSSeqParser::cmdCall;
//             ( JaiSSeqParser::cmdType::cmdCallF           ): cmds[i] = JaiSSeqParser::cmdCallF;
//             ( JaiSSeqParser::cmdType::cmdRet             ): cmds[i] = JaiSSeqParser::cmdRet;
//             ( JaiSSeqParser::cmdType::cmdRetF            ): cmds[i] = JaiSSeqParser::cmdRetF;
//             ( JaiSSeqParser::cmdType::cmdJmp             ): cmds[i] = JaiSSeqParser::cmdJmp;
//             ( JaiSSeqParser::cmdType::cmdJmpF            ): cmds[i] = JaiSSeqParser::cmdJmpF;
//             ( JaiSSeqParser::cmdType::cmdJmpTable        ): cmds[i] = JaiSSeqParser::cmdJmpTable;
//             ( JaiSSeqParser::cmdType::cmdCallTable       ): cmds[i] = JaiSSeqParser::cmdCallTable;
//             ( JaiSSeqParser::cmdType::cmdLoopS           ): cmds[i] = JaiSSeqParser::cmdLoopS;
//             ( JaiSSeqParser::cmdType::cmdLoopE           ): cmds[i] = JaiSSeqParser::cmdLoopE;
//             ( JaiSSeqParser::cmdType::cmdReadPort        ): cmds[i] = JaiSSeqParser::cmdReadPort;
//             ( JaiSSeqParser::cmdType::cmdWritePort       ): cmds[i] = JaiSSeqParser::cmdWritePort;
//             ( JaiSSeqParser::cmdType::cmdCheckPortImport ): cmds[i] = JaiSSeqParser::;
//             ( JaiSSeqParser::cmdType::cmdCheckPortExport ): cmds[i] = JaiSSeqParser::;
//             ( JaiSSeqParser::cmdType::cmdParentWritePort ): cmds[i] = JaiSSeqParser::;
//             ( JaiSSeqParser::cmdType::cmdChildWritePort  ): cmds[i] = JaiSSeqParser::cmdChildWritePort;
//             ( JaiSSeqParser::cmdType::cmdParentReadPort  ): cmds[i] = JaiSSeqParser::cmdParentReadPort;
//             ( JaiSSeqParser::cmdType::cmdChildReadPort   ): cmds[i] = JaiSSeqParser::cmdChildReadPort;
//             ( JaiSSeqParser::cmdType::cmdRegLoad         ): cmds[i] = JaiSSeqParser::cmdRegLoad;
//             ( JaiSSeqParser::cmdType::cmdReg             ): cmds[i] = JaiSSeqParser::cmdReg;
//             ( JaiSSeqParser::cmdType::cmdReg             ): cmds[i] = JaiSSeqParser::cmdReg;
//             ( JaiSSeqParser::cmdType::cmdRegUni          ): cmds[i] = JaiSSeqParser::cmdRegUni;
//             ( JaiSSeqParser::cmdType::cmdRegTblLoad      ): cmds[i] = JaiSSeqParser::cmdRegTblLoad;
//             ( JaiSSeqParser::cmdType::cmdTempo           ): cmds[i] = JaiSSeqParser::cmdTempo;
//             ( JaiSSeqParser::cmdType::cmdBankPrg         ): cmds[i] = JaiSSeqParser::cmdBankPrg;
//             ( JaiSSeqParser::cmdType::cmdBank            ): cmds[i] = JaiSSeqParser::cmdBank;
//             ( JaiSSeqParser::cmdType::cmdPrg             ): cmds[i] = JaiSSeqParser::cmdPrg;
//             ( JaiSSeqParser::cmdType::cmdEnvScaleSet     ): cmds[i] = JaiSSeqParser::cmdEnvScaleSet;
//             ( JaiSSeqParser::cmdType::cmdEnvSet          ): cmds[i] = JaiSSeqParser::cmdEnvSet;
//             ( JaiSSeqParser::cmdType::cmdSimpleADSR      ): cmds[i] = JaiSSeqParser::cmdSimpleADSR;
//             ( JaiSSeqParser::cmdType::cmdBusConnect      ): cmds[i] = JaiSSeqParser::cmdBusConnect;
//             ( JaiSSeqParser::cmdType::cmdIIRCutOff       ): cmds[i] = JaiSSeqParser::cmdIIRCutOff;
//             ( JaiSSeqParser::cmdType::cmdIIRSet          ): cmds[i] = JaiSSeqParser::cmdIIRSet;
//             ( JaiSSeqParser::cmdType::cmdFIRSet          ): cmds[i] = JaiSSeqParser::cmdFIRSet;
//             ( JaiSSeqParser::cmdType::cmdWait            ): cmds[i] = JaiSSeqParser::cmdWait;
//             ( JaiSSeqParser::cmdType::cmdWaitByte        ): cmds[i] = JaiSSeqParser::cmdWaitByte;
//             ( JaiSSeqParser::cmdType::cmdSetIntTable     ): cmds[i] = JaiSSeqParser::cmdSetIntTable;
//             ( JaiSSeqParser::cmdType::cmdSetInterrupt    ): cmds[i] = JaiSSeqParser::cmdSetInterrupt;
//             ( JaiSSeqParser::cmdType::cmdDisInterrupt    ): cmds[i] = JaiSSeqParser::cmdDisInterrupt;
//             ( JaiSSeqParser::cmdType::cmdRetI            ): cmds[i] = JaiSSeqParser::cmdRetI;
//             ( JaiSSeqParser::cmdType::cmdClrI            ): cmds[i] = JaiSSeqParser::cmdClrI;
//             ( JaiSSeqParser::cmdType::cmdIntTimer        ): cmds[i] = JaiSSeqParser::cmdIntTimer;
//             ( JaiSSeqParser::cmdType::cmdSyncCPU         ): cmds[i] = JaiSSeqParser::cmdSyncCPU;
//             ( JaiSSeqParser::cmdType::cmdPrintf          ): cmds[i] = JaiSSeqParser::cmdPrintf;
//             ( JaiSSeqParser::cmdType::cmdNop             ): cmds[i] = JaiSSeqParser::cmdNop;
//             ( JaiSSeqParser::cmdType::cmdFinish          ): cmds[i] = JaiSSeqParser::cmdFinish;
//         }
//     }
//     
//     return cmds;
// };
// 
// struct file {
// 
//     JaiSSeqParser::cmdType cmds [1024] = parse();
//     
// };


JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x00 @ 0x00;
JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x05 @ 0x05;
JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x0A @ 0x0A;
JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x0F @ 0x0F;
JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x14 @ 0x14;
JaiSSeqParser::cmdOpenTrack jaisseqparser_cmdopentrack_at_0x19 @ 0x19;
JaiSSeqParser::cmdNop jaisseqparser_cmdnop_at_0x1E @ 0x1E;
JaiSSeqParser::cmdEnvScaleSet jaisseqparser_cmdenvscaleset_at_0x36 @ 0x36;
JaiSSeqParser::cmdEnvScaleSet jaisseqparser_cmdenvscaleset_at_0xB3 @ 0xB3;
JaiSSeqParser::cmdEnvScaleSet jaisseqparser_cmdenvscaleset_at_0x132 @ 0x132;
JaiSSeqParser::cmdEnvScaleSet jaisseqparser_cmdenvscaleset_at_0x337 @ 0x337;
JaiSSeqParser::cmdEnvScaleSet jaisseqparser_cmdenvscaleset_at_0xE95 @ 0xE95;

JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0xB2 @ 0xB2;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0x131 @ 0x131;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0x336 @ 0x336;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0xE94 @ 0xE94;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x5E @ 0x5E;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x63 @ 0x63;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x68 @ 0x68;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x73 @ 0x73;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0xDD @ 0xDD;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0xE0 @ 0xE0;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x61 @ 0x61;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x66 @ 0x66;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x6B @ 0x6B;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x6D @ 0x6D;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x6E @ 0x6E;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x70 @ 0x70;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x71 @ 0x71;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x76 @ 0x76;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x78 @ 0x78;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x7B @ 0x7B;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x7D @ 0x7D;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x7E @ 0x7E;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x80 @ 0x80;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x83 @ 0x83;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x85 @ 0x85;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x86 @ 0x86;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0x35 @ 0x35;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x88 @ 0x88;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x8B @ 0x8B;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x8D @ 0x8D;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x8E @ 0x8E;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x90 @ 0x90;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte1_at_0x93 @ 0x93;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x95 @ 0x95;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x96 @ 0x96;
JaiSSeqParser::parseNoteOn jaisseqparser_parsenoteon_at_0x98 @ 0x98;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x9B @ 0x9B;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0x9D @ 0x9D;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0x9E @ 0x9E;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0xA0 @ 0xA0;
JaiSSeqParser::cmdWaitByte jaisseqparser_cmdwaitbyte_at_0xA1 @ 0xA1;
JaiSSeqParser::parseNoteOff jaisseqparser_parsenoteoff_at_0xA3 @ 0xA3;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA4 @ 0xA4;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA5 @ 0xA5;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA6 @ 0xA6;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA7 @ 0xA7;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA8 @ 0xA8;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xA9 @ 0xA9;
JaiSSeqParser::cmdJmpF jaisseqparser_cmdjmpf_at_0xAA @ 0xAA;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xAF @ 0xAF;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xB0 @ 0xB0;
JaiSSeqParser::unknown jaisseqparser_unknown_at_0xB1 @ 0xB1;