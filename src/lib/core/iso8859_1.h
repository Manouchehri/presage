
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

const char NUL         =  0;    // NUL
const char SOH         =  1;    // SOH
const char STX         =  2;    // STX
const char ETX         =  3;    // ETX
const char EOT         =  4;    // EOT
const char ENQ         =  5;    // ENQ
const char ACK         =  6;    // ACK
const char BEL         =  7;    // BEL
const char BS          =  8;    // BS 
const char HT          =  9;    // HT 
const char NL          =  10;   // NL 
const char VT          =  11;   // VT 
const char NP          =  12;   // NP 
const char CR          =  13;   // CR 
const char SO          =  14;   // SO 
const char SI          =  15;   // SI 
const char DLE         =  16;   // DLE
const char DC1         =  17;   // DC1
const char DC2         =  18;   // DC2
const char DC3         =  19;   // DC3
const char DC4         =  20;   // DC4
const char NAK         =  21;   // NAK
const char SYN         =  22;   // SYN
const char ETB         =  23;   // ETB
const char CAN         =  24;   // CAN
const char EM          =  25;   // EM 
const char SUB         =  26;   // SUB
const char ESC         =  27;   // ESC
const char FS          =  28;   // FS 
const char GS          =  29;   // GS 
const char RS          =  30;   // RS 
const char US          =  31;   // US 
const char space       =  32;   // normal space 
const char exclamation =  33;   // !
const char quote       =  34;   // "  
const char hashsign    =  35;   // #  
const char dollar      =  36;   // $  
const char percent     =  37;   // %  
const char ampersand   =  38;   // &  
const char apostrophe  =  39;   // '  
const char openbracket =  40;   // (  
const char closebracket=  41;   // )  
const char asterisk    =  42;   // *  
const char plus        =  43;   // +  
const char comma       =  44;   // ,  
const char minus       =  45;   // -  
const char period      =  46;   // .  
const char slash       =  47;   // /  
const char digit0      =  48;   // 0  
const char digit1      =  49;   // 1  
const char digit2      =  50;   // 2  
const char digit3      =  51;   // 3  
const char digit4      =  52;   // 4  
const char digit5      =  53;   // 5  
const char digit6      =  54;   // 6  
const char digit7      =  55;   // 7  
const char digit8      =  56;   // 8  
const char digit9      =  57;   // 9  
const char colon       =  58;   // :  
const char semicolon   =  59;   // ;  
const char lessthan    =  60;   // <  
const char equals      =  61;   // =  
const char greaterthan =  62;   // >  
const char question    =  63;   // ?  
const char at          =  64;   // @  
const char Aletter     =  65;   // A  
const char Bletter     =  66;   // B  
const char Cletter     =  67;   // C  
const char Dletter     =  68;   // D  
const char Eletter     =  69;   // E  
const char Fletter     =  70;   // F  
const char Gletter     =  71;   // G  
const char Hletter     =  72;   // H  
const char Iletter     =  73;   // I  
const char Jletter     =  74;   // J  
const char Kletter     =  75;   // K  
const char Lletter     =  76;   // L  
const char Mletter     =  77;   // M  
const char Nletter     =  78;   // N  
const char Oletter     =  79;   // O  
const char Pletter     =  80;   // P  
const char Qletter     =  81;   // Q  
const char Rletter     =  82;   // R  
const char Sletter     =  83;   // S  
const char Tletter     =  84;   // T  
const char Uletter     =  85;   // U  
const char Vletter     =  86;   // V  
const char Wletter     =  87;   // W  
const char Xletter     =  88;   // X  
const char Yletter     =  89;   // Y  
const char Zletter     =  90;   // Z  
const char opensquare  =  91;   // [  
const char backslash   =  92;   //   
const char closesquare =  93;   // ]  
const char pointer     =  94;   // ^  
const char underscore  =  95;   // _  
const char grave       =  96;   // `  
const char aletter     =  97;   // a  
const char bletter     =  98;   // b  
const char cletter     =  99;   // c  
const char dletter     = 100;   // d  
const char eletter     = 101;   // e  
const char fletter     = 102;   // f  
const char gletter     = 103;   // g  
const char hletter     = 104;   // h  
const char iletter     = 105;   // i  
const char jletter     = 106;   // j  
const char kletter     = 107;   // k  
const char lletter     = 108;   // l  
const char mletter     = 109;   // m  
const char nletter     = 110;   // n  
const char oletter     = 111;   // o  
const char pletter     = 112;   // p  
const char qletter     = 113;   // q  
const char rletter     = 114;   // r  
const char sletter     = 115;   // s  
const char tletter     = 116;   // t  
const char uletter     = 117;   // u  
const char vletter     = 118;   // v  
const char wletter     = 119;   // w  
const char xletter     = 120;   // x  
const char yletter     = 121;   // y  
const char zletter     = 122;   // z  
const char leftbrace   = 123;   // {  
const char verticalbar = 124;   // |  
const char rightbrace  = 125;   // }  
const char tilde       = 126;   // ~  


