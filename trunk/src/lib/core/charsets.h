
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


#ifndef PRESAGE_CHARSETS
#define PRESAGE_CHARSETS

// ISO 8859-1 standard ///////////////////////
#include "iso8859_1.h"


// FIXME: ensure values are correct
const char RIGHT_ARROW = 28;
const char LEFT_ARROW  = 29;
const char UP_ARROW    = 30;
const char DOWN_ARROW  = 31;
const char END         = 128;    // fix this!!!
const char HOME        = 128;    // <----------
const char BACKSPACE   = '\b';
const char DELETE      = 18;     // ?
const char PAGE_UP     = 128;
const char PAGE_DOWN   = 128;


const char DEFAULT_WORD_CHARS[]={
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
	
    Agrave  ,
    Aacute  ,
    Acirc   ,
    Atilde  ,
    Auml    ,
    Aring   ,
    AElig   ,
    Ccedil  ,
    Egrave  ,
    Eacute  ,
    Ecirc   ,
    Euml    ,
    Igrave  ,
    Iacute  ,
    Icirc   ,
    Iuml    ,
    ETH     ,
    Ntilde  ,
    Ograve  ,
    Oacute  ,
    Ocirc   ,
    Otilde  ,
    Ouml    ,
    times   ,
    Oslash  ,
    Ugrave  ,
    Uacute  ,
    Ucirc   ,
    Uuml    ,
    Yacute  ,
    THORN   ,
    szlig   ,
    agrave  ,
    aacute  ,
    acirc   ,
    atilde  ,
    auml    ,
    aring   ,
    aelig   ,
    ccedil  ,
    egrave  ,
    eacute  ,
    ecirc   ,
    euml    ,
    igrave  ,
    iacute  ,
    icirc   ,
    iuml    ,
    eth     ,
    ntilde  ,
    ograve  ,
    oacute  ,
    ocirc   ,
    otilde  ,
    ouml    ,
    divide  ,
    oslash  ,
    ugrave  ,
    uacute  ,
    ucirc   ,
    uuml    ,
    yacute  ,
    thorn   ,
    yuml    ,

    '\0' };

const char DEFAULT_SEPARATOR_CHARS[]={
    '!',
    '"',
    '#',
    '$',
    '%',
    '&',
    '\'',
    '(',
    ')',
    '*',
    '+',
    ',',
    '-',
    '.',
    '/',
    ':',
    ';',
    '<',
    '=',
    '>',
    '?',
    '@',
    '[',
    '\\',
    ']',
    '^',
    '_',
    '`',
    '{',
    '|',
    '}',
    '~',
    '\0' };

const char DEFAULT_BLANKSPACE_CHARS[]={
    ' ',  // space
    '\f', // form feed
    '\n', // newline
    '\r', // carriage return
    '\t', // horizontal tab
    '\v', // vertical tab
    '\0' };

const char DEFAULT_CONTROL_CHARS[]={
    RIGHT_ARROW,
    LEFT_ARROW,
    UP_ARROW,
    DOWN_ARROW,
    END,
    HOME,
    BACKSPACE,
    DELETE,
    PAGE_UP,
    PAGE_DOWN,
    '\0' };


#endif // PRESAGE_CHARSETS
