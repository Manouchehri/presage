
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


#ifndef ISO8859_1
#define ISO8859_1

// ISO 8859-1 standard ///////////////////////

const int NUL         =  0;    // NUL
const int SOH         =  1;    // SOH
const int STX         =  2;    // STX
const int ETX         =  3;    // ETX
const int EOT         =  4;    // EOT
const int ENQ         =  5;    // ENQ
const int ACK         =  6;    // ACK
const int BEL         =  7;    // BEL
const int BS          =  8;    // BS 
const int HT          =  9;    // HT 
const int NL          =  10;   // NL 
const int VT          =  11;   // VT 
const int NP          =  12;   // NP 
const int CR          =  13;   // CR 
const int SO          =  14;   // SO 
const int SI          =  15;   // SI 
const int DLE         =  16;   // DLE
const int DC1         =  17;   // DC1
const int DC2         =  18;   // DC2
const int DC3         =  19;   // DC3
const int DC4         =  20;   // DC4
const int NAK         =  21;   // NAK
const int SYN         =  22;   // SYN
const int ETB         =  23;   // ETB
const int CAN         =  24;   // CAN
const int EM          =  25;   // EM 
const int SUB         =  26;   // SUB
const int ESC         =  27;   // ESC
const int FS          =  28;   // FS 
const int GS          =  29;   // GS 
const int RS          =  30;   // RS 
const int US          =  31;   // US 
const int space       =  32;   // normal space 
const int exclamation =  33;   // !
const int quote       =  34;   // "  
const int hashsign    =  35;   // #  
const int dollar      =  36;   // $  
const int percent     =  37;   // %  
const int ampersand   =  38;   // &  
const int apostrophe  =  39;   // '  
const int openbracket =  40;   // (  
const int closebracket=  41;   // )  
const int asterisk    =  42;   // *  
const int plus        =  43;   // +  
const int comma       =  44;   // ,  
const int minus       =  45;   // -  
const int period      =  46;   // .  
const int slash       =  47;   // /  
const int digit0      =  48;   // 0  
const int digit1      =  49;   // 1  
const int digit2      =  50;   // 2  
const int digit3      =  51;   // 3  
const int digit4      =  52;   // 4  
const int digit5      =  53;   // 5  
const int digit6      =  54;   // 6  
const int digit7      =  55;   // 7  
const int digit8      =  56;   // 8  
const int digit9      =  57;   // 9  
const int colon       =  58;   // :  
const int semicolon   =  59;   // ;  
const int lessthan    =  60;   // <  
const int equals      =  61;   // =  
const int greaterthan =  62;   // >  
const int question    =  63;   // ?  
const int at          =  64;   // @  
const int Aletter     =  65;   // A  
const int Bletter     =  66;   // B  
const int Cletter     =  67;   // C  
const int Dletter     =  68;   // D  
const int Eletter     =  69;   // E  
const int Fletter     =  70;   // F  
const int Gletter     =  71;   // G  
const int Hletter     =  72;   // H  
const int Iletter     =  73;   // I  
const int Jletter     =  74;   // J  
const int Kletter     =  75;   // K  
const int Lletter     =  76;   // L  
const int Mletter     =  77;   // M  
const int Nletter     =  78;   // N  
const int Oletter     =  79;   // O  
const int Pletter     =  80;   // P  
const int Qletter     =  81;   // Q  
const int Rletter     =  82;   // R  
const int Sletter     =  83;   // S  
const int Tletter     =  84;   // T  
const int Uletter     =  85;   // U  
const int Vletter     =  86;   // V  
const int Wletter     =  87;   // W  
const int Xletter     =  88;   // X  
const int Yletter     =  89;   // Y  
const int Zletter     =  90;   // Z  
const int opensquare  =  91;   // [  
const int backslash   =  92;   //   
const int closesquare =  93;   // ]  
const int pointer     =  94;   // ^  
const int underscore  =  95;   // _  
const int grave       =  96;   // `  
const int aletter     =  97;   // a  
const int bletter     =  98;   // b  
const int cletter     =  99;   // c  
const int dletter     = 100;   // d  
const int eletter     = 101;   // e  
const int fletter     = 102;   // f  
const int gletter     = 103;   // g  
const int hletter     = 104;   // h  
const int iletter     = 105;   // i  
const int jletter     = 106;   // j  
const int kletter     = 107;   // k  
const int lletter     = 108;   // l  
const int mletter     = 109;   // m  
const int nletter     = 110;   // n  
const int oletter     = 111;   // o  
const int pletter     = 112;   // p  
const int qletter     = 113;   // q  
const int rletter     = 114;   // r  
const int sletter     = 115;   // s  
const int tletter     = 116;   // t  
const int uletter     = 117;   // u  
const int vletter     = 118;   // v  
const int wletter     = 119;   // w  
const int xletter     = 120;   // x  
const int yletter     = 121;   // y  
const int zletter     = 122;   // z  
const int leftbrace   = 123;   // {  
const int verticalbar = 124;   // |  
const int rightbrace  = 125;   // }  
const int tilde       = 126;   // ~  