const char DEL = 127;   // DEL
const char PAD = 128;   // -- 
const char HOP = 129;   // -- 
const char BPH = 130;   // -- 
const char NBH = 131;   // -- 
const char IND = 132;   // -- 
const char NEL = 133;   // -- 
const char SSA = 134;   // -- 
const char ESA = 135;   // -- 
const char HTS = 136;   // -- 
const char HTJ = 137;   // -- 
const char VTS = 138;   // -- 
const char PLD = 139;   // -- 
const char PLU = 140;   // -- 
const char RI  = 141;   // -- 
const char SS2 = 142;   // -- 
const char SS3 = 143;   // -- 
const char DCS = 144;   // -- 
const char PU1 = 145;   // -- 
const char PU2 = 146;   // -- 
const char STS = 147;   // -- 
const char CCH = 148;   // -- 
const char MW  = 149;   // -- 
const char SPA = 150;   // -- 
const char EPA = 151;   // -- 
const char SOS = 152;   // -- 
const char SGCI= 153;   // -- 
const char SCI = 154;   // -- 
const char CSI = 155;   // -- 
const char ST  = 156;   // -- 
const char OSC = 157;   // -- 
const char PM  = 158;   // -- 
const char APC = 159;   // -- 
		     

const char nbsp   = 160;   //    non-breaking space
const char iexcl  = 161;   // ¡  
const char cent   = 162;   // ¢  
const char pound  = 163;   // £  
const char curren = 164;   // ¤  
const char yen    = 165;   // ¥  
const char brvbar = 166;   // ¦  
const char sect   = 167;   // §  
const char uml    = 168;   // ¨  
const char copyr  = 169;   // ©  
const char ordf   = 170;   // ª  
const char laquo  = 171;   // «  
const char notsign= 172;   // ¬  
const char shy    = 173;   // ­  
const char reg    = 174;   // ®  
const char macr   = 175;   // ¯  
const char deg    = 176;   // °  
const char plusmn = 177;   // ±  
const char sup2   = 178;   // ²  
const char sup3   = 179;   // ³  
const char acute  = 180;   // ´  
const char micro  = 181;   // µ  
const char para   = 182;   // ¶  
const char middot = 183;   // ·  
const char cedil  = 184;   // ¸  
const char sup1   = 185;   // ¹  
const char ordm   = 186;   // º  
const char raquo  = 187;   // »  
const char frac14 = 188;   // ¼  
const char frac12 = 189;   // ½  
const char frac34 = 190;   // ¾  
const char iquest = 191;   // ¿  
const char Agrave = 192;   // À  
const char Aacute = 193;   // Á  
const char Acirc  = 194;   // Â  
const char Atilde = 195;   // Ã  
const char Auml   = 196;   // Ä  
const char Aring  = 197;   // Å  
const char AElig  = 198;   // Æ  
const char Ccedil = 199;   // Ç  
const char Egrave = 200;   // È  
const char Eacute = 201;   // É  
const char Ecirc  = 202;   // Ê  
const char Euml   = 203;   // Ë  
const char Igrave = 204;   // Ì  
const char Iacute = 205;   // Í  
const char Icirc  = 206;   // Î  
const char Iuml   = 207;   // Ï  
const char ETH    = 208;   // Ð  
const char Ntilde = 209;   // Ñ  
const char Ograve = 210;   // Ò  
const char Oacute = 211;   // Ó  
const char Ocirc  = 212;   // Ô  
const char Otilde = 213;   // Õ  
const char Ouml   = 214;   // Ö  
const char times  = 215;   // ×  
const char Oslash = 216;   // Ø  
const char Ugrave = 217;   // Ù  
const char Uacute = 218;   // Ú  
const char Ucirc  = 219;   // Û  
const char Uuml   = 220;   // Ü  
const char Yacute = 221;   // Ý  
const char THORN  = 222;   // Þ  
const char szlig  = 223;   // ß  
const char agrave = 224;   // à  
const char aacute = 225;   // á  
const char acirc  = 226;   // â  
const char atilde = 227;   // ã  
const char auml   = 228;   // ä  
const char aring  = 229;   // å  
const char aelig  = 230;   // æ  
const char ccedil = 231;   // ç  
const char egrave = 232;   // è  
const char eacute = 233;   // é  
const char ecirc  = 234;   // ê  
const char euml   = 235;   // ë  
const char igrave = 236;   // ì  
const char iacute = 237;   // í  
const char icirc  = 238;   // î  
const char iuml   = 239;   // ï  
const char eth    = 240;   // ð  
const char ntilde = 241;   // ñ  
const char ograve = 242;   // ò  
const char oacute = 243;   // ó  
const char ocirc  = 244;   // ô  
const char otilde = 245;   // õ  
const char ouml   = 246;   // ö  
const char divide = 247;   // ÷  
const char oslash = 248;   // ø  
const char ugrave = 249;   // ù  
const char uacute = 250;   // ú  
const char ucirc  = 251;   // û  
const char uuml   = 252;   // ü  
const char yacute = 253;   // ý  
const char thorn  = 254;   // þ  
const char yuml   = 255;   // ÿ  


#endif // ISO8859_1
