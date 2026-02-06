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

namespace JaiSSeqParser {

    struct parseNoteOn                { u8  opCode; u8 voice; u8 velocity; };

    struct cmdWaitByte                { u8  opCode; };
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
    
    struct cmdCall            { u8 opCode; u8 pointer[3]; };
    struct cmdCallF           { u8 opCode;                };
    
    struct cmdRet             { u8 opCode; };
    struct cmdRetF            { u8 opCode; };
    
    struct cmdJmp             { u8 opCode; u8 pointer[3]; };
    struct cmdJmpF            { u8 opCode;                };
    
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
    struct cmdWaitByte1       { u8 opCode; u8 waitTime;   };
    
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

// E7 00 00 A4 20 00 A4 21 -09- 9C 00 -7D FA- 9C 00 -7D FA- 9A 03 -6D- 0A A4 07 02 9C 01 00 00 E6 00 00         98 02 00 98 04 00 98 02 00 -45 01 2F 80 0B 4A 02 3E 80 09 4F 03 70 80 26 81 80 0B 82 80 0B 4E 01 5B 80 3C 4A 02 5D 80 39 83 80 03 45 03 55 80 39 81 80 03 43 01 4B 80 39 82 80 03 3E 02 40 80 39 83 80 03 39 03 36 80 39 81 80 3C 82 80 3C 83 88 01 6B 88 1E 00 C8 00 00 00 A7-                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              88 01 E0 FF
// E7 00 00 A4 20 00 A4 21 -02- 9C 00 -70 E0- 9C 00 -70 E0- 9A 03 -13- 0A A4 07 02 9C 01 00 00 E6 00 00         98 02 00 98 04 00 98 02 00 -80 0B 45 01 3D 80 05 4A 02 46 80 04 4F 03 58 80 07 81 80 05 82 80 04 83 80 2C 4E 01 69 80 10 81 80 2C 4A 01 5D 80 10 81 80 2C 45 01 58 80 10 81 80 2C 43 01 4F 80 10 81 80 2C 3E 01 51 80 10 81 80 2C 39 01 4D 80 10 81 88 02 0C 88 1E 00 C8 00 00 01 26-                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        88 01 E0 FF
// E7 00 00 A4 20 00 A4 21 -06- 9C 00 -70 E0- 9C 00 -70 E0- 9A 03 -47- 0A A4 07 02 9C 01 00 00 E6 00 00         98 02 00 98 04 00 98 02 00 -F4 41 88 03 98 88 0D 54 4A 01 53 80 3A 49 02 48 80 01 81 80 3D 82 80 02 4A 01 49 80 1A 81 80 60 4A 01 50 80 13 81 80 67 56 01 51 80 0D 81 80 69 4A 01 4C 80 14 81 80 67 4A 01 5D 80 12 81 80 64 4A 01 60 80 18 81 80 25 56 01 53 80 0D 81 80 30 56 01 4B 80 0D 81 80 64 48 01 57 80 3A 81 80 01 47 01 49 80 3D 81 80 02 48 01 5F 80 16 81 80 66 54 01 4B 80 0E 81 80 67 54 01 5C 80 0E 81 80 69 54 01 4E 80 41 53 02 40 80 01 81 80 0F 82 80 2B 54 01 53 80 0D 81 80 2F 56 01 46 80 3A 81 80 02 54 01 40 80 0D 81 80 2F 52 01 4A 80 10 81 80 2C 51 01 4B 80 0E 81 80 65 46 01 60 80 3D 81 80 01 45 01 4A 80 3C 81 80 02 46 01 58 80 1C 81 80 1F 4B 01 46 80 1C 81 80 23 52 01 4A 80 11 81 80 67 52 01 58 80 12 81 80 61 52 01 4F 80 12 81 80 2D 51 01 40 80 0E 81 80 32 52 01 46 80 39 81 80 02 54 01 42 80 13 81 80 27 52 01 4A 80 10 81 80 2D 51 01 44 80 0F 81 80 2D 4F 01 4E 80 11 81 80 2B 4E 01 54 80 10 4F 02 49 80 01 81 80 14 82 4E 01 4D 88 01 44 81 80 37 4E 01 4A 80 33 81 80 0D 56 01 42 80 79 E6 00 00 80 03 E6 00 60 80 04 E6 00 E0 80 04 E6 01 60 80 04 E6 01 E0 80 03 E6 02 E0 80 04 E6 03 80 80 04 E6 04 00 80 03 E6 04 E0 80 01 9C 00 7E FC 80 03 9C 00 7D FA 80 37 9C 00 7C F8 80 15 9C 00 7B F6 80 12 9C 00 7A F4 80 13 9C 00 79 F2 80 0A 9C 00 78 F0 80 0B 9C 00 77 EE 80 0F 9C 00 76 EC 80 07 9C 00 75 EA 80 0B 9C 00 74 E8 80 0B 9C 00 73 E6 80 07 9C 00 72 E4 80 07 9C 00 71 E2 80 08 9C 00 70 E0 80 05 81 80 3E E6 00 00 88 02 1C C8 00 00 01 5F-                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      88 01 E0 FF
// E7 00 00 A4 20 00 A4 21 -07- 9C 00 -59 B2- 9C 00 -59 B2- 9A 03 -33- 0A A4 07 02 9C 01 00 00 E6 00 00 [F4 40] 98 02 00 98 04 00 98 02 00 -88 01 B8 9C 00 4E 9C 80 3C 47 01 5B 4A 02 53 80 11 81 82 80 67 45 01 50 49 02 54 80 0F 9C 00 4F 9E 80 02 81 82 80 4E 9C 00 50 A0 80 19 47 01 5C 4A 02 5D 80 11 81 82 80 14 9C 00 51 A2 80 33 9C 00 52 A4 80 20 45 01 54 49 02 56 80 0F 9C 00 53 A6 80 02 81 82 80 26 9C 00 54 A8 80 05 80 22 9C 00 55 AA 80 1A 47 01 5C 4A 02 5C 80 07 9C 00 56 AC 80 0A 81 82 80 1A 9C 00 57 AE 80 1D 9C 00 58 B0 80 1D 9C 00 59 B2 80 13 45 01 58 49 02 56 80 0A 9C 00 5A B4 80 07 81 82 80 16 9C 00 5B B6 80 15 9C 00 5C B8 80 3C 47 01 56 4A 02 5C 80 11 81 82 80 2B 9C 00 58 B0 47 01 62 4A 02 60 80 03 9C 00 57 AE 80 0E 81 82 80 13 9C 00 56 AC 80 0E 9C 00 55 AA 80 0A 47 01 5D 4A 02 5D 80 01 9C 00 54 A8 80 0B 9C 00 53 A6 80 05 81 82 80 02 9C 00 52 A4 80 08 9C 00 51 A2 80 07 9C 00 50 A0 80 04 9C 00 4F 9E 80 07 9C 00 4E 9C 80 07 9C 00 4D 9A 80 04 9C 00 4C 98 80 04 9C 00 4E 9C 80 3C 45 01 56 48 02 54 80 11 81 82 80 67 43 01 5D 47 02 54 80 11 81 82 80 0D 9C 00 4F 9E 80 57 9C 00 50 A0 80 03 45 01 5E 48 02 5E 80 11 81 82 80 2D 9C 00 51 A2 80 3A 43 01 58 47 02 5A 9C 00 52 A4 80 11 81 82 80 1E 9C 00 53 A6 80 2F 9C 00 54 A8 80 1A 45 01 5E 48 02 60 80 0E 9C 00 55 AA 80 03 81 82 80 25 9C 00 56 AC 80 21 9C 00 57 AE 80 21 43 01 5A 47 02 5C 80 03 9C 00 58 B0 80 0E 81 82 80 13 9C 00 59 B2 80 18 9C 00 5A B4 80 3C 45 01 5C 48 02 5A 80 11 81 82 80 2B 9C 00 58 B0 45 01 62 48 02 64 80 03 9C 00 57 AE 80 0E 81 82 80 13 9C 00 56 AC 80 0E 9C 00 55 AA 80 0A 45 01 5D 48 02 61 80 01 9C 00 54 A8 80 0B 9C 00 53 A6 80 05 81 82 80 02 9C 00 52 A4 80 08 9C 00 51 A2 80 07 9C 00 50 A0 80 07 9C 00 4F 9E 80 04 9C 00 4E 9C 80 07 9C 00 4D 9A 80 04 9C 00 4C 98 80 04 9C 00 4E 9C 80 3C 43 01 56 46 02 56 80 11 81 82 80 67 41 01 56 45 02 4C 80 0F 9C 00 4F 9E 80 02 81 82 80 4E 9C 00 50 A0 80 19 43 01 60 46 02 56 80 11 81 82 80 14 9C 00 51 A2 80 32 9C 00 52 A4 80 21 41 01 5A 45 02 54 80 0F 9C 00 53 A6 80 02 81 82 80 25 9C 00 54 A8 80 28 9C 00 55 AA 80 1A 43 01 5C 46 02 5C 80 07 9C 00 56 AC 80 0A 81 82 80 1A 9C 00 57 AE 80 1D 9C 00 58 B0 80 21 9C 00 59 B2 80 0F 41 01 58 45 02 56 80 0A 9C 00 5A B4 80 07 81 82 80 16 9C 00 5B B6 80 15 9C 00 5C B8 80 3C 43 01 5F 46 02 5A 80 11 81 82 80 2B 9C 00 5A B4 43 01 68 46 02 66 80 03 9C 00 59 B2 80 0E 81 82 80 08 9C 00 58 B0 80 0B 9C 00 57 AE 80 07 9C 00 56 AC 80 04 9C 00 55 AA 80 07 9C 00 54 A8 80 04 9C 00 53 A6 80 02 43 01 63 46 02 58 80 05 9C 00 52 A4 80 03 9C 00 51 A2 80 04 9C 00 50 A0 80 04 9C 00 4F 9E 80 01 81 82 80 02 9C 00 4E 9C 80 04 9C 00 4D 9A 80 04 9C 00 4C 98 80 03 9C 00 4B 96 80 04 9C 00 49 92 80 03 9C 00 48 90 80 04 9C 00 47 8E 80 04 9C 00 46 8C 80 03 9C 00 44 88 80 04 9C 00 43 86 80 04 9C 00 41 82 80 03 9C 00 40 80 80 01 42 01 5C 45 02 58 80 02 9C 00 3F 7E 80 03 9C 00 40 80 80 04 9C 00 41 82 80 04 9C 00 43 86 80 03 9C 00 44 88 80 04 9C 00 45 8A 80 04 9C 00 46 8C 80 03 9C 00 47 8E 80 04 9C 00 48 90 80 03 9C 00 49 92 80 04 9C 00 4A 94 80 04 9C 00 4B 96 80 03 9C 00 4C 98 80 04 9C 00 4D 9A 80 04 9C 00 4E 9C 80 07 9C 00 4F 9E 80 03 9C 00 50 A0 80 04 9C 00 51 A2 80 07 9C 00 52 A4 80 08 9C 00 53 A6 80 07 9C 00 54 A8 80 0B 9C 00 55 AA 80 18 9C 00 56 AC 80 44 9C 00 54 A8 80 03 9C 00 53 A6 80 0B 9C 00 52 A4 80 0F 9C 00 51 A2 80 0A 9C 00 50 A0 80 0F 9C 00 4F 9E 80 12 9C 00 4E 9C 80 0F 9C 00 4D 9A 80 12 9C 00 4C 98 80 03 E6 00 00 80 06 80 03 E6 00 40 80 09 9C 00 4B 96 80 02 E6 00 C0 80 0B E6 01 40 80 09 9C 00 4A 94 80 02 E6 01 E0 80 0A E6 02 60 80 0B E6 03 00 80 03 9C 00 49 92 80 1C 9C 00 46 8C 80 04 9C 00 45 8A 80 3D 9C 00 44 88 80 1A 9C 00 43 86 80 15 9C 00 42 84 80 13 9C 00 41 82 80 0E 9C 00 40 80 80 0B 9C 00 3F 7E 80 0E 9C 00 3E 7C 80 0B 9C 00 3D 7A 80 0B 9C 00 3C 78 80 0B 9C 00 3B 76 80 0B 9C 00 3A 74 80 07 9C 00 39 72 80 0B 9C 00 38 70 80 07 9C 00 37 6E 80 08 9C 00 36 6C 80 05 9C 00 32 64 80 04 9C 00 31 62 80 0B 9C 00 30 60 80 07 9C 00 2F 5E 80 04 9C 00 2E 5C 80 03 9C 00 2D 5A 80 04 9C 00 2C 58 80 03 9C 00 2A 54 80 04 9C 00 29 52 80 04 9C 00 27 4E 80 03 9C 00 25 4A 80 04 9C 00 23 46 80 04 9C 00 21 42 80 03 9C 00 1F 3E 80 04 9C 00 1D 3A 80 03 9C 00 1A 34 80 04 9C 00 18 30 80 03 82 80 01 81 9C 00 15 2A 80 01 9C 00 14 28 80 78 E6 00 00 80 78 9C 00 4E 9C 80 3C 47 01 5A 4A 02 5A 80 11 81 82 80 67 45 01 5C 49 02 56 80 11 81 82 80 06 9C 00 4F 9E 80 53 9C 00 50 A0 80 0E 47 01 58 4A 02 5C 80 11 81 82 80 1F 9C 00 51 A2 80 36 9C 00 52 A4 80 12 45 01 5C 49 02 56 80 11 81 82 80 0C 9C 00 53 A6 80 2C 9C 00 54 A8 80 27 9C 00 55 AA 80 08 47 01 5C 4A 02 5E 80 11 81 82 80 0C 9C 00 56 AC 80 24 9C 00 57 AE 80 21 9C 00 58 B0 80 16 45 01 60 49 02 5C 80 07 9C 00 59 B2 80 0A 81 82 80 13 9C 00 5A B4 80 1D 9C 00 5B B6 80 19 9C 00 5C B8 80 1E 47 01 5C 4A 02 5E 80 11 81 82 80 2B 47 01 66 4A 02 64 80 11 81 82 80 0D 9C 00 5A B4 80 03 9C 00 59 B2 80 1A 9C 00 58 B0 80 01 47 01 64 4A 02 62 80 09 9C 00 57 AE 80 08 9C 00 56 AC 81 82 80 07 9C 00 55 AA 80 04 9C 00 54 A8 80 07 9C 00 53 A6 80 03 9C 00 52 A4 80 08 9C 00 51 A2 80 03 9C 00 50 A0 80 04 9C 00 4F 9E 80 04 9C 00 4E 9C 80 03 80 3C 45 01 5A 48 02 56 80 11 81 82 80 67 43 01 5A 47 02 4E 80 11 81 82 80 0D 9C 00 4F 9E 80 57 9C 00 50 A0 80 03 45 01 5E 48 02 58 80 11 81 82 80 2D 9C 00 51 A2 80 3A 43 01 58 47 02 54 9C 00 52 A4 80 11 81 82 80 1E 9C 00 53 A6 80 2F 9C 00 54 A8 80 1A 45 01 62 48 02 5C 80 0E 9C 00 55 AA 80 03 81 82 80 25 9C 00 56 AC 80 21 9C 00 57 AE 80 21 43 01 5A 47 02 5A 80 03 9C 00 58 B0 80 0E 81 82 80 13 9C 00 59 B2 80 18 9C 00 5A B4 80 3C 45 01 56 48 02 60 80 11 81 82 80 2B 45 01 66 48 02 68 80 11 81 82 80 0D 9C 00 58 B0 80 03 9C 00 57 AE 80 1A 9C 00 56 AC 80 01 45 01 60 48 02 62 80 0A 9C 00 55 AA 80 07 9C 00 54 A8 81 82 80 07 9C 00 53 A6 80 04 9C 00 52 A4 80 07 9C 00 51 A2 80 04 9C 00 50 A0 80 07 9C 00 4F 9E 80 03 9C 00 4E 9C 80 04 9C 00 4D 9A 80 04 9C 00 4C 98 80 03 9C 00 4E 9C 80 3C 43 01 59 46 02 58 80 11 81 82 80 67 41 01 54 45 02 56 80 0F 9C 00 4F 9E 80 02 81 82 80 4E 9C 00 50 A0 80 19 43 01 5E 46 02 5A 80 11 81 82 80 14 9C 00 51 A2 80 33 9C 00 52 A4 80 20 41 01 5A 45 02 52 80 0F 9C 00 53 A6 80 02 81 82 80 26 9C 00 54 A8 80 27 9C 00 55 AA 80 1A 43 01 5C 46 02 5E 80 07 9C 00 56 AC 80 0A 81 82 80 1A 9C 00 57 AE 80 1D 9C 00 58 B0 80 1D 9C 00 59 B2 80 13 41 01 5A 45 02 58 80 0A 9C 00 5A B4 80 07 81 82 80 16 9C 00 5B B6 80 15 9C 00 5C B8 80 3C 43 01 5E 46 02 5E 80 11 81 82 80 2B 43 01 62 46 02 66 80 11 81 82 80 2B 9C 00 58 B0 43 01 5E 46 02 5E 80 03 9C 00 57 AE 80 0B 9C 00 56 AC 80 03 81 82 80 01 9C 00 55 AA 80 03 9C 00 54 A8 80 04 9C 00 53 A6 80 04 9C 00 52 A4 80 03 9C 00 50 A0 80 04 9C 00 4F 9E 80 03 9C 00 4D 9A 80 04 9C 00 4B 96 80 04 9C 00 49 92 80 03 9C 00 46 8C 80 04 9C 00 44 88 80 04 9C 00 41 82 80 02 9C 00 42 84 42 01 5B 45 02 5C 80 03 9C 00 43 86 80 04 9C 00 44 88 80 04 9C 00 45 8A 80 03 9C 00 46 8C 80 04 9C 00 47 8E 80 03 9C 00 48 90 80 04 9C 00 4A 94 80 04 9C 00 4B 96 80 03 9C 00 4C 98 80 04 9C 00 4D 9A 80 07 9C 00 4E 9C 80 04 9C 00 4F 9E 80 03 9C 00 50 A0 80 04 9C 00 51 A2 80 04 9C 00 52 A4 80 07 9C 00 53 A6 80 04 9C 00 54 A8 80 03 9C 00 55 AA 80 07 9C 00 56 AC 80 08 9C 00 57 AE 80 07 9C 00 58 B0 80 07 9C 00 59 B2 80 07 9C 00 5A B4 80 0F 9C 00 5B B6 80 1B 9C 00 5C B8 80 3C 9C 00 5A B4 80 03 9C 00 59 B2 80 04 9C 00 58 B0 80 07 9C 00 57 AE 80 07 9C 00 56 AC 80 08 9C 00 55 AA 80 07 9C 00 54 A8 80 0B 9C 00 53 A6 80 07 9C 00 52 A4 80 07 9C 00 51 A2 80 0B 9C 00 50 A0 80 07 9C 00 4F 9E 80 0B 9C 00 4E 9C 80 06 80 05 9C 00 4D 9A 80 01 E6 00 20 80 03 E6 00 40 80 07 9C 00 4C 98 80 02 E6 00 C0 80 09 9C 00 4B 96 80 01 E6 01 40 80 09 E6 01 E0 80 04 9C 00 4A 94 80 06 E6 02 60 80 09 9C 00 49 92 E6 03 00 80 0E 9C 00 48 90 80 13 9C 00 47 8E 80 19 9C 00 46 8C 80 1E 80 03 9C 00 45 8A 80 28 9C 00 44 88 80 12 9C 00 43 86 80 0B 9C 00 42 84 80 0B 9C 00 41 82 80 0B 9C 00 40 80 80 0B 9C 00 3F 7E 80 07 9C 00 3E 7C 80 07 9C 00 3D 7A 80 08 9C 00 3C 78 80 07 9C 00 3B 76 80 07 9C 00 3A 74 80 04 9C 00 39 72 80 07 9C 00 38 70 80 04 9C 00 37 6E 80 07 9C 00 36 6C 80 03 9C 00 35 6A 80 08 9C 00 34 68 80 03 9C 00 33 66 80 08 9C 00 32 64 80 03 9C 00 31 62 80 04 9C 00 30 60 80 03 9C 00 2F 5E 80 08 9C 00 2E 5C 80 03 9C 00 2D 5A 80 04 9C 00 2C 58 80 04 9C 00 2B 56 80 03 9C 00 2A 54 80 04 9C 00 29 52 80 03 9C 00 28 50 80 04 9C 00 27 4E 80 04 9C 00 26 4C 80 03 9C 00 25 4A 80 04 9C 00 24 48 80 04 9C 00 23 46 80 03 9C 00 22 44 80 04 9C 00 21 42 80 03 9C 00 20 40 80 04 9C 00 1F 3E 80 04 9C 00 1E 3C 80 03 9C 00 1D 3A 80 03 82 80 01 9C 00 1C 38 80 05 9C 00 16 2C 80 03 81 80 72 E6 00 00 80 78 9C 00 4E 9C 80 3C 47 01 5B 4A 02 53 80 11 81 82 80 67 45 01 50 49 02 54 80 0F 9C 00 4F 9E 80 02 81 82 80 4E 9C 00 50 A0 80 19 47 01 5C 4A 02 5D 80 11 81 82 80 14 9C 00 51 A2 80 33 9C 00 52 A4 80 20 45 01 54 49 02 56 80 0F 9C 00 53 A6 80 02 81 82 80 26 9C 00 54 A8 80 05 C8 00 00 03 BE- 88 01 E0 FF
// E7 00 00 A4 20 00 A4 21 -05- 9C 00 -4A 94- 9C 00 -4A 94- 9A 03 -2C- 0A A4 07 02 9C 01 00 00 E6 00 00         98 02 00 98 04 00 98 02 00 -88 01 B8 37 01 4E 80 3C 81 80 3C 37 01 4C 80 37 81 80 41 37 01 44 80 35 81 80 43 37 01 44 80 37 81 80 41 37 01 44 80 35 81 80 43 37 01 43 80 37 81 80 41 37 01 43 80 37 81 80 41 37 01 43 80 37 81 80 41 35 01 4C 80 3E 81 80 3A 35 01 45 80 33 81 80 45 35 01 3C 80 33 81 80 45 35 01 3E 80 3B 81 80 3D 35 01 3E 80 40 81 80 38 35 01 3E 80 35 81 80 43 35 01 37 80 31 81 80 47 35 01 39 80 32 81 80 46 33 01 45 80 3C 81 80 3C 33 01 41 80 3A 81 80 3E 33 01 3B 80 45 81 80 33 33 01 3A 80 42 81 80 36 33 01 3F 80 36 81 80 42 33 01 3F 80 36 81 80 42 33 01 3B 80 3E 81 80 3A 33 01 3C 80 46 81 80 32 32 01 4C 80 38 81 80 40 32 01 40 80 3D 81 80 3B 32 01 40 80 3C 81 80 3C 32 01 40 80 3C 81 80 3C 32 01 3E 80 41 81 80 37 32 01 3E 80 3C 81 80 3C 32 01 3F 80 43 81 80 35 32 01 3F 80 40 81 80 38 37 01 4A 80 3C 81 80 3C 37 01 41 80 3A 81 80 3E 37 01 42 80 33 81 80 45 37 01 3E 80 36 81 80 42 37 01 3C 80 37 81 80 41 37 01 3D 80 37 81 80 41 37 01 3F 80 34 81 80 44 37 01 4C 80 38 81 80 40
// 

JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0xB2 @ 0xB2;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0x131 @ 0x131;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0x336 @ 0x336;
JaiSSeqParser::cmdFinish jaisseqparser_cmdfinish_at_0xE94 @ 0xE94;
u8 u8_array_at_0x37[0x49] @ 0x37;