const int DEL = 127;   // DEL
const int PAD = 128;   // -- 
const int HOP = 129;   // -- 
const int BPH = 130;   // -- 
const int NBH = 131;   // -- 
const int IND = 132;   // -- 
const int NEL = 133;   // -- 
const int SSA = 134;   // -- 
const int ESA = 135;   // -- 
const int HTS = 136;   // -- 
const int HTJ = 137;   // -- 
const int VTS = 138;   // -- 
const int PLD = 139;   // -- 
const int PLU = 140;   // -- 
const int RI  = 141;   // -- 
const int SS2 = 142;   // -- 
const int SS3 = 143;   // -- 
const int DCS = 144;   // -- 
const int PU1 = 145;   // -- 
const int PU2 = 146;   // -- 
const int STS = 147;   // -- 
const int CCH = 148;   // -- 
const int MW  = 149;   // -- 
const int SPA = 150;   // -- 
const int EPA = 151;   // -- 
const int SOS = 152;   // -- 
const int SGCI= 153;   // -- 
const int SCI = 154;   // -- 
const int CSI = 155;   // -- 
const int ST  = 156;   // -- 
const int OSC = 157;   // -- 
const int PM  = 158;   // -- 
const int APC = 159;   // -- 
		     

const int nbsp   = 160;   //    non-breaking space
const int iexcl  = 161;   // ¡  
const int cent   = 162;   // ¢  
const int pound  = 163;   // £  
const int curren = 164;   // ¤  
const int yen    = 165;   // ¥  
const int brvbar = 166;   // ¦  
const int sect   = 167;   // §  
const int uml    = 168;   // ¨  
const int copyr  = 169;   // ©  
const int ordf   = 170;   // ª  
const int laquo  = 171;   // «  
const int notsign= 172;   // ¬  
const int shy    = 173;   // ­  
const int reg    = 174;   // ®  
const int macr   = 175;   // ¯  
const int deg    = 176;   // °  
const int plusmn = 177;   // ±  
const int sup2   = 178;   // ²  
const int sup3   = 179;   // ³  
const int acute  = 180;   // ´  
const int micro  = 181;   // µ  
const int para   = 182;   // ¶  
const int middot = 183;   // ·  
const int cedil  = 184;   // ¸  
const int sup1   = 185;   // ¹  
const int ordm   = 186;   // º  
const int raquo  = 187;   // »  
const int frac14 = 188;   // ¼  
const int frac12 = 189;   // ½  
const int frac34 = 190;   // ¾  
const int iquest = 191;   // ¿  
const int Agrave = 192;   // À  
const int Aacute = 193;   // Á  
const int Acirc  = 194;   // Â  
const int Atilde = 195;   // Ã  
const int Auml   = 196;   // Ä  
const int Aring  = 197;   // Å  
const int AElig  = 198;   // Æ  
const int Ccedil = 199;   // Ç  
const int Egrave = 200;   // È  
const int Eacute = 201;   // É  
const int Ecirc  = 202;   // Ê  
const int Euml   = 203;   // Ë  
const int Igrave = 204;   // Ì  
const int Iacute = 205;   // Í  
const int Icirc  = 206;   // Î  
const int Iuml   = 207;   // Ï  
const int ETH    = 208;   // Ð  
const int Ntilde = 209;   // Ñ  
const int Ograve = 210;   // Ò  
const int Oacute = 211;   // Ó  
const int Ocirc  = 212;   // Ô  
const int Otilde = 213;   // Õ  
const int Ouml   = 214;   // Ö  
const int times  = 215;   // ×  
const int Oslash = 216;   // Ø  
const int Ugrave = 217;   // Ù  
const int Uacute = 218;   // Ú  
const int Ucirc  = 219;   // Û  
const int Uuml   = 220;   // Ü  
const int Yacute = 221;   // Ý  
const int THORN  = 222;   // Þ  
const int szlig  = 223;   // ß  
const int agrave = 224;   // à  
const int aacute = 225;   // á  
const int acirc  = 226;   // â  
const int atilde = 227;   // ã  
const int auml   = 228;   // ä  
const int aring  = 229;   // å  
const int aelig  = 230;   // æ  
const int ccedil = 231;   // ç  
const int egrave = 232;   // è  
const int eacute = 233;   // é  
const int ecirc  = 234;   // ê  
const int euml   = 235;   // ë  
const int igrave = 236;   // ì  
const int iacute = 237;   // í  
const int icirc  = 238;   // î  
const int iuml   = 239;   // ï  
const int eth    = 240;   // ð  
const int ntilde = 241;   // ñ  
const int ograve = 242;   // ò  
const int oacute = 243;   // ó  
const int ocirc  = 244;   // ô  
const int otilde = 245;   // õ  
const int ouml   = 246;   // ö  
const int divide = 247;   // ÷  
const int oslash = 248;   // ø  
const int ugrave = 249;   // ù  
const int uacute = 250;   // ú  
const int ucirc  = 251;   // û  
const int uuml   = 252;   // ü  
const int yacute = 253;   // ý  
const int thorn  = 254;   // þ  
const int yuml   = 255;   // ÿ  


#endif // ISO8859_1